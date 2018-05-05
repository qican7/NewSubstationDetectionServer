// MainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NewSubstationDetectionServer.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include "WinsockMatTransmissionServer.h"
#include "WinsockMatTransmissionServer_1.h"
#include "Utils.h"
#include "CvvImage.h"
#include "DetailsDlg.h"

WinsockMatTransmissionServer socketMat;  
WinsockMatTransmissionServer_1 socketMat_receiveVideo; 
 
char* name = new char[30];


UINT message_socket(LPVOID lpParameter);//�̺߳���������,�����쳣����
CWinThread* socketThread;	

UINT message_socket_receiveVideo(LPVOID lpParameter);//�̺߳�����������������Ƶ
CWinThread* socketThread_receiveVideo;

CString time_flag;  //ʱ����


// CMainDlg �Ի���

IMPLEMENT_DYNAMIC(CMainDlg, CDialogEx)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainDlg::IDD, pParent)
{

}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_infrare, m_infrare);
	DDX_Control(pDX, IDC_light, m_light);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_PAINT()
	ON_NOTIFY(NM_CLICK, IDC_LIST, &CMainDlg::OnNMClickList)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMainDlg ��Ϣ�������


BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	
	//list control�ؼ���ʼ��
	CRect rect;   

	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
	m_list.GetClientRect(&rect);   

	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   
 
	// Ϊ�б���ͼ�ؼ��������   
	m_list.InsertColumn(0, _T("���"), LVCFMT_CENTER, rect.Width()/24, 0);  
	m_list.InsertColumn(1, _T("�豸����"), LVCFMT_CENTER, rect.Width()/12, 1);  
	m_list.InsertColumn(2, _T("�쳣����"), LVCFMT_CENTER, rect.Width()/12, 2);   
	m_list.InsertColumn(3, _T("�쳣���"), LVCFMT_CENTER, rect.Width()/12, 3); 
	m_list.InsertColumn(4, _T("����¶�(��C)"), LVCFMT_CENTER, rect.Width()/12, 4);
	m_list.InsertColumn(5, _T("ƽ���¶�(��C)"), LVCFMT_CENTER, rect.Width()/12, 5);
	m_list.InsertColumn(6, _T("�ο��¶�(��C)"), LVCFMT_CENTER, rect.Width()/12, 6);
	m_list.InsertColumn(7, _T("�����¶�(��C)"), LVCFMT_CENTER, rect.Width()/12, 7);
	m_list.InsertColumn(8, _T("����²�(%)"), LVCFMT_CENTER, rect.Width()/12, 8);
	m_list.InsertColumn(9, _T("�����ٶ�(��C/s)"), LVCFMT_CENTER, rect.Width()/12, 9);
	m_list.InsertColumn(10, _T("ȱ�ݵȼ�"), LVCFMT_CENTER, rect.Width()/12, 10);
	m_list.InsertColumn(11, _T("���ʱ��"), LVCFMT_CENTER, rect.Width()*3/24, 11);

	

	socketThread = AfxBeginThread(message_socket,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
	socketThread_receiveVideo = AfxBeginThread(message_socket_receiveVideo,this,THREAD_PRIORITY_NORMAL,0,0,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CMainDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	//IplImage* frare =  cvLoadImage("test.jpg");
	//Mat image = imread("test.jpg");
	//IplImage image_test = matToIplImage(&image);

	//cvSaveImage("1.jpg",&image_test);
	//DrawPicToHDC(&image_test, IDC_infrare);
	//DrawPicToHDC(frare, IDC_light);
}


//�̺߳��������տͻ��˵���Ϣ
UINT message_socket(LPVOID lpParameter)//��������ΪUINT����
{

	CMainDlg *Dlg = (CMainDlg*) lpParameter;
	cv::Mat image; 
	cv::Mat image_1; 
	cv::Mat image_2;
	string deviceType;
	int x =0;
	int y =0;
	int abnormalDimension =0;
	float areaMaxTemp =0;
	float areaAvgTemp =0;
	float referTemp = 0;
	float enviTemp =0;
	float diffTemp = 0;
	float speed = 0;
	int level = 0;
    string detectTime;

	if (socketMat.socketConnect(6666) < 0)  
	{  
		return FALSE;  
	} 

	cv::Mat image_3; 
	int ad = 0;
	//if (socketMat_receiveVideo.socketConnect(6667) < 0)  
	//{  
	//	return FALSE;  
	//} 

	CString pos;
	while (1)  
    {  
        if(socketMat.receive(image,image_1,image_2,deviceType,x,y,abnormalDimension,areaMaxTemp,areaAvgTemp,referTemp,enviTemp, diffTemp,speed,level,detectTime) > 0)  
        {  
			
		  //AfxMessageBox(L"�û��������벻ƥ��"); //��ƥ���������ʾ��

          Dlg->insertIntoList(stringToCString(deviceType),x,y,abnormalDimension,areaMaxTemp,areaAvgTemp,Save2Float(referTemp),Save2Float(enviTemp), diffTemp,speed,level,stringToCString(detectTime));
		  send(socketMat.sockConn, "qican", strlen("qican")+1, 0) ;	
        }  

    }  

	return 0;
}

//�̺߳�����������Ƶ
UINT message_socket_receiveVideo(LPVOID lpParameter)//��������ΪUINT����
{

	CMainDlg *Dlg = (CMainDlg*) lpParameter;
	Mat image_infrare; 
	Mat image_light; 
	if (socketMat_receiveVideo.socketConnect(6667) < 0)  
	{  
		return FALSE;  
	} 

	while (1)  
	{  
		if(socketMat_receiveVideo.receive(image_infrare,image_light) > 0)  
		{  

			//AfxMessageBox(L"�û��������벻ƥ��"); //��ƥ���������ʾ��
			//AfxMessageBox(intToCString(x)+_T(" ")+intToCString(year)); //��ƥ���������ʾ��
			Dlg->DrawPicToHDC(&matToIplImage(&image_infrare), IDC_infrare);
			Dlg->DrawPicToHDC(&matToIplImage(&image_light), IDC_light);
		}  
	}  
	return 0;
}

//���쳣��Ϣ���뵽�б���
void CMainDlg::insertIntoList(CString deviceType,int x,int y,int AbnormalDimension,float areaMaxTemp,float areaAvgTemp,CString referTemp,CString enviTemp, float referDiffTemp,float speed,int abnormalLevel,CString time){

	int nIndex = m_list.GetItemCount() + 1; //����������ţ���1��ʼ
	CString t;
	t.Format(_T("%d"), nIndex);
	//LV_ITEM lvItem;
	//lvItem.mask = LVIF_TEXT;
	//lvItem.iItem = nIndex;     //����
	//lvItem.iSubItem = 0;
	//lvItem.pszText = (LPWSTR)(LPCWSTR)t;
	//m_video_list.InsertItem(&lvItem);
	////����������


	CString abnormalCenter = _T("(") + intToCString(x) + _T(",") + intToCString(y) + _T(")");
	CString abnormalType;

	if (abnormalLevel == 0)
	{
		abnormalType = _T("һ��ȱ��");
	}else if (abnormalLevel == 1)
	{
		abnormalType = _T("����ȱ��");
	}else if (abnormalLevel == 2)
	{
		abnormalType = _T("��ͬ����ȱ��");
	}

	//�����б�����
	m_list.InsertItem(0,(LPCTSTR)t);
	m_list.SetItemText(0, 1, deviceType);
	m_list.SetItemText(0, 2, abnormalCenter);
	m_list.SetItemText(0, 3,intToCString(AbnormalDimension));
	m_list.SetItemText(0, 4, Save2Float(areaMaxTemp)); 
	m_list.SetItemText(0, 5, Save2Float(areaAvgTemp)); 
	m_list.SetItemText(0, 6, referTemp); 
	m_list.SetItemText(0, 7, enviTemp); 
	m_list.SetItemText(0, 8, Save2Float(referDiffTemp)); 
	m_list.SetItemText(0, 9, Save2Float(speed)); 
	m_list.SetItemText(0, 10, abnormalType); 
	m_list.SetItemText(0, 11, time); 



}

//����Ƶ֡��ʾ��pictureControl
void CMainDlg::DrawPicToHDC(IplImage *img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC(); 
	HDC hDC= pDC->GetSafeHdc(); 
	CRect rect; 
	GetDlgItem(ID)->GetClientRect(&rect); 
	CvvImage cimg; 

	cimg.CopyOf(img); 
	cimg.DrawToHDC(hDC,&rect); 

	ReleaseDC(pDC); 
}



void CMainDlg::OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	CString number;    //ѡ�еı��
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;   

	if (-1 != pNMListView->iItem)        // ���iItem����-1����˵�����б��ѡ��   
	{   
		// ��ȡ��ѡ���б����һ��������ı�   
		number = m_list.GetItemText(pNMListView->iItem, 0);   //��ȡ���

		//time_flag = m_list.GetItemText(pNMListView->iItem, 10);

		CDetailsDlg detailsDlg;
		detailsDlg.data.deviceType = m_list.GetItemText(pNMListView->iItem, 1);
		detailsDlg.data.position = m_list.GetItemText(pNMListView->iItem, 2);
		detailsDlg.data.abnormalDimension = m_list.GetItemText(pNMListView->iItem, 3);
		detailsDlg.data.areaMaxTemp = m_list.GetItemText(pNMListView->iItem, 4);
		detailsDlg.data.areaAvgTemp = m_list.GetItemText(pNMListView->iItem, 5);
		detailsDlg.data.enviTemperature = m_list.GetItemText(pNMListView->iItem, 6);
		detailsDlg.data.diffTemp = m_list.GetItemText(pNMListView->iItem, 7);
		detailsDlg.data.speed = m_list.GetItemText(pNMListView->iItem, 8);
		detailsDlg.data.abnormalType = m_list.GetItemText(pNMListView->iItem, 9);
		detailsDlg.data.Time = m_list.GetItemText(pNMListView->iItem, 10);
		detailsDlg.DoModal();

		//AfxMessageBox(time_flag);

	}   
}


//void CMainDlg::OnClose()
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	DestroyWindow();
//	CDialogEx::OnClose();
//	
//}


void CMainDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	PostQuitMessage(0);

	// TODO: �ڴ˴������Ϣ����������
}
