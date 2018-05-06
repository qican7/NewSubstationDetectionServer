    /*M/////////////////////////////////////////////////////////////////////////////////////// 
    // 
    //  基于OpenCV和Winsock的图像传输（接收） 
    //   
    //   By 漆灿 , at CUST, 2017.03.06   
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
      
    //待传输图像默认大小为 640*480，可修改  
    //#define IMG_WIDTH 640   // 需传输图像的宽  
    //#define IMG_HEIGHT 480  // 需传输图像的高  
	#define IMG_WIDTH 320   // 需传输图像的宽  
	#define IMG_HEIGHT 240  // 需传输图像的高  
    //默认格式为CV_8UC3  
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
		string detectDate;
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
        // 打开socket连接  
        // params : PORT    传输端口  
        // return : -1      连接失败  
        //          1       连接成功  
        int socketConnect(int PORT);  
      
      
        // 传输图像  
        // params : image   待接收图像  
        // return : -1      接收失败  
        //          1       接收成功  
		int receive(cv::Mat image,cv::Mat image_1,cv::Mat image_2,string &deviceType,int &x,int &y,int &abnormalDimension,float &areaMaxTemp,float &areaAvgTemp,float &referTemp,float &enviTemp, float &diffTemp,float &speed,int &level, string &detectDate, string &detectTime);  

      
      
        // 断开socket连接  
        void socketDisconnect(void);  
    };  
      
    #endif  