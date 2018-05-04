    /*M/////////////////////////////////////////////////////////////////////////////////////// 
    // 
    //  基于OpenCV和Winsock的图像传输（接收） 
    //   
    //   By 漆灿 , at CUST, 2017.03.06   
    // 
    //  website: www.pengz0807.com  email: pengz0807@163.com  
    //   
    //M*/  
      
    #include "StdAfx.h" 
    #include "WinsockMatTransmissionServer_1.h"  
    #include "Utils.h"
      
      
    WinsockMatTransmissionServer_1::WinsockMatTransmissionServer_1(void)  
    {  
    }  
      
      
    WinsockMatTransmissionServer_1::~WinsockMatTransmissionServer_1(void)  
    {  
    }  
      
      
    int WinsockMatTransmissionServer_1::socketConnect(int PORT)  
    {  
        WORD wVersionRequested;  
        WSADATA wsaData;  
        int err;  
      
        wVersionRequested = MAKEWORD(1, 1);  
      
        err = WSAStartup(wVersionRequested, &wsaData);  
      
        if (err != 0)  
        {  
            return -1;  
        }  
      
        if (LOBYTE(wsaData.wVersion) != 1 ||  
            HIBYTE(wsaData.wVersion) != 1)  
        {  
            WSACleanup();  
            return -1;  
        }  
      
        sockSrv = socket(AF_INET, SOCK_STREAM, 0);  
      
        SOCKADDR_IN addrSrv;  
        addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);  
        addrSrv.sin_family = AF_INET;  
        addrSrv.sin_port = htons(PORT);  
        bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));  
        listen(sockSrv, 5);  
      
        SOCKADDR_IN addrClient;  
        int len = sizeof(SOCKADDR);  
        sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);  
      
        int nrecvbuf_1 = 1024 * 1024 * 10;  
        setsockopt(sockConn, SOL_SOCKET, SO_RCVBUF, (const char*)&nrecvbuf_1, sizeof(int));  
    }  
      
      
    void WinsockMatTransmissionServer_1::socketDisconnect(void)  
    {  
        closesocket(sockConn);  
    }  
      
    int WinsockMatTransmissionServer_1::receive(cv::Mat &image_infrare,cv::Mat &image_light)  
    {  
        cv::Mat img_infrare(IMG_HEIGHT,IMG_WIDTH,CV_8UC3,cv::Scalar(0));  
		cv::Mat img_light(IMG_HEIGHT,IMG_WIDTH,CV_8UC3,cv::Scalar(0));  
      
        int needRecv = sizeof(recvbuf_1);  
        int count = 0;  
        //name = data.name;
		
		
        while (1)  
        {  
            for (int i = 0; i < 40; i++)  
            {  
                int pos = 0;  
                int len0 = 0;  
      
                while (pos < needRecv)  
                {  
                    len0 = recv(sockConn, (char*)(&data) + pos, needRecv - pos, 0);  
                    if (len0 < 0)  
                    {  
                        printf("Server Recieve Data Failed!\n");  
                        return -1;  
                    }  
                    pos += len0;  
                }  
      
                count = count + data.flag;  
      
				//接受红外图像
                int num1 = IMG_HEIGHT / 40 * i;  
                for (int j = 0; j < IMG_HEIGHT / 40; j++)  
                {  
                    int num2 = j * IMG_WIDTH * 3;  
                    uchar* ucdata = img_infrare.ptr<uchar>(j + num1);  
                    for (int k = 0; k < IMG_WIDTH * 3; k++)  
                    {  
                        ucdata[k] = data.buf[num2 + k];  
                    }  
                }  

				//接受可见光图像
				int num3 = IMG_HEIGHT / 40 * i;  
				for (int j = 0; j < IMG_HEIGHT / 40; j++)  
				{  
					int num4 = j * IMG_WIDTH * 3;  
					uchar* ucdata = img_light.ptr<uchar>(j + num3);  
					for (int k = 0; k < IMG_WIDTH * 3; k++)  
					{  
						ucdata[k] = data.buf[num4 + k];  
					}  
				}  

				
      
                if (data.flag == 2)  
                {  
					/*std::cout<<data.name<<" "<<data.maxTemp<<" "<<data.position<<std::endl; 
					memcpy(nameString,data.name,sizeof(data.name));
					temp = data.maxTemp;
					memcpy(positionTemp,data.position,sizeof(data.position));*/

                    if (count == 41)  
                    {  
					
                        image_infrare = img_infrare;  
						image_light = img_light;


						//AfxMessageBox(intToCString(ad)); //不匹配则给出提示框
						//std::cout<<x<<" "<<y<<" "<<abnormalDimension<<" "<<areaMinTemp<<" "<<areaMaxTemp<<" "<<areaAvgTemp<<" "<<referTemp<<" "<<diffTemp<<" "<<speed<<" "<<level<<" "<<year<<" "<<month<<" "<<day<<" "<<hour<<" "<<minute<<" "<<second<<std::endl;
						count = 0;
                        return 1;  
                          
                    }  
                    else  
                    {  
                        count = 0;  
                        i = 0;  
                    }  
                }  
            }  
        }  
    }  