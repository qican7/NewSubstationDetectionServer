    /*M/////////////////////////////////////////////////////////////////////////////////////// 
    // 
    //  ����OpenCV��Winsock��ͼ���䣨���գ� 
    //   
    //   By ��� , at CUST, 2017.03.06   
    // 
    //  website: www.pengz0807.com  email: pengz0807@163.com  
    //   
    //M*/  
      
    #ifndef __WINSOCKMATTRANSMISSIONSEVER_H__  
    #define __WINSOCKMATTRANSMISSIONSEVER_H__  
      
    #include "opencv2/opencv.hpp"  
    #include "opencv2/highgui/highgui.hpp"  
    #include "opencv2/imgproc/imgproc.hpp"  
    #include "opencv2/core/core.hpp"  
    #include <stdio.h>  
    #include <Winsock2.h>  

	#include <string>
	using namespace std;
      
    #pragma comment(lib,"WS2_32.lib")  
      
    //������ͼ��Ĭ�ϴ�СΪ 640*480�����޸�  
    //#define IMG_WIDTH 640   // �贫��ͼ��Ŀ�  
    //#define IMG_HEIGHT 480  // �贫��ͼ��ĸ�  
	#define IMG_WIDTH 320   // �贫��ͼ��Ŀ�  
	#define IMG_HEIGHT 240  // �贫��ͼ��ĸ�  
    //Ĭ�ϸ�ʽΪCV_8UC3  
    #define BUFFER_SIZE IMG_WIDTH*IMG_HEIGHT*3/40
      
    struct recvbuf  
    {  
		char buf[BUFFER_SIZE];  
		char buf_1[BUFFER_SIZE];
		char buf_2[BUFFER_SIZE];
		int flag;  
		string deviceType;
		int x;
		int y;
		int abnormalDimension;
		float areaMaxTemp;
		float areaAvgTemp;
		float referTemp;
		float enviTemp;
		float diffTemp;
		float speed;
		int level ;
        string detectTime;
    };  
      
    class WinsockMatTransmissionServer  
    {  
    public:  
        WinsockMatTransmissionServer(void);  
        ~WinsockMatTransmissionServer(void);  
      
    private:  
        
      
    public:  
		SOCKET sockConn;  
		struct recvbuf data;  
		SOCKET sockSrv;
        // ��socket����  
        // params : PORT    ����˿�  
        // return : -1      ����ʧ��  
        //          1       ���ӳɹ�  
        int socketConnect(int PORT);  
      
      
        // ����ͼ��  
        // params : image   ������ͼ��  
        // return : -1      ����ʧ��  
        //          1       ���ճɹ�  
		int receive(cv::Mat image,cv::Mat image_1,cv::Mat image_2,string &deviceType,int &x,int &y,int &abnormalDimension,float &areaMaxTemp,float &areaAvgTemp,float &referTemp,float &enviTemp, float &diffTemp,float &speed,int &level, string &detectTime);  

      
      
        // �Ͽ�socket����  
        void socketDisconnect(void);  
    };  
      
    #endif  