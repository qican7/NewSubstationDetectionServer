#include "stdafx.h"
#include "dataBaseUtils.h"
#include "Utils.h"



MYSQL *sock; //���ݿ�����ָ������
MYSQL_RES *results; //��ѯ���������
MYSQL_ROW record; //��ѯ���������
MYSQL mysql; 

//�������ݿ�
void  DataBaseConnect()
{
	mysql_init(&mysql);    
	// localhost:������ root/123456Ϊ�˺����� managesystemdbΪ���ݿ��� 3306Ϊ�˿�    
	if(!mysql_real_connect(&mysql,"localhost","root","root","substation_monitoring",3306,NULL,0))    
	{    
		AfxMessageBox(_T("�������ݿ�ʧ��!"));  
		//CString e=mysql_error(&m_sqlCon);//��Ҫ����Ŀ�������ַ����޸�Ϊ��ʹ�ö��ֽ��ַ�������δ���á�  
		//MessageBox(e);  
		return;  
	}  
	else  
	{    
		//AfxMessageBox(_T("�ɹ�!"));  
		mysql_query(&mysql,"SET NAMES 'GB2312'");//��������ݿ��ж�ȡ���ݺ���������ʾ������ 
	}
}

//��ѯ���ݣ����ز�ѯ���
MYSQL_RES* selectData(CString m_sql){

	//��CStringת��Ϊchar*
	char *my_sql=new char[200];
	WideCharToMultiByte(CP_ACP,0,m_sql.GetBuffer(),-1,my_sql,200,NULL,NULL);//Unicodeת���ɶ��ֽ��ַ���

	if(mysql_query(&mysql,my_sql))//����my_sql��Ҫ�Ƕ��ֽڵ��ַ�������
	{
		AfxMessageBox(L"��ѯʧ�ܣ�"); 
		return 0;
	}else
		results=mysql_store_result(&mysql);
	
	return results;
}

//ͨ��cmaeraName��ȡ������������
vector<CString>  getEnviParamFromCameraName(char* cameraName){

	vector<CString> enviParam;

	CString cameraId = getCameraIdFromCamerName((CString)cameraName);

	//���SQL���
	CString  SQL=L"select *from enviparam";
	SQL=SQL+L" where cameraId='"+cameraId+L"'";
	CString m_sql=SQL;


	results  = selectData(m_sql); //��ѯ����


	CString radiation,airTemp,relHumi,distance,airSpeed,airPressure; //��ʱ�洢������������
	while(record = mysql_fetch_row(results))
	{
		//user_name   CA2W�����ַ�����ASCII��ת��Ϊ���ַ�����Unicode��
		radiation = CA2W(record[1], CP_ACP);
		airTemp = CA2W(record[2], CP_ACP);
		relHumi = CA2W(record[3], CP_ACP);
		distance = CA2W(record[4], CP_ACP);
		airSpeed = CA2W(record[5], CP_ACP);
		airPressure = CA2W(record[6], CP_ACP);

		enviParam.push_back(radiation); //radiation
		enviParam.push_back(airTemp); //airTemp
		enviParam.push_back(relHumi); //relHumi
		enviParam.push_back(distance); //distance
		enviParam.push_back(airSpeed); //airSpeed
		enviParam.push_back(airPressure); //airPressure
		//AfxMessageBox(str);
	}
	return enviParam;
}

//ͨ��cameraName��ȡcameraId
CString getCameraIdFromCamerName(CString cameraName){
	//���SQL���
	CString  SQL=L"select *from camera";
	SQL=SQL+L" where cameraName='"+(CString)cameraName+L"'";
	CString m_sql=SQL;


	MYSQL_RES *results  = selectData(m_sql); //��ѯ����


	CString str;
	MYSQL_ROW record;
	while(record = mysql_fetch_row(results))
	{
		str= CA2W(record[0], CP_ACP);//user_name   CA2W�����ַ�����ASCII��ת��Ϊ���ַ�����Unicode��
		//AfxMessageBox(str);
	}
	return str;
}

//ͨ��cameraName��ȡposition
CString getPositionFromCameraName(char* cameraName){

	//���SQL���
	CString  SQL=L"select *from camera";
	SQL=SQL+L" where cameraName='"+(CString)cameraName+L"'";
	CString m_sql=SQL;


	MYSQL_RES *results  = selectData(m_sql); //��ѯ����


	CString str;
	MYSQL_ROW record;
	while(record = mysql_fetch_row(results))
	{
		str= CA2W(record[2], CP_ACP);//user_name   CA2W�����ַ�����ASCII��ת��Ϊ���ַ�����Unicode��
		//AfxMessageBox(str);
	}
	return str;
}

//ͨ��cameraName��ȡcamera
vector<CString> getCameraFromCameraName(char* cameraName){
	//���SQL���
	CString  SQL=L"select *from camera";
	SQL=SQL+L" where cameraName='"+(CString)cameraName+L"'";
	CString m_sql=SQL;


	MYSQL_RES *results  = selectData(m_sql); //��ѯ����


	CString str;
	MYSQL_ROW record;

	CString cameraId,position,deviceType;
	vector<CString> camera;
	while(record = mysql_fetch_row(results))
	{
		cameraId = CA2W(record[0], CP_ACP);
		position = CA2W(record[2], CP_ACP);
		deviceType = CA2W(record[3], CP_ACP);

		camera.push_back(cameraId);
		camera.push_back(position);
		camera.push_back(deviceType);
	}
	return camera;

}

//����ʱ�����ѯ�쳣��Ϣ
vector<CString>  getAbnormalDataFromTime(int Year,int Month,int Day,int Hour,int Min,int Sec){
	//���SQL���
	CString  SQL=L"select *from abnormalPic";
	SQL=SQL+L" where year="+intToCString(Year)+L" and month="+intToCString(Month)+L"  and day="+intToCString(Day)+L" and hour="+intToCString(Hour)+L" and minute="+intToCString(Min)+L" and second="+intToCString(Sec)+L"";
	CString m_sql=SQL;


	MYSQL_RES *results  = selectData(m_sql); //��ѯ����


	CString str;
	MYSQL_ROW record;

	CString cameraId,maxTemp,minTemp,path,failtureLevel,referTemp,enviTemp,tempDiff;
	vector<CString> abnormalData;
	while(record = mysql_fetch_row(results))
	{
		cameraId = CA2W(record[1], CP_ACP);
		maxTemp = CA2W(record[2], CP_ACP);
		minTemp = CA2W(record[3], CP_ACP);
		path = CA2W(record[4], CP_ACP);
		failtureLevel = CA2W(record[5], CP_ACP);
		referTemp = CA2W(record[6], CP_ACP);
		enviTemp = CA2W(record[7], CP_ACP);
		tempDiff = CA2W(record[8], CP_ACP);
	
		abnormalData.push_back(cameraId);
		abnormalData.push_back(maxTemp);
		abnormalData.push_back(minTemp);
		abnormalData.push_back(path);
		abnormalData.push_back(failtureLevel);
		abnormalData.push_back(referTemp);
		abnormalData.push_back(enviTemp);
		abnormalData.push_back(tempDiff);

	}
	return abnormalData;

}

//�����쳣���ݵ�abnormalPic
void saveDataToDataBase(CString cameraId,char* failureType,float MaxTemp,  float referTemp, float diffTemp,CString path,int year, int month,int day,int hour, int minu,int sec){

	CString failureLevel = getFailureLevelFromFailureType((CString)failureType);

	if (failureLevel.IsEmpty())
	{
		AfxMessageBox(L"�޷���ȡ���ϵȼ���saveDataToDataBase����");
		return;
	}

	//���SQL���

	CString  SQL=L"insert into abnormalpic(cameraId,maxTemp,imagePath_1,failtureLevel,referTemp,tempDiff,year,month,day,hour,minute,second)  values(";
	SQL=SQL+cameraId+L","+floatToCString(MaxTemp)+L",'"+path+L"',"+failureLevel+L","+floatToCString(referTemp)+L","+floatToCString(diffTemp)+L","+intToCString(year)+L","+intToCString(month)+L","+intToCString(day)+L","+intToCString(hour)+L","+intToCString(minu)+L","+intToCString(sec)+L")";
	CString m_sql=SQL;
	//AfxMessageBox(m_sql);

	//���SQL���
	/*CString  SQL=L"insert into abnormalpic(cameraId,imagePath_1,year,month,day) values(";
	SQL=SQL+cameraId+L",'"+path+L"',"+intToCString(year)+L","+intToCString(month)+L","+intToCString(day)+L")";
	CString m_sql=SQL;*/

	//CString  SQL=L"insert into test(cameraId,imagePath) values(5,'www')";
	//CString m_sql=SQL;
	char *my_sql=new char[250];
	WideCharToMultiByte(CP_ACP,0,m_sql.GetBuffer(),-1,my_sql,250,NULL,NULL);//Unicodeת���ɶ��ֽ��ַ���
	if(mysql_query(&mysql,my_sql))//����my_sql��Ҫ�Ƕ��ֽڵ��ַ�������
	{
		AfxMessageBox(L"�޸�ʧ�ܣ�");
		
	}
	else
	{
		//AfxMessageBox(L"�޸ĳɹ���");
		return;
	}

}

//����ʱ���ȡ�쳣��Ϣ����ѯ�����ȡ��Ϣ
vector<vector<CString>> getAbnormalInfoFromTime(int m_year,int m_month,int m_day){
	//���SQL���
	CString  SQL=L"select *from abnormalPic";
	if (m_day>2)
	{
		SQL=SQL+L" where year="+intToCString(m_year)+L" and month="+intToCString(m_month)+L"  and day>"+intToCString(m_day-3)+L" and day<="+intToCString(m_day)+L"";
	}else{
		SQL=SQL+L" where year="+intToCString(m_year)+L" and month="+intToCString(m_month)+L"  and day="+intToCString(m_day)+L"";
	}
	
	
	CString m_sql=SQL;


	MYSQL_RES *results  = selectData(m_sql); //��ѯ����


	CString str;
	MYSQL_ROW record;

	CString cameraId,failureLevel,tempDiff,year,month,day,hour,minute,second;
	vector<vector<CString>> abnormalDataList;
	vector<CString> abnormalData;
	while(record = mysql_fetch_row(results))
	{
		cameraId = CA2W(record[1], CP_ACP);
		failureLevel = CA2W(record[5], CP_ACP);
		tempDiff = CA2W(record[8], CP_ACP);
		year = CA2W(record[9], CP_ACP);
		month = CA2W(record[10], CP_ACP);
		day = CA2W(record[11], CP_ACP);
		hour = CA2W(record[12], CP_ACP);
		minute = CA2W(record[13], CP_ACP);
		second = CA2W(record[14], CP_ACP);

		abnormalData.push_back(cameraId);
		abnormalData.push_back(failureLevel);
		abnormalData.push_back(tempDiff);
		abnormalData.push_back(year);
		abnormalData.push_back(month);
		abnormalData.push_back(day);
		abnormalData.push_back(hour);
		abnormalData.push_back(minute);
		abnormalData.push_back(second);
		abnormalDataList.push_back(abnormalData);
		abnormalData.clear();

	}
	return abnormalDataList;

}

//ͨ��cameraId��ȡcamera
vector<CString> getCameraFromCameraId(CString cameraId){
	//���SQL���
	CString  SQL=L"select *from camera";
	SQL=SQL+L" where cameraId='"+cameraId+L"'";
	CString m_sql=SQL;


	MYSQL_RES *results  = selectData(m_sql); //��ѯ����


	CString str;
	MYSQL_ROW record;

	CString position,deviceType;
	vector<CString> camera;
	while(record = mysql_fetch_row(results))
	{
		position = CA2W(record[2], CP_ACP);
		deviceType = CA2W(record[3], CP_ACP);

		camera.push_back(position);
		camera.push_back(deviceType);
	}
	return camera;

}

//ͨ��failureLevel��ȡFailureType
CString getFailureTypeFromFailureLevel(CString failureLevel){

	//���SQL���
	CString  SQL=L"select *from devicefaitolev";
	SQL=SQL+L" where failureLevel="+failureLevel+L"";
	CString m_sql=SQL;


	MYSQL_RES *results  = selectData(m_sql); //��ѯ����


	CString failureType;
	MYSQL_ROW record;

	
	while(record = mysql_fetch_row(results))
	{
		failureType = CA2W(record[1], CP_ACP);
	}
	return failureType;
}

//ͨ��FailureType��ȡfailureLevel
CString  getFailureLevelFromFailureType(CString failureType){

	//���Ϊ���У�ֱ�ӷ���0
	if (failureType == _T("����"))
	{
		 return intToCString(0);
	}
	//���SQL���
	CString  SQL=L"select *from devicefaitolev";
	SQL=SQL+L" where type='"+(CString)failureType+L"'";
	CString m_sql=SQL;


	MYSQL_RES *results  = selectData(m_sql); //��ѯ����


	CString failureLevel;
	MYSQL_ROW record;


	while(record = mysql_fetch_row(results))
	{
		failureLevel = CA2W(record[0], CP_ACP);
	}
	return failureLevel;
}

//ͨ��Position DeviceType��ȡCameraId
 CString  getCameraIdFromPositionAndDeviceType(CString position,CString deviceType){

	 //���SQL���
	 CString  SQL=L"select *from camera";

	 if (position == _T("����") && deviceType == _T("����"))
	 {
		 return intToCString(0);
	 }else if (position == _T("����") )
	 {
		 SQL=SQL+L" where deviceType= '"+deviceType+L"'";
	 }else if (deviceType == _T("����"))
	 {
		 SQL=SQL+L" where position='"+position+L"'";
	 }else{
		 SQL=SQL+L" where position='"+position+L"' and deviceType= '"+deviceType+L"'";
	 }
	 
	 CString m_sql=SQL;
	 //AfxMessageBox(m_sql);


	 MYSQL_RES *results  = selectData(m_sql); //��ѯ����


	 CString cameraId;
	 MYSQL_ROW record;

	 while(record = mysql_fetch_row(results))
	 {
		 cameraId = CA2W(record[0], CP_ACP);
		 
	 }
	 return cameraId;
 }

 //��ѯ��������������
vector<vector<CString>> searchDlg_searchData(CString cameraId,CString failureLevel,CString year,CString month,CString day){

	//���SQL��� , 0��������
	CString  SQL=L"select *from abnormalPic";
	if (CStringToInt(cameraId)==0 && CStringToInt(failureLevel)==0)
	{
		SQL=SQL+L" where  year="+year+L" and month="+month+L"  and day="+day+L" ";
	}else if (CStringToInt(cameraId)==0)
	{
		SQL=SQL+L" where failtureLevel="+failureLevel+L" and year="+year+L" and month="+month+L"  and day="+day+L" ";
	}else if (CStringToInt(failureLevel)==0)
	{
		SQL=SQL+L" where  cameraId="+cameraId+L"  and year="+year+L" and month="+month+L"  and day="+day+L" ";
	}else{
		SQL=SQL+L" where  cameraId="+cameraId+L" and failtureLevel="+failureLevel+L" and year="+year+L" and month="+month+L"  and day="+day+L" ";
	}
	
	CString m_sql=SQL;
	//AfxMessageBox(m_sql);

	vector<vector<CString>> abnormalDataList;
	MYSQL_RES *results  = selectData(m_sql); //��ѯ����
	if (results->row_count != 0) //�����Ϊ��
	{
		CString str;
		MYSQL_ROW record;

		CString tempDiff,hour,minute,second;
		CString cameraId_1,failureLevel_1;
		vector<CString> abnormalData;
		while(record = mysql_fetch_row(results))
		{
			cameraId_1 = CA2W(record[1], CP_ACP);
			failureLevel_1 = CA2W(record[5], CP_ACP);
			tempDiff = CA2W(record[8], CP_ACP);
			//year = CA2W(record[9], CP_ACP);
			//month = CA2W(record[10], CP_ACP);
			//day = CA2W(record[11], CP_ACP);
			hour = CA2W(record[12], CP_ACP);
			minute = CA2W(record[13], CP_ACP);
			second = CA2W(record[14], CP_ACP);

			abnormalData.push_back(cameraId_1);
			abnormalData.push_back(failureLevel_1);
			abnormalData.push_back(tempDiff);
			abnormalData.push_back(year);
			abnormalData.push_back(month);
			abnormalData.push_back(day);
			abnormalData.push_back(hour);
			abnormalData.push_back(minute);
			abnormalData.push_back(second);
			abnormalDataList.push_back(abnormalData);
			abnormalData.clear();

		}
		
	}

	return abnormalDataList;

 }

//����deviceType��ȡdevice
vector<CString>  getDeviceFromDeviceType(CString deviceType){

	//���SQL���
	CString  SQL=L"select * from device";
	SQL=SQL+L" where deviceType='"+deviceType+L"'";
	CString m_sql=SQL;

	MYSQL_RES *results  = selectData(m_sql); //��ѯ����

	vector<CString> device;
	CString nomalTemp,firstRate,secondRate,thirdRate;
	MYSQL_ROW record;


	while(record = mysql_fetch_row(results))
	{
		nomalTemp = CA2W(record[2], CP_ACP);
		firstRate = CA2W(record[3], CP_ACP);
		secondRate = CA2W(record[4], CP_ACP);
		thirdRate = CA2W(record[5], CP_ACP);


		device.push_back(nomalTemp);
		device.push_back(firstRate);
		device.push_back(secondRate);
		device.push_back(thirdRate);
	}

	
	return device;
}
