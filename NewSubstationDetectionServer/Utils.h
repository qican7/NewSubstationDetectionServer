#ifndef UTILS_H
#define UTILS_H

#include<stdio.h>  
#include <iostream> 
#include "stdafx.h"

#include "cv.h"
#include "highgui.h"

using namespace std; 
using namespace cv; 

//��������ת��
extern CString intToCString(int data);
extern CString floatToCString(float data);
extern CString stringToCString(string data);


extern char* CStringToChar(CString data);
extern int CStringToInt(CString data);
extern float CStringToFloat(CString data);
extern CString Save2Float(float data); //float���ݱ�����λС��




extern IplImage*  IplImage_colorToGray(IplImage* pic); //��ɫת�Ҷ�
extern Mat Mat_colorToGray(Mat image); //��ɫת�Ҷ�

extern IplImage* MatToIplImage(Mat image); //MatתIplImage
extern Mat IplImageToMat(IplImage* image); //IplImageתMat

extern IplImage matToIplImage(Mat* mat);


#endif