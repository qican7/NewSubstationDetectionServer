#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include<stdio.h>  
#include <iostream> 
#include "mysql.h"
#include "afxdialogex.h"
#include "stdafx.h"

#include<vector>

using namespace std;

extern void DataBaseConnect();   //�������ݿ�
extern MYSQL_RES* selectData(CString m_sql);  //��ѯ����

extern CString getCameraIdFromCamerName(CString cameraName); //ͨ��cameraName��ȡcameraId
extern vector<CString> getEnviParamFromCameraName(char* cameraName); //ͨ��cameraName��ȡenviParam
extern CString getPositionFromCameraName(char* cameraName); //ͨ��cameraName��ȡposition

extern vector<CString> getCameraFromCameraName(char* cameraName);//ͨ��cameraName��ȡcamera

extern void saveDataToDataBase(CString cameraId,char* failureType,float MaxTemp,  float referTemp, float diffTemp,CString path,int year, int month,int day,int hour, int minu,int sec); //�������ݵ�abnormalPic

extern vector<CString>  getAbnormalDataFromTime(int Year,int Month,int Day,int Hour,int Min,int Sec); //����ʱ�����ѯ�쳣��Ϣ

extern vector<vector<CString>> getAbnormalInfoFromTime(int year,int month,int day); //����ʱ���ȡ�쳣��Ϣ����ѯ�����ȡ��Ϣ

extern vector<CString> getCameraFromCameraId(CString cameraId); //ͨ��cameraId��ȡcamera

extern CString getFailureTypeFromFailureLevel(CString failureLevel); //ͨ��failureLevel��ȡFailureType

extern CString  getFailureLevelFromFailureType(CString failureType); //ͨ��FailureType��ȡfailureLevel

extern CString  getCameraIdFromPositionAndDeviceType(CString position,CString deviceType); //ͨ��Position DeviceType��ȡCameraId

extern vector<vector<CString>> searchDlg_searchData(CString cameraId,CString failureLevel,CString year,CString month,CString day); //��ѯ��������������

extern vector<CString>  getDeviceFromDeviceType(CString deviceType); //����deviceType��ȡdevice

#endif