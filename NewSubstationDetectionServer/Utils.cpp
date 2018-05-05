#include "stdafx.h"
#include "Utils.h"

CString intToCString(int data){

	CString t;
	t.Format(_T("%d"), data);
	return t;
}

CString floatToCString(float data){

	CString t;
	t.Format(_T("%f"), data);
	return t;

}

CString stringToCString(string data){
	CString t;

	t = data.c_str();

	//t.Format(_T("%s"), data);
	return t;
}



char* CStringToChar(CString data){

	char *data1 = new char[100];
	WideCharToMultiByte(CP_ACP,0,data.GetBuffer(),-1,data1,100,NULL,NULL);//Unicodeת���ɶ��ֽ��ַ���

	return data1;
}

int CStringToInt(CString data){

	int number = _ttoi(data);
	return number;
}

float CStringToFloat(CString data){

	float number = _ttof(data);
	return number;
}

//float���ݱ�����λС��
extern CString Save2Float(float data){

	char data_1[10] = {0};
	sprintf(data_1,"%0.2f",data);
	return (CString)data_1;
}


string CStringToString(CString data){

	string s = CT2CA(data.GetBuffer(0));
	return s;

}



//��ɫת�Ҷ�
IplImage*  IplImage_colorToGray(IplImage* pic){

	IplImage* picGray = cvCreateImage(cvGetSize(pic), IPL_DEPTH_8U, 1);//����Ŀ��ͼ��  
	cvCvtColor(pic,picGray,CV_BGR2GRAY);//cvCvtColor(src,des,CV_BGR2GRAY) 

	return picGray;
}

//��ɫת�Ҷ�
Mat Mat_colorToGray(Mat image){

	Mat destImg;
	cvtColor(image, destImg, CV_BGR2GRAY); // תΪ�Ҷ�ͼ��
	return destImg;
}

//MatתIplImage
IplImage* MatToIplImage(Mat image){

	IplImage pImg = IplImage(image);

	return &pImg;
}

//IplImageתMat
Mat IplImageToMat(IplImage* image){

	Mat img(image,0);

	return img;
}

IplImage matToIplImage(Mat* mat){
	return IplImage(*mat);
}

