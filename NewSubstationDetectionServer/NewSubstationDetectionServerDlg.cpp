
// NewSubstationDetectionServerDlg.cpp : 实现文件
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


UINT receiveThread_1(LPVOID lpParameter);//线程函数的声明

CWinThread* Thread_1;		//读红外视频线程


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CNewSubstationDetectionServerDlg 对话框




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


// CNewSubstationDetectionServerDlg 消息处理程序

BOOL CNewSubstationDetectionServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码



	//list control控件初始化
	CRect rect;   

	// 获取编程语言列表视图控件的位置和大小   
	m_list.GetClientRect(&rect);   

	// 为列表视图控件添加全行选中和栅格风格   
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   

	// 为列表视图控件添加三列   
	m_list.InsertColumn(0, _T("编号"), LVCFMT_CENTER, rect.Width()/22, 0);  
	m_list.InsertColumn(1, _T("设备类型"), LVCFMT_CENTER, rect.Width()/11, 1);  
	m_list.InsertColumn(2, _T("异常中心"), LVCFMT_CENTER, rect.Width()/11, 2);   
	m_list.InsertColumn(3, _T("异常面积"), LVCFMT_CENTER, rect.Width()/11, 3); 
	m_list.InsertColumn(4, _T("最高温度(°C)"), LVCFMT_CENTER, rect.Width()/11, 4);
	m_list.InsertColumn(5, _T("平均温度(°C)"), LVCFMT_CENTER, rect.Width()/11, 5);
	m_list.InsertColumn(6, _T("参考温度(°C)"), LVCFMT_CENTER, rect.Width()/11, 6);
	m_list.InsertColumn(7, _T("相对温差(%)"), LVCFMT_CENTER, rect.Width()/11, 7);
	m_list.InsertColumn(8, _T("温升速度(°C/s)"), LVCFMT_CENTER, rect.Width()/11, 8);
	m_list.InsertColumn(9, _T("缺陷等级"), LVCFMT_CENTER, rect.Width()/11, 9);
	m_list.InsertColumn(10, _T("监测时间"), LVCFMT_CENTER, rect.Width()*3/22, 9);




	CTime tm = CTime::GetCurrentTime();
	//获取时间，存入数据库
	int m_nYear = tm.GetYear(); ///年
	int m_nMonth = tm.GetMonth(); ///月  
	int m_nDay = tm.GetDay(); ///日
	int m_nHour = tm.GetHour(); //时
	int m_nMin = tm.GetMinute(); //分
	int m_nSec = tm.GetSecond(); //秒

	//位置添加选项
	m_position.InsertString(0, _T("所有"));
	m_position.InsertString(1, _T("变电1站"));
	m_position.InsertString(2, _T("变电2站"));
	m_position.SetCurSel(0);

	//设备类型添加选项
	m_deviceName.InsertString(0, _T("所有"));
	m_deviceName.InsertString(1, _T("连接器"));
	m_deviceName.InsertString(2, _T("充油套管"));
	m_deviceName.InsertString(3, _T("高压开关柜"));
	m_deviceName.InsertString(4, _T("空气断路器"));
	// 默认选择第一项   
	m_deviceName.SetCurSel(0);   

	//故障类型添加选项
	m_failureLevel.InsertString(0, _T("所有"));
	m_failureLevel.InsertString(1, _T("一般缺陷"));
	m_failureLevel.InsertString(2, _T("重大缺陷"));
	m_failureLevel.InsertString(3, _T("紧急缺陷"));
	// 默认选择第一项   
	m_failureLevel.SetCurSel(0); 

	//时间框初始化
	//开始时间初始化
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

	m_startMonth.SetCurSel(m_nMonth-1);  //默认选择本月

	for (int i=1;i<=31;i++)
	{
		m_startDay.InsertString(i-1,intToCString(i));
	}

	m_startDay.SetCurSel(m_nDay-1);

	//结束时间初始化
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

	m_endMonth.SetCurSel(m_nMonth-1);  //默认选择本月

	for (int i=1;i<=31;i++)
	{
		m_endDay.InsertString(i-1,intToCString(i));
	}

	m_endDay.SetCurSel(m_nDay-1);


	////创建线程
	//Thread_1 = AfxBeginThread(receiveThread_1,this,THREAD_PRIORITY_NORMAL,0,0,NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNewSubstationDetectionServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNewSubstationDetectionServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//线程函数，播放视频
UINT receiveThread_1(LPVOID lpParameter)//必须声明为UINT类型
{
	//与客户端建立连接
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

