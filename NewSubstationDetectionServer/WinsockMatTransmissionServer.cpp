    /*M/////////////////////////////////////////////////////////////////////////////////////// 
    // 
    //  基于OpenCV和Winsock的图像传输（接收） 
    //   
    //  By 漆灿 , at CUST, 2017.03.06  
    // 
    //  website: www.pengz0807.com  email: pengz0807@163.com  
    //   
    //M*/  
      
    #include "StdAfx.h" 
    #include "WinsockMatTransmissionServer.h" 
    #include "Utils.h"
      
      
    WinsockMatTransmissionServer::WinsockMatTransmissionServer(void)  
    {  
    }  
      
      
    WinsockMatTransmissionServer::~WinsockMatTransmissionServer(void)  
    {  
    }  
      
      
    int WinsockMatTransmissionServer::socketConnect(int PORT)  
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
      
        int nRecvBuf = 1024 * 1024 * 10;  
        setsockopt(sockConn, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int));  
    }  
      
      
    void WinsockMatTransmissionServer::socketDisconnect(void)  
    {  
        closesocket(sockConn);  
    }  
      
    int WinsockMatTransmissionServer::receive(cv::Mat image,cv::Mat image_1,cv::Mat image_2,int &position,string &deviceType,int &x,int &y,int &abnormalDimension,float &areaMaxTemp,float &areaAvgTemp,float &referTemp,float &enviTemp,float &diffTemp,float &speed,int &level, int &year,int &month,int &day,int &hour,int &minute,int &second)  
    {  
        cv::Mat img(IMG_HEIGHT,IMG_WIDTH,CV_8UC3,cv::Scalar(0));  
		cv::Mat img_1(IMG_HEIGHT,IMG_WIDTH,CV_8UC3,cv::Scalar(0));  
		cv::Mat img_2(IMG_HEIGHT,IMG_WIDTH,CV_8UC3,cv::Scalar(0));  
      
        int needRecv = sizeof(recvbuf);  
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
      
                int num1 = IMG_HEIGHT / 40 * i;  
                for (int j = 0; j < IMG_HEIGHT / 40; j++)  
                {  
                    int num2 = j * IMG_WIDTH * 3;  
                    uchar* ucdata = img.ptr<uchar>(j + num1);  
                    for (int k = 0; k < IMG_WIDTH * 3; k++)  
                    {  
                        ucdata[k] = data.buf[num2 + k];  
                    }  
                }  

				int num1_1 = IMG_HEIGHT / 40 * i;  
				for (int j = 0; j < IMG_HEIGHT / 40; j++)  
				{  
					int num2_1 = j * IMG_WIDTH * 3;  
					uchar* ucdata_1 = img_1.ptr<uchar>(j + num1_1);  
					for (int k = 0; k < IMG_WIDTH * 3; k++)  
					{  
						ucdata_1[k] = data.buf_1[num2_1 + k];  
					}  
				}  

				int num1_2 = IMG_HEIGHT / 40 * i;  
				for (int j = 0; j < IMG_HEIGHT / 40; j++)  
				{  
					int num2_2 = j * IMG_WIDTH * 3;  
					uchar* ucdata_2 = img_2.ptr<uchar>(j + num1_2);  
					for (int k = 0; k < IMG_WIDTH * 3; k++)  
					{  
						ucdata_2[k] = data.buf_2[num2_2 + k];  
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
                        image = img;  
						image_1 = img_1;
						image_2 = img_2;
						position = data.position;
						deviceType = data.deviceType;
						x = data.x;
						y = data.y;
						abnormalDimension = data.abnormalDimension;
						areaMaxTemp = data.areaMaxTemp;
						areaAvgTemp = data.areaAvgTemp;
						referTemp = data.referTemp;
						enviTemp = data.enviTemp;
						diffTemp = data.diffTemp;
						speed = data.speed;
						level = data.level;
						year = data.year;
						month = data.month;
						day = data.day;
						hour = data.hour;
						minute = data.minute;
						second = data.second;
						//AfxMessageBox(intToCString(abnormalDimension)); //不匹配则给出提示框
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