
// NewSubstationDetectionServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NewSubstationDetectionServer.h"
#include "NewSubstationDetectionServerDlg.h"
#include "afxdialogex.h"
#include "WinsockMatTransmissionServer.h"

#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

WinsockMatTransmissionServer socketMat_1;


UINT receiveThread_1(LPVOID lpParameter);//�̺߳���������

CWinThread* Thread_1;		//��������Ƶ�߳�


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNewSubstationDetectionServerDlg �Ի���




CNewSubstationDetectionServerDlg::CNewSubstationDetectionServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewSubstationDetectionServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNewSubstationDetectionServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_deviceName, m_deviceName);
	DDX_Control(pDX, IDC_failureType, m_failureLevel);
	DDX_Control(pDX, IDC_position, m_position);
	DDX_Control(pDX, IDC_startYear, m_startYear);
	DDX_Control(pDX, IDC_startMonth, m_startMonth);
	DDX_Control(pDX, IDC_startDay, m_startDay);
	DDX_Control(pDX, IDC_endYear, m_endYear);
	DDX_Control(pDX, IDC_endMonth, m_endMonth);
	DDX_Control(pDX, IDC_endDay, m_endDay);
}

BEGIN_MESSAGE_MAP(CNewSubstationDetectionServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CNewSubstationDetectionServerDlg ��Ϣ�������

BOOL CNewSubstationDetectionServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������



	//list control�ؼ���ʼ��
	CRect rect;   

	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
	m_list.GetClientRect(&rect);   

	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   

	// Ϊ�б���ͼ�ؼ��������   
	m_list.InsertColumn(0, _T("���"), LVCFMT_CENTER, rect.Width()/22, 0);  
	m_list.InsertColumn(1, _T("�豸����"), LVCFMT_CENTER, rect.Width()/11, 1);  
	m_list.InsertColumn(2, _T("�쳣����"), LVCFMT_CENTER, rect.Width()/11, 2);   
	m_list.InsertColumn(3, _T("�쳣���"), LVCFMT_CENTER, rect.Width()/11, 3); 
	m_list.InsertColumn(4, _T("����¶�(��C)"), LVCFMT_CENTER, rect.Width()/11, 4);
	m_list.InsertColumn(5, _T("ƽ���¶�(��C)"), LVCFMT_CENTER, rect.Width()/11, 5);
	m_list.InsertColumn(6, _T("�ο��¶�(��C)"), LVCFMT_CENTER, rect.Width()/11, 6);
	m_list.InsertColumn(7, _T("����²�(%)"), LVCFMT_CENTER, rect.Width()/11, 7);
	m_list.InsertColumn(8, _T("�����ٶ�(��C/s)"), LVCFMT_CENTER, rect.Width()/11, 8);
	m_list.InsertColumn(9, _T("ȱ�ݵȼ�"), LVCFMT_CENTER, rect.Width()/11, 9);
	m_list.InsertColumn(10, _T("���ʱ��"), LVCFMT_CENTER, rect.Width()*3/22, 9);




	CTime tm = CTime::GetCurrentTime();
	//��ȡʱ�䣬�������ݿ�
	int m_nYear = tm.GetYear(); ///��
	int m_nMonth = tm.GetMonth(); ///��  
	int m_nDay = tm.GetDay(); ///��
	int m_nHour = tm.GetHour(); //ʱ
	int m_nMin = tm.GetMinute(); //��
	int m_nSec = tm.GetSecond(); //��

	//λ�����ѡ��
	m_position.InsertString(0, _T("����"));
	m_position.InsertString(1, _T("���1վ"));
	m_position.InsertString(2, _T("���2վ"));
	m_position.SetCurSel(0);

	//�豸�������ѡ��
	m_deviceName.InsertString(0, _T("����"));
	m_deviceName.InsertString(1, _T("������"));
	m_deviceName.InsertString(2, _T("�����׹�"));
	m_deviceName.InsertString(3, _T("��ѹ���ع�"));
	m_deviceName.InsertString(4, _T("������·��"));
	// Ĭ��ѡ���һ��   
	m_deviceName.SetCurSel(0);   

	//�����������ѡ��
	m_failureLevel.InsertString(0, _T("����"));
	m_failureLevel.InsertString(1, _T("һ��ȱ��"));
	m_failureLevel.InsertString(2, _T("�ش�ȱ��"));
	m_failureLevel.InsertString(3, _T("����ȱ��"));
	// Ĭ��ѡ���һ��   
	m_failureLevel.SetCurSel(0); 

	//ʱ����ʼ��
	//��ʼʱ���ʼ��
	m_startYear.InsertString(0,intToCString(m_nYear));
	m_startYear.InsertString(1,intToCString(m_nYear-1));
	m_startYear.InsertString(2,intToCString(m_nYear-2));
	m_startYear.InsertString(3,intToCString(m_nYear-3));
	m_startYear.InsertString(4,intToCString(m_nYear-4));
	m_startYear.SetCurSel(0);

	for (int i=1;i<=12;i++)
	{
		m_startMonth.InsertString(i-1,intToCString(i));
	}

	m_startMonth.SetCurSel(m_nMonth-1);  //Ĭ��ѡ����

	for (int i=1;i<=31;i++)
	{
		m_startDay.InsertString(i-1,intToCString(i));
	}

	m_startDay.SetCurSel(m_nDay-1);

	//����ʱ���ʼ��
	m_endYear.InsertString(0,intToCString(m_nYear));
	m_endYear.InsertString(1,intToCString(m_nYear-1));
	m_endYear.InsertString(2,intToCString(m_nYear-2));
	m_endYear.InsertString(3,intToCString(m_nYear-3));
	m_endYear.InsertString(4,intToCString(m_nYear-4));
	m_endYear.SetCurSel(0);

	for (int i=1;i<=12;i++)
	{
		m_endMonth.InsertString(i-1,intToCString(i));
	}

	m_endMonth.SetCurSel(m_nMonth-1);  //Ĭ��ѡ����

	for (int i=1;i<=31;i++)
	{
		m_endDay.InsertString(i-1,intToCString(i));
	}

	m_endDay.SetCurSel(m_nDay-1);


	////�����߳�
	//Thread_1 = AfxBeginThread(receiveThread_1,this,THREAD_PRIORITY_NORMAL,0,0,NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CNewSubstationDetectionServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CNewSubstationDetectionServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CNewSubstationDetectionServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//�̺߳�����������Ƶ
UINT receiveThread_1(LPVOID lpParameter)//��������ΪUINT����
{
	//��ͻ��˽�������
	if (socketMat_1.socketConnect(6666) < 0)  
	{  
		return 0;  
	}  

	CNewSubstationDetectionServerDlg *Dlg = (CNewSubstationDetectionServerDlg*) lpParameter;
	char* name = new char[30];
	cv::Mat image; 
	cv::Mat image_1; 
	cv::Mat image_2;
	int position;
	string deviceType;
	int x = 0;
	int y = 0;
	int abnormalDimension = 0;
	float areaMinTemp = 0;
	float areaMaxTemp = 0;
	float areaAvgTemp = 0;
	float referTemp = 0;
	float diffTemp = 0;
	float speed = 0;
	int level = 0;
	int year = 0;
	int month = 0;
	int day = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;
        while (1)  
        {  
             if(socketMat_1.receive(image,image_1,image_2,position,deviceType,x,y,abnormalDimension,areaMinTemp,areaMaxTemp,areaAvgTemp,referTemp,diffTemp,speed,level,year,month,day,hour,minute,second) > 0)   
            {  

                cv::imshow("image",image);  
				cv::imshow("image_1",image_1);
                //cv::waitKey(30); 
				imwrite("image.jpg",image);
				imwrite("image_1.jpg",image_1);
				
				
				//std::cout<<name<<" "<<temp<<" "<<positionTemp<<std::endl; 
				/*picCount++;
				std::cout<<picCount<<std::endl; */
            }  
        }  
      
		socketMat_1.socketDisconnect(); 

	
	return 0;
}

