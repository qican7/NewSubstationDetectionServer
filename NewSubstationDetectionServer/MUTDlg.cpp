// MUTDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "PowerSystem.h"
#include "MUTDlg.h"
#include "Utils.h"

CvvImage MutCvvImage;
CWnd *pwnd_mut;
CRect rect_img1,rect_img2,rect_img3;
CRect rect_bar1,rect_bar2,rect_bar3;
HDC hDC_img1,hDC_img2,hDC_img3;
HDC hDC_bar1,hDC_bar2,hDC_bar3;
CDC *pDC_mut;

CToolTipCtrl muttooltip1;
CToolTipCtrl muttooltip2;
CToolTipCtrl muttooltip3;

double Para[6][3] = {{1,1,1},{1,1,1},{1,1,1},{1,1,1},{1,1,1},{1,1,1}};
IplImage* NumTemplMR[10];
IplImage* NumTemplMK[10];
IplImage *ImgRS1;
IplImage *ImgRS2;
IplImage *ImgRS3;
int MMHW1[4];
int MMHW2[4];
int MMHW3[4];
double MinT1;
double MinT2;
double MinT3;
double MaxT1;
double MaxT2;
double MaxT3;

double ColorBar1[256][3];
double ColorBar2[256][3];
double ColorBar3[256][3];

IplImage* MUTIMG1;
IplImage* MUTIMG2;
IplImage* MUTIMG3;
IplImage* TMPIMG1;
IplImage* TMPIMG2;
IplImage* TMPIMG3;
IplImage* ScaleImg1;
IplImage* ScaleImg2;
IplImage* ScaleImg3;
IplImage* GrayImg1;
IplImage* GrayImg2;
IplImage* GrayImg3;
CString SMinT1;
CString SMaxT1;
CString SMinT2;
CString SMaxT2;
CString SMinT3;
CString SMaxT3;

int ImgType1;
int ImgType2;
int ImgType3;
int NumFlag1;
int NumFlag2;
int NumFlag3;

CWinThread* m_pThread_OpenIRImage1,*m_pThread_OpenIRImage2,*m_pThread_OpenIRImage3;
HANDLE hEvent_OpenIRImage1,hEvent_OpenIRImage2,hEvent_OpenIRImage3;
BOOL ThreadIRImageOpenFlag1 = FALSE,ThreadIRImageOpenFlag2 = FALSE,ThreadIRImageOpenFlag3 = FALSE;
BOOL ThreadIRImageRecogFinishFlag1 = FALSE,ThreadIRImageRecogFinishFlag2 = FALSE,ThreadIRImageRecogFinishFlag3 = FALSE;
//CProgressCtrl m_IR_Progress1;
BOOL DialogCloseFlag = FALSE;
UINT ThreadProc_OpenIRImage1(LPVOID pThreadParam)
{
	CMUTDlg* pDlg = (CMUTDlg*)pThreadParam;
	ThreadIRImageRecogFinishFlag1 = FALSE;
	while( ThreadIRImageOpenFlag1 )
	{
		WaitForSingleObject(hEvent_OpenIRImage1,INFINITE);

		pDlg->ClimgCvtTpimg(ImgRS1,GrayImg1,TMPIMG1,MaxT1,MinT1,ColorBar1,256);

		//cvReleaseImage(&GrayImg1);
		ThreadIRImageOpenFlag1 = FALSE;
		ThreadIRImageRecogFinishFlag1 = TRUE;
		SetEvent(hEvent_OpenIRImage1);	
		//Sleep(1);
	}
	return 0;
}
UINT ThreadProc_OpenIRImage2(LPVOID pThreadParam)
{
	CMUTDlg* pDlg = (CMUTDlg*)pThreadParam;
	ThreadIRImageRecogFinishFlag2 = FALSE;
	while( ThreadIRImageOpenFlag2 )
	{
		WaitForSingleObject(hEvent_OpenIRImage2,INFINITE);

		pDlg->ClimgCvtTpimg(ImgRS2,GrayImg2,TMPIMG2,MaxT2,MinT2,ColorBar2,256);

		//cvReleaseImage(&GrayImg2);
		ThreadIRImageOpenFlag2 = FALSE;
		ThreadIRImageRecogFinishFlag2 = TRUE;
		SetEvent(hEvent_OpenIRImage2);	
		//Sleep(1);
	}
	return 0;
}
UINT ThreadProc_OpenIRImage3(LPVOID pThreadParam)
{
	CMUTDlg* pDlg = (CMUTDlg*)pThreadParam;
	ThreadIRImageRecogFinishFlag3 = FALSE;
	while( ThreadIRImageOpenFlag3 )
	{
		WaitForSingleObject(hEvent_OpenIRImage1,INFINITE);

		pDlg->ClimgCvtTpimg(ImgRS3,GrayImg3,TMPIMG3,MaxT3,MinT3,ColorBar3,256);

		//cvReleaseImage(&GrayImg3);
		ThreadIRImageOpenFlag3 = FALSE;
		ThreadIRImageRecogFinishFlag3 = TRUE;
		SetEvent(hEvent_OpenIRImage3);	
		//Sleep(1);
	}
	return 0;
}
// CMUTDlg 对话框

IMPLEMENT_DYNAMIC(CMUTDlg, CDialog)

CMUTDlg::CMUTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMUTDlg::IDD, pParent)
{

}

CMUTDlg::~CMUTDlg()
{
}

void CMUTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTMUTINFO, MTListCtrl);
	//DDX_Control(pDX, IDC_IR_PROGRESS1, m_IR_Progress1);
}


BEGIN_MESSAGE_MAP(CMUTDlg, CDialog)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	//ON_BN_CLICKED(IDC_LOADIMG1, &CMUTDlg::OnBnClickedLoadimg1)
	//ON_BN_CLICKED(IDC_LOADIMG2, &CMUTDlg::OnBnClickedLoadimg2)
	//ON_BN_CLICKED(IDC_LOADIMG3, &CMUTDlg::OnBnClickedLoadimg3)
	ON_BN_CLICKED(IDC_LOADIMG1, &CMUTDlg::OnBnClickedLoadimg1)
	ON_BN_CLICKED(IDC_LOADIMG2, &CMUTDlg::OnBnClickedLoadimg2)
	ON_BN_CLICKED(IDC_LOADIMG3, &CMUTDlg::OnBnClickedLoadimg3)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CMUTDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CONFORM1, &CMUTDlg::OnBnClickedConform1)
	ON_BN_CLICKED(IDC_CONFORM2, &CMUTDlg::OnBnClickedConform2)
	ON_BN_CLICKED(IDC_CONFORM3, &CMUTDlg::OnBnClickedConform3)
END_MESSAGE_MAP()


// CMUTDlg 消息处理程序

void CMUTDlg::PreInitDialog()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::PreInitDialog();
}

BOOL CMUTDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CDialog::OnInitDialog();
	CvRect MutiMain = {0,0,1480,650};
	::SetWindowPos(this->m_hWnd,HWND_BOTTOM,MutiMain.x,MutiMain.y,MutiMain.width,MutiMain.height,SWP_NOZORDER);
	CenterWindow();

	MUT_Font_Caption.CreatePointFont(100,_T("微软雅黑"),NULL);
	MUT_Font_Button.CreatePointFont(110,_T("微软雅黑"),NULL);


	GetDlgItem(IDC_IMG1)->MoveWindow(20,20,400,300);
	GetDlgItem(IDC_FLUKEBAR_FRAME1)->MoveWindow(420,20,60,300);
	GetDlgItem(IDC_IMG2)->MoveWindow(490,20,400,300);
	GetDlgItem(IDC_FLUKEBAR_FRAME2)->MoveWindow(890,20,60,300);
	GetDlgItem(IDC_IMG3)->MoveWindow(960,20,400,300);
	GetDlgItem(IDC_FLUKEBAR_FRAME3)->MoveWindow(1360,20,60,300);
	GetDlgItem(IDC_FLUKEBAR_FRAME1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FLUKEBAR_FRAME2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FLUKEBAR_FRAME3)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_PIC1)->MoveWindow(180,320,80,25);
	GetDlgItem(IDC_PIC1)->SetFont(&MUT_Font_Caption);
	GetDlgItem(IDC_PIC2)->MoveWindow(670,320,80,25);
	GetDlgItem(IDC_PIC2)->SetFont(&MUT_Font_Caption);
	GetDlgItem(IDC_PIC3)->MoveWindow(1140,320,80,25);
	GetDlgItem(IDC_PIC3)->SetFont(&MUT_Font_Caption);

	GetDlgItem(IDC_OPERATE)->MoveWindow(20,340,140,230);
	GetDlgItem(IDC_OPERATE)->SetFont(&MUT_Font_Caption);
	GetDlgItem(IDC_LOADIMG1)->MoveWindow(40,360,100,40);
	GetDlgItem(IDC_LOADIMG1)->SetFont(&MUT_Font_Button);
	GetDlgItem(IDC_LOADIMG2)->MoveWindow(40,410,100,40);
	GetDlgItem(IDC_LOADIMG2)->SetFont(&MUT_Font_Button);
	GetDlgItem(IDC_LOADIMG3)->MoveWindow(40,460,100,40);
	GetDlgItem(IDC_LOADIMG3)->SetFont(&MUT_Font_Button);
	GetDlgItem(IDC_BUTTON1)->MoveWindow(40,510,100,40);
	GetDlgItem(IDC_BUTTON1)->SetFont(&MUT_Font_Button);


	GetDlgItem(IDC_INFOR)->MoveWindow(240,340,430,230);
	GetDlgItem(IDC_INFOR)->SetFont(&MUT_Font_Caption);
	GetDlgItem(IDC_TU1)->MoveWindow(250,380,60,30);
	GetDlgItem(IDC_TU1)->SetFont(&MUT_Font_Caption);
	GetDlgItem(IDC_TU2)->MoveWindow(250,430,60,30);
	GetDlgItem(IDC_TU2)->SetFont(&MUT_Font_Caption);
	GetDlgItem(IDC_TU3)->MoveWindow(250,480,60,30);
	GetDlgItem(IDC_TU3)->SetFont(&MUT_Font_Caption);
	GetDlgItem(IDC_MAX1)->MoveWindow(325,380,70,30);
	GetDlgItem(IDC_MAX1)->SetFont(&MUT_Font_Caption);
	GetDlgItem(IDC_MAX2)->MoveWindow(325,430,70,30);
	GetDlgItem(IDC_MAX2)->SetFont(&MUT_Font_Caption);
	GetDlgItem(IDC_MAX3)->MoveWindow(325,480,70,30);
	GetDlgItem(IDC_MAX3)->SetFont(&MUT_Font_Caption);
	GetDlgItem(IDC_MAXT1)->MoveWindow(400,380,80,30);
	GetDlgItem(IDC_MAXT1)->SetFont(&MUT_Font_Caption);
	GetDlgItem(IDC_MAXT2)->MoveWindow(400,430,80,30);
	GetDlgItem(IDC_MAXT2)->SetFont(&MUT_Font_Caption);
	GetDlgItem(IDC_MAXT3)->MoveWindow(400,480,80,30);
	GetDlgItem(IDC_MAXT3)->SetFont(&MUT_Font_Caption);
	GetDlgItem(IDC_MIN1)->MoveWindow(495,380,70,30);
	GetDlgItem(IDC_MIN1)->SetFont(&MUT_Font_Caption);
	GetDlgItem(IDC_MIN2)->MoveWindow(495,430,70,30);
	GetDlgItem(IDC_MIN2)->SetFont(&MUT_Font_Caption);
	GetDlgItem(IDC_MIN3)->MoveWindow(495,480,70,30);
	GetDlgItem(IDC_MIN3)->SetFont(&MUT_Font_Caption);
	GetDlgItem(IDC_MINT1)->MoveWindow(570,380,80,30);
	GetDlgItem(IDC_MINT1)->SetFont(&MUT_Font_Caption);
	GetDlgItem(IDC_MINT2)->MoveWindow(570,430,80,30);
	GetDlgItem(IDC_MINT2)->SetFont(&MUT_Font_Caption);
	GetDlgItem(IDC_MINT3)->MoveWindow(570,480,80,30);
	GetDlgItem(IDC_MINT3)->SetFont(&MUT_Font_Caption);
	GetDlgItem(IDC_CONFORM1)->MoveWindow(250,520,60,40);
	GetDlgItem(IDC_CONFORM1)->SetFont(&MUT_Font_Button);
	GetDlgItem(IDC_CONFORM2)->MoveWindow(375,520,60,40);
	GetDlgItem(IDC_CONFORM2)->SetFont(&MUT_Font_Button);
	GetDlgItem(IDC_CONFORM3)->MoveWindow(500,520,60,40);
	GetDlgItem(IDC_CONFORM3)->SetFont(&MUT_Font_Button);
	GetDlgItem(IDC_LISTMUTINFO)->MoveWindow(690,360,240,180);
	GetDlgItem(IDC_LISTMUTINFO)->SetFont(&MUT_Font_Caption);
	
	GetDlgItem(IDC_IR_PRO_STATIC1)->MoveWindow(1000,555,50,30);
	GetDlgItem(IDC_IR_PROGRESS1)->MoveWindow(1040,550,220,30);

	pwnd_mut = GetDlgItem(IDC_IMG1);
	pDC_mut = pwnd_mut->GetDC();
	hDC_img1 = pDC_mut->GetSafeHdc();
	pwnd_mut->GetClientRect(&rect_img1);

	pwnd_mut = GetDlgItem(IDC_IMG2);
	pDC_mut = pwnd_mut->GetDC();
	hDC_img2 = pDC_mut->GetSafeHdc();
	pwnd_mut->GetClientRect(&rect_img2);

	pwnd_mut = GetDlgItem(IDC_IMG3);
	pDC_mut = pwnd_mut->GetDC();
	hDC_img3 = pDC_mut->GetSafeHdc();
	pwnd_mut->GetClientRect(&rect_img3);

	pwnd_mut = GetDlgItem(IDC_FLUKEBAR_FRAME1);
	pDC_mut = pwnd_mut->GetDC();
	hDC_bar1 = pDC_mut->GetSafeHdc();
	pwnd_mut->GetClientRect(&rect_bar1);

	pwnd_mut = GetDlgItem(IDC_FLUKEBAR_FRAME2);
	pDC_mut = pwnd_mut->GetDC();
	hDC_bar2 = pDC_mut->GetSafeHdc();
	pwnd_mut->GetClientRect(&rect_bar2);

	pwnd_mut = GetDlgItem(IDC_FLUKEBAR_FRAME3);
	pDC_mut = pwnd_mut->GetDC();
	hDC_bar3 = pDC_mut->GetSafeHdc();
	pwnd_mut->GetClientRect(&rect_bar3);

	MTListCtrl.SetExtendedStyle(MTListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	MTListCtrl.InsertColumn(0, _T("编号"), LVCFMT_CENTER, 55);
	MTListCtrl.InsertColumn(1, _T("位置坐标"), LVCFMT_CENTER, 110);
	MTListCtrl.InsertColumn(2, _T("温度(℃)"), LVCFMT_CENTER, 80);

	SMinT1=_T("");SMaxT1=_T("");
	SMinT2=_T("");SMaxT2=_T("");
	SMinT3=_T("");SMaxT3=_T("");

	muttooltip1.Create(this);
	muttooltip1.AddTool(this,_T(""));
	muttooltip1.SetTipTextColor(RGB(0,0,0));
	muttooltip1.SetDelayTime(100);
	muttooltip1.Activate(true);

	muttooltip2.Create(this);
	muttooltip2.AddTool(this,_T(""));
	muttooltip2.SetTipTextColor(RGB(0,0,0));
	muttooltip2.SetDelayTime(100);
	muttooltip2.Activate(true);

	muttooltip3.Create(this);
	muttooltip3.AddTool(this,_T(""));
	muttooltip3.SetTipTextColor(RGB(0,0,0));
	muttooltip3.SetDelayTime(100);
	muttooltip3.Activate(true);
	//m_IR_Progress1.SetRange(1,100);
	((CProgressCtrl*)GetDlgItem(IDC_IR_PROGRESS1))->SetRange(1,100);

	CString strCurPath;
	CString strImageNameR;
	CString strImageNameK;
	::GetCurrentDirectory(MAX_PATH,strCurPath.GetBuffer(MAX_PATH)); 
	strCurPath.ReleaseBuffer();
	strCurPath.Format(_T("%s"),strCurPath);
	//strImageName = strCurPath + "\\res\\line.bmp";

	CString nnum= _T("");
	for (int i=0; i<10; i++)
	{
		nnum.Format(_T("%d"),i);
		nnum = nnum+_T(".bmp");
		strImageNameR = strCurPath + _T("\\res\\FLIR\\") + nnum;
		strImageNameK = strCurPath + _T("\\res\\FLUKE\\") + nnum;
		NumTemplMR[i] = cvLoadImage(CStringToChar(strImageNameR),-1);
		NumTemplMK[i] = cvLoadImage(CStringToChar(strImageNameK),-1);
		cvNormalize(NumTemplMR[i],NumTemplMR[i],1,0,CV_MINMAX);
		cvNormalize(NumTemplMK[i],NumTemplMK[i],1,0,CV_MINMAX);
	}

	DialogCloseFlag = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CMUTDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (MUTIMG1 != NULL)
	{
		if (ImgType1 == 1)
		{
			if (MousePtM.x >= 20 && MousePtM.x < 420 && MousePtM.y >= 20 && MousePtM.y < 320)
			{
				muttooltip1.RelayEvent(pMsg);
			}
			MutCvvImage.CopyOf(MUTIMG1,1);
			MutCvvImage.DrawToHDC(hDC_img1,&rect_img1);
		}
		else if (ImgType1 == 2)
		{
			if (MousePtM.x >= 20 && MousePtM.x < 420 && MousePtM.y >= 20 && MousePtM.y < 320)
			{
				muttooltip1.RelayEvent(pMsg);
			}
			MutCvvImage.CopyOf(ImgRS1,1);
			MutCvvImage.DrawToHDC(hDC_img1,&rect_img1);
		}
	}
	if (MUTIMG2 != NULL)
	{
		if (ImgType2 == 1)
		{
			if (MousePtM.x >= 490 && MousePtM.x < 890 && MousePtM.y >= 20 && MousePtM.y < 320)
			{
				muttooltip2.RelayEvent(pMsg);
			}
			MutCvvImage.CopyOf(MUTIMG2,1);
			MutCvvImage.DrawToHDC(hDC_img2,&rect_img2);
		}
		else if (ImgType2 == 2)
		{
			if (MousePtM.x >= 490 && MousePtM.x < 890 && MousePtM.y >= 20 && MousePtM.y < 320)
			{
				muttooltip2.RelayEvent(pMsg);
			}
			MutCvvImage.CopyOf(ImgRS2,1);
			MutCvvImage.DrawToHDC(hDC_img2,&rect_img2);
		}
	}
	if (MUTIMG3 != NULL)
	{
		if (ImgType3 == 1)
		{
			if (MousePtM.x >= 960 && MousePtM.x < 1360 && MousePtM.y >= 20 && MousePtM.y < 320)
			{
				muttooltip3.RelayEvent(pMsg);
			}
			MutCvvImage.CopyOf(MUTIMG3,1);
			MutCvvImage.DrawToHDC(hDC_img3,&rect_img3);
		}
		else if (ImgType3 == 2)
		{
			if (MousePtM.x >= 960 && MousePtM.x < 1360 && MousePtM.y >= 20 && MousePtM.y < 320)
			{
				muttooltip3.RelayEvent(pMsg);
			}
			MutCvvImage.CopyOf(ImgRS3,1);
			MutCvvImage.DrawToHDC(hDC_img3,&rect_img3);
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CMUTDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	MousePtM = point;
	CPoint RealP;
	if (ImgRS1 != NULL)
	{
		RealP.x = (point.x-20);
		RealP.y = (point.y-20);
		if (ImgType1 == 1)
		{
			int flag = 0;
			if (point.x >= 20 && point.x < 420 && point.y >= 20 && point.y < 320)
			{
				int NMaxW1 = (MMHW1[0]+1)/2, NMaxH1 = (MMHW1[1]+1)/2;
				int NMinW1 = (MMHW1[2]+1)/2, NMinH1 = (MMHW1[3]+1)/2;

				if (((RealP.x >= (384-NMaxW1)) && (RealP.x < 384) && (RealP.y >= (35-NMaxH1)) && (RealP.y < 35)) ||
					((RealP.x >= (384-NMinW1)) && (RealP.x < 384) && (RealP.y >= 264) && (RealP.y < (264+NMinH1))))
					flag = 1;

				if (flag != 1)
				{
					double hottmpr = cvGetReal2D(TMPIMG1,RealP.y,RealP.x);
					if (NumFlag1 != 1)
					{
						CString strt = _T("查看图像温度信息前，请先点击确认温度信息识别无误！");
						muttooltip1.UpdateTipText(strt,this);
					}
					else
					{
						CString strt = _T("");
						strt.Format(_T("温度:%.1f℃"),hottmpr);
						muttooltip1.UpdateTipText(strt,this);
					}
				}
			}
		}
		else if (ImgType1 == 2)
		{
			if (point.x >= 20 && point.x < 420 && point.y >= 20 && point.y < 320)
			{
				double hottmpr = cvGetReal2D(TMPIMG1,RealP.y,RealP.x);
				if (NumFlag1 != 1)
				{
					CString strt = _T("查看图像温度信息前，请先点击确认温度信息识别无误！");
					muttooltip1.UpdateTipText(strt,this);
				}
				else
				{
					CString strt = _T("");
					strt.Format(_T("温度:%.1f℃"),hottmpr);
					muttooltip1.UpdateTipText(strt,this);
				}
			}
		}
	}
	
	if (ImgRS2 != NULL)
	{
		RealP.x = (point.x-490);
		RealP.y = (point.y-20);
		if (ImgType2 == 1)
		{
			int flag = 0;
			if (point.x >= 490 && point.x < 890 && point.y >= 20 && point.y < 320)
			{
				int NMaxW2 = (MMHW2[0]+1)/2, NMaxH2 = (MMHW2[1]+1)/2;
				int NMinW2 = (MMHW2[2]+1)/2, NMinH2 = (MMHW2[3]+1)/2;

				if (((RealP.x >= (384-NMaxW2)) && (RealP.x < 384) && (RealP.y >= (35-NMaxH2)) && (RealP.y < 35)) ||
					((RealP.x >= (384-NMinW2)) && (RealP.x < 384) && (RealP.y >= 264) && (RealP.y < (264+NMinH2))))
					flag = 1;

				if (flag != 1)
				{
					double hottmpr = cvGetReal2D(TMPIMG2,RealP.y,RealP.x);
					if (NumFlag2 != 1)
					{
						CString strt = _T("查看图像温度信息前，请先点击确认温度信息识别无误！");
						muttooltip2.UpdateTipText(strt,this);
					}
					else
					{
						CString strt = _T("");
						strt.Format(_T("温度:%.1f℃"),hottmpr);
						muttooltip2.UpdateTipText(strt,this);
					}
				}
			}
		}
		else if (ImgType2 == 2)
		{
			if (point.x >= 490 && point.x < 890 && point.y >= 20 && point.y < 320)
			{
				double hottmpr = cvGetReal2D(TMPIMG2,RealP.y,RealP.x);
				if (NumFlag2 != 1)
				{
					CString strt = _T("查看图像温度信息前，请先点击确认温度信息识别无误！");
					muttooltip2.UpdateTipText(strt,this);
				}
				else
				{
					CString strt = _T("");
					strt.Format(_T("温度:%.1f℃"),hottmpr);
					muttooltip2.UpdateTipText(strt,this);
				}
			}
		}
	}

	if (ImgRS3 != NULL)
	{
		RealP.x = (point.x-960);
		RealP.y = (point.y-20);
		if (ImgType3 == 1)
		{
			int flag = 0;
			if (point.x >= 960 && point.x < 1360 && point.y >= 20 && point.y < 320)
			{
				int NMaxW3 = (MMHW3[0]+1)/2, NMaxH3 = (MMHW3[1]+1)/2;
				int NMinW3 = (MMHW3[2]+1)/2, NMinH3 = (MMHW3[3]+1)/2;

				if (((RealP.x >= (384-NMaxW3)) && (RealP.x < 384) && (RealP.y >= (35-NMaxH3)) && (RealP.y < 35)) ||
					((RealP.x >= (384-NMinW3)) && (RealP.x < 384) && (RealP.y >= 264) && (RealP.y < (264+NMinH3))))
					flag = 1;

				if (flag != 1)
				{
					double hottmpr = cvGetReal2D(TMPIMG3,RealP.y,RealP.x);
					if (NumFlag3 != 1)
					{
						CString strt = _T("查看图像温度信息前，请先点击确认温度信息识别无误！");
						muttooltip3.UpdateTipText(strt,this);
					}
					else
					{
						CString strt = _T("");
						strt.Format(_T("温度:%.1f℃"),hottmpr);
						muttooltip3.UpdateTipText(strt,this);
					}
				}
			}
		}
		else if (ImgType3 == 2)
		{
			if (point.x >= 960 && point.x < 1360 && point.y >= 20 && point.y < 320)
			{
				double hottmpr = cvGetReal2D(TMPIMG3,RealP.y,RealP.x);
				if (NumFlag3 != 1)
				{
					CString strt = _T("查看图像温度信息前，请先点击确认温度信息识别无误！");
					muttooltip3.UpdateTipText(strt,this);
				}
				else
				{
					CString strt = _T("");
					strt.Format(_T("温度:%.1f℃"),hottmpr);
					muttooltip3.UpdateTipText(strt,this);
				}
			}
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CMUTDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint RealP;
	double tmptmp;
	CString SStmp = _T("");
	CString Lx,Ly,Lc;
	if (SMinT1 != _T(""))
	{
		if (point.x >= 20 && point.x < 420 && point.y >= 20 && point.y < 320)
		{
			RealP.x = (point.x-20);
			RealP.y = (point.y-20);
			if (ImgType1 == 1)
			{
				int NMaxW1 = (MMHW1[0]+1)/2, NMaxH1 = (MMHW1[1]+1)/2;
				int NMinW1 = (MMHW1[2]+1)/2, NMinH1 = (MMHW1[3]+1)/2;
				
				if (((RealP.x >= (384-NMaxW1)) && (RealP.x < 384) && (RealP.y >= (35-NMaxH1)) && (RealP.y < 35)) ||
					((RealP.x >= (384-NMinW1)) && (RealP.x < 384) && (RealP.y >= 264) && (RealP.y < (264+NMinH1))))
				{
					MessageBox(_T("点击区域无效，请点击图像区域！"));
					return;
				}
				else
				{
					MTListCtrl.DeleteItem(0);
					Lx.Format(_T("%d"),RealP.x);
					Ly.Format(_T("%d"),RealP.y);
					Lc = _T('(') + Lx + _T(',') + Ly + _T(')');

					double tmptmp = cvGetReal2D(TMPIMG1,RealP.y,RealP.x);
					SStmp.Format(_T("%.2f"),tmptmp);
					MTListCtrl.InsertItem(0,_T("图1"));
					MTListCtrl.SetItemText(0,1,Lc);
					MTListCtrl.SetItemText(0,2,SStmp);
				}
			}
			else if (ImgType1 == 2)
			{
				MTListCtrl.DeleteItem(0);
				Lx.Format(_T("%d"),RealP.x);
				Ly.Format(_T("%d"),RealP.y);
				Lc = _T('(') + Lx + _T(',') + Ly + _T(')');

				double tmptmp = cvGetReal2D(TMPIMG1,RealP.y,RealP.x);
				SStmp.Format(_T("%.2f"),tmptmp);
				MTListCtrl.InsertItem(0,_T("图1"));
				MTListCtrl.SetItemText(0,1,Lc);
				MTListCtrl.SetItemText(0,2,SStmp);
			}
		}
	}
	if (SMinT2 != _T(""))
	{
		if (point.x >= 490 && point.x < 890 && point.y >= 20 && point.y < 320)
		{
			RealP.x = (point.x-490);
			RealP.y = (point.y-20);
			if (ImgType2 == 1)
			{
				int NMaxW2 = (MMHW2[0]+1)/2, NMaxH2 = (MMHW2[1]+1)/2;
				int NMinW2 = (MMHW2[2]+1)/2, NMinH2 = (MMHW2[3]+1)/2;
				
				if (((RealP.x >= (384-NMaxW2)) && (RealP.x < 384) && (RealP.y >= (35-NMaxH2)) && (RealP.y < 35)) ||
					((RealP.x >= (384-NMinW2)) && (RealP.x < 384) && (RealP.y >= 264) && (RealP.y < (264+NMinH2))))
				{
					MessageBox(_T("点击区域无效，请点击图像区域！"));
					return;
				}
				else
				{
					MTListCtrl.DeleteItem(1);
					Lx.Format(_T("%d"),RealP.x);
					Ly.Format(_T("%d"),RealP.y);
					Lc = _T('(') + Lx + _T(',') + Ly + _T(')');

					double tmptmp = cvGetReal2D(TMPIMG2,RealP.y,RealP.x);
					SStmp.Format(_T("%.2f"),tmptmp);
					MTListCtrl.InsertItem(1,_T("图2"));
					MTListCtrl.SetItemText(1,1,Lc);
					MTListCtrl.SetItemText(1,2,SStmp);
				}
			}
			else if (ImgType2 == 2)
			{
				MTListCtrl.DeleteItem(1);
				Lx.Format(_T("%d"),RealP.x);
				Ly.Format(_T("%d"),RealP.y);
				Lc = _T('(') + Lx + _T(',') + Ly + _T(')');

				double tmptmp = cvGetReal2D(TMPIMG2,RealP.y,RealP.x);
				SStmp.Format(_T("%.2f"),tmptmp);
				MTListCtrl.InsertItem(1,_T("图2"));
				MTListCtrl.SetItemText(1,1,Lc);
				MTListCtrl.SetItemText(1,2,SStmp);
			}
		}
	}
	if (SMinT3 != _T(""))
	{
		if (point.x >= 960 && point.x < 1360 && point.y >= 20 && point.y < 320)
		{
			RealP.x = (point.x-960);
			RealP.y = (point.y-20);
			if (ImgType3 == 1)
			{
				int NMaxW3 = (MMHW3[0]+1)/2, NMaxH3 = (MMHW3[1]+1)/2;
				int NMinW3 = (MMHW3[2]+1)/2, NMinH3 = (MMHW3[3]+1)/2;
				
				if (((RealP.x >= (384-NMaxW3)) && (RealP.x < 384) && (RealP.y >= (35-NMaxH3)) && (RealP.y < 35)) ||
					((RealP.x >= (384-NMinW3)) && (RealP.x < 384) && (RealP.y >= 264) && (RealP.y < (264+NMinH3))))
				{
					MessageBox(_T("点击区域无效，请点击图像区域！"));
					return;
				}
				else
				{
					MTListCtrl.DeleteItem(2);
					Lx.Format(_T("%d"),RealP.x);
					Ly.Format(_T("%d"),RealP.y);
					Lc = _T('(') + Lx + _T(',') + Ly + _T(')');

					double tmptmp = cvGetReal2D(TMPIMG3,RealP.y,RealP.x);
					SStmp.Format(_T("%.2f"),tmptmp);
					MTListCtrl.InsertItem(2,_T("图3"));
					MTListCtrl.SetItemText(2,1,Lc);
					MTListCtrl.SetItemText(2,2,SStmp);
				}
			}
			else if (ImgType3 == 2)
			{
				MTListCtrl.DeleteItem(2);
				Lx.Format(_T("%d"),RealP.x);
				Ly.Format(_T("%d"),RealP.y);
				Lc = _T('(') + Lx + _T(',') + Ly + _T(')');

				double tmptmp = cvGetReal2D(TMPIMG3,RealP.y,RealP.x);
				SStmp.Format(_T("%.2f"),tmptmp);
				MTListCtrl.InsertItem(2,_T("图3"));
				MTListCtrl.SetItemText(2,1,Lc);
				MTListCtrl.SetItemText(2,2,SStmp);
			}
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CMUTDlg::OnBnClickedLoadimg1()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_FLUKEBAR_FRAME1)->ShowWindow(SW_HIDE);
	CString filepath;
	CString szFilters = _T("image files (*.bmp; *.jpg) |*.bmp;*.jpg|All Files (*.*)|*.*||");
	CFileDialog fileDlg(TRUE,_T("*.bmp"),_T(""),OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,szFilters,NULL);
	char title[] = {"打开图像"};
	fileDlg.m_ofn.lpstrTitle = stringToCString(title);
	if (fileDlg.DoModal() == IDOK)
	{
		filepath = fileDlg.GetPathName();
		MUTIMG1 = cvLoadImage(CStringToChar(filepath));
		ImgRS1 = cvCreateImage(cvSize(400,300),MUTIMG1->depth,MUTIMG1->nChannels);
		int OW = MUTIMG1->width, OH = MUTIMG1->height;
		if (OW == 800 && OH == 600)
		{
			ImgType1 = 1;
			cvResize(MUTIMG1,ImgRS1,CV_INTER_CUBIC);
			MutCvvImage.CopyOf(ImgRS1,1);
			MutCvvImage.DrawToHDC(hDC_img1,&rect_img1);
		}
		else if (OW == 392 && OH == 240)
		{
			GetDlgItem(IDC_FLUKEBAR_FRAME1)->ShowWindow(SW_SHOW);
			ImgType1 = 2;
			IplImage* tmpOrg = cvCreateImage(cvSize(320,240),MUTIMG1->depth,MUTIMG1->nChannels);
			cvSetImageROI(MUTIMG1,cvRect(0,0,320,240));
			cvCopy(MUTIMG1,tmpOrg);
			cvResetImageROI(MUTIMG1);
			cvResize(tmpOrg,ImgRS1,CV_INTER_CUBIC);
			ScaleImg1 = cvCreateImage(cvSize(72,240),MUTIMG1->depth,MUTIMG1->nChannels);
			cvSetImageROI(MUTIMG1,cvRect(320,0,72,240));
			cvCopy(MUTIMG1,ScaleImg1);
			cvResetImageROI(MUTIMG1);
			MutCvvImage.CopyOf(ImgRS1,1);
			MutCvvImage.DrawToHDC(hDC_img1,&rect_img1);
			MutCvvImage.CopyOf(ScaleImg1,1);
			MutCvvImage.DrawToHDC(hDC_bar1,&rect_bar1);
		}
		else
		{
			MessageBox(_T("选取的图像不合要求，请重新选取！\n也可选择FLIR或FLUKE软件处理图像！"));
			return;
		}
	}

	if (MUTIMG1 != NULL)
	{
		SetDlgItemText(IDC_MAXT1,_T(""));
		SetDlgItemText(IDC_MINT1,_T(""));
		TMPIMG1 = cvCreateImage(cvSize(400,300),32,1);
		GrayImg1 = cvCreateImage(cvSize(400,300),MUTIMG1->depth,1);
		if (ImgType1 == 1)
			ImageSeparate(MUTIMG1,ImgRS1,MMHW1,4,MaxT1,MinT1,ColorBar1,256);
		else if (ImgType1 == 2)
			ImageSeparateK(MUTIMG1,ImgRS1,ScaleImg1,MaxT1,MinT1,ColorBar1,256);

		SMinT1.Format(_T("%.1f"),MinT1);
		SMaxT1.Format(_T("%.1f"),MaxT1);

		CEdit *MinEdit1, *MaxEdit1;
		MinEdit1 = (CEdit*)GetDlgItem(IDC_MINT1);
		MaxEdit1 = (CEdit*)GetDlgItem(IDC_MAXT1);
		MinEdit1->SetWindowText(SMinT1);
		MaxEdit1->SetWindowText(SMaxT1);
	}
	else
	{
		return;
	}
}

void CMUTDlg::OnBnClickedLoadimg2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filepath;
	CString szFilters = _T("image files (*.bmp; *.jpg) |*.bmp;*.jpg|All Files (*.*)|*.*||");
	CFileDialog fileDlg(TRUE,_T("*.bmp"),_T(""),OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,szFilters,NULL);
	char title[] = {"打开图像"};
	fileDlg.m_ofn.lpstrTitle = stringToCString(title);
	if (fileDlg.DoModal() == IDOK)
	{
		filepath = fileDlg.GetPathName();
		MUTIMG2 = cvLoadImage(CStringToChar(filepath));
		ImgRS2 = cvCreateImage(cvSize(400,300),MUTIMG2->depth,MUTIMG2->nChannels);
		int OW = MUTIMG2->width, OH = MUTIMG2->height;
		if (OW == 800 && OH == 600)
		{
			ImgType2 = 1;
			cvResize(MUTIMG2,ImgRS2,CV_INTER_CUBIC);
			MutCvvImage.CopyOf(ImgRS2,1);
			MutCvvImage.DrawToHDC(hDC_img2,&rect_img2);
		}
		else if (OW == 392 && OH == 240)
		{
			GetDlgItem(IDC_FLUKEBAR_FRAME2)->ShowWindow(SW_SHOW);
			ImgType2 = 2;
			IplImage* tmpOrg = cvCreateImage(cvSize(320,240),MUTIMG1->depth,MUTIMG1->nChannels);
			cvSetImageROI(MUTIMG2,cvRect(0,0,320,240));
			cvCopy(MUTIMG2,tmpOrg);
			cvResetImageROI(MUTIMG2);
			cvResize(tmpOrg,ImgRS2,CV_INTER_CUBIC);
			ScaleImg2 = cvCreateImage(cvSize(72,240),MUTIMG2->depth,MUTIMG2->nChannels);
			cvSetImageROI(MUTIMG2,cvRect(320,0,72,240));
			cvCopy(MUTIMG2,ScaleImg2);
			cvResetImageROI(MUTIMG2);
			MutCvvImage.CopyOf(ImgRS2,1);
			MutCvvImage.DrawToHDC(hDC_img2,&rect_img2);
			MutCvvImage.CopyOf(ScaleImg2,1);
			MutCvvImage.DrawToHDC(hDC_bar2,&rect_bar2);
		}
		else
		{
			MessageBox(_T("选取的图像不合要求，请重新选取！\n也可选择FLIR或FLUKE软件处理图像！"));
			return;
		}
	}

	if (MUTIMG2 != NULL)
	{
		SetDlgItemText(IDC_MAXT2,_T(""));
		SetDlgItemText(IDC_MINT2,_T(""));
		TMPIMG2 = cvCreateImage(cvSize(400,300),32,1);
		GrayImg2 = cvCreateImage(cvSize(400,300),MUTIMG2->depth,1);
		if (ImgType2 == 1)
			ImageSeparate(MUTIMG2,ImgRS2,MMHW2,4,MaxT2,MinT2,ColorBar2,256);
		else if (ImgType2 == 2)
			ImageSeparateK(MUTIMG2,ImgRS2,ScaleImg2,MaxT2,MinT2,ColorBar2,256);
		
		SMinT2.Format(_T("%.1f"),MinT2);
		SMaxT2.Format(_T("%.1f"),MaxT2);

		CEdit *MinEdit2, *MaxEdit2;
		MinEdit2 = (CEdit*)GetDlgItem(IDC_MINT2);
		MaxEdit2 = (CEdit*)GetDlgItem(IDC_MAXT2);
		MinEdit2->SetWindowText(SMinT2);
		MaxEdit2->SetWindowText(SMaxT2);		
	}
	else
	{
		return;
	}
}

void CMUTDlg::OnBnClickedLoadimg3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filepath;
	CString szFilters = _T("image files (*.bmp; *.jpg) |*.bmp;*.jpg|All Files (*.*)|*.*||");
	CFileDialog fileDlg(TRUE,_T("*.bmp"),_T(""),OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,szFilters,NULL);
	char title[] = {"打开图像"};
	fileDlg.m_ofn.lpstrTitle = stringToCString(title);
	if (fileDlg.DoModal() == IDOK)
	{
		filepath = fileDlg.GetPathName();
		MUTIMG3 = cvLoadImage(CStringToChar(filepath));
		ImgRS3 = cvCreateImage(cvSize(400,300),MUTIMG3->depth,MUTIMG3->nChannels);
		int OW = MUTIMG3->width, OH = MUTIMG3->height;
		if (OW == 800 && OH == 600)
		{
			ImgType3 = 1;
			cvResize(MUTIMG3,ImgRS3,CV_INTER_CUBIC);
			MutCvvImage.CopyOf(ImgRS3,1);
			MutCvvImage.DrawToHDC(hDC_img3,&rect_img3);
		}
		else if (OW == 392 && OH == 240)
		{
			GetDlgItem(IDC_FLUKEBAR_FRAME3)->ShowWindow(SW_SHOW);
			ImgType3 = 2;
			IplImage* tmpOrg = cvCreateImage(cvSize(320,240),MUTIMG3->depth,MUTIMG3->nChannels);
			cvSetImageROI(MUTIMG3,cvRect(0,0,320,240));
			cvCopy(MUTIMG3,tmpOrg);
			cvResetImageROI(MUTIMG3);
			cvResize(tmpOrg,ImgRS3,CV_INTER_CUBIC);
			ScaleImg3 = cvCreateImage(cvSize(72,240),MUTIMG3->depth,MUTIMG3->nChannels);
			cvSetImageROI(MUTIMG3,cvRect(320,0,72,240));
			cvCopy(MUTIMG3,ScaleImg3);
			cvResetImageROI(MUTIMG3);
			MutCvvImage.CopyOf(ImgRS3,1);
			MutCvvImage.DrawToHDC(hDC_img3,&rect_img3);
			MutCvvImage.CopyOf(ScaleImg3,1);
			MutCvvImage.DrawToHDC(hDC_bar3,&rect_bar3);
		}
		else
		{
			MessageBox(_T("选取的图像不合要求，请重新选取！\n也可选择FLIR或FLUKE软件处理图像！"));
			return;
		}
	}

	if (MUTIMG3 != NULL)
	{
		SetDlgItemText(IDC_MAXT3,_T(""));
		SetDlgItemText(IDC_MINT3,_T(""));
		TMPIMG3 = cvCreateImage(cvSize(400,300),32,1);
		GrayImg3 = cvCreateImage(cvSize(400,300),MUTIMG3->depth,1);
		if (ImgType3 == 1)
			ImageSeparate(MUTIMG3,ImgRS3,MMHW3,4,MaxT3,MinT3,ColorBar3,256);
		else if (ImgType3 == 2)
			ImageSeparateK(MUTIMG3,ImgRS3,ScaleImg3,MaxT3,MinT3,ColorBar3,256);

		SMinT3.Format(_T("%.1f"),MinT3);
		SMaxT3.Format(_T("%.1f"),MaxT3);

		CEdit *MinEdit3, *MaxEdit3;
		MinEdit3 = (CEdit*)GetDlgItem(IDC_MINT3);
		MaxEdit3 = (CEdit*)GetDlgItem(IDC_MAXT3);
		MinEdit3->SetWindowText(SMinT3);
		MaxEdit3->SetWindowText(SMaxT3);
	}
	else
	{
		return;
	}
}

void CMUTDlg::ClimgCvtTpimg(IplImage* PreTmp, IplImage* GrayImgr, IplImage* TmpImg, double& MaxTmp, double& MinTmp, double ColorBar[][3], int size2)
{
	//ImageSeparate(ColImg,PreTmp,MMHW,4,MaxTmp,MinTmp,ColorBar,256);
	PseColor2Gray(PreTmp,GrayImgr,ColorBar,256);
	cvNormalize(GrayImgr,TmpImg,MaxTmp,MinTmp,CV_MINMAX);
}

void CMUTDlg::ImageSeparate(IplImage* SrcImg, IplImage* PreTmp, int MMHW[], int size1, double& MaxTmp, double& MinTmp, double ColorBar[][3], int size2)
{
	int i,j;
	IplImage* SrcIR = cvCreateImage(cvGetSize(SrcImg),SrcImg->depth,SrcImg->nChannels);
	cvCopy(SrcImg,SrcIR);

	//////*改造colorbar*//////
	IplImage* ColorBarO = cvCreateImage(cvSize(37,454),SrcImg->depth,SrcImg->nChannels);
	cvSetImageROI(SrcIR,cvRect(732,73,37,454));
	cvCopy(SrcIR,ColorBarO);
	cvResetImageROI(SrcIR);

	//得到MAP//
	IplImage* ColorPure = cvCreateImage(cvSize(15,450),SrcImg->depth,SrcImg->nChannels);
	cvSetImageROI(ColorBarO,cvRect(15,2,15,450));
	cvCopy(ColorBarO,ColorPure);
	cvResetImageROI(ColorBarO);
	IplImage* ColorResz = cvCreateImage(cvSize(15,256),SrcImg->depth,SrcImg->nChannels);
	cvResize(ColorPure,ColorResz,CV_INTER_CUBIC);
	//cvShowImage("ColorResz",ColorResz);
	IplImage* CPR = cvCreateImage(cvGetSize(ColorResz),ColorResz->depth,1);
	IplImage* CPG = cvCreateImage(cvGetSize(ColorResz),ColorResz->depth,1);
	IplImage* CPB = cvCreateImage(cvGetSize(ColorResz),ColorResz->depth,1);
	cvSplit(ColorResz,CPB,CPG,CPR,NULL);
	/*cvShowImage("CPR",CPR);
	cvShowImage("CPG",CPG);
	cvShowImage("CPB",CPB);*/
	
	for (i=0; i<256; i++)
	{
		double tmpB=0, tmpG=0, tmpR=0;
		for (j=0; j<15; j++)
		{
			tmpB += cvGetReal2D(CPB,i,j);
			tmpG += cvGetReal2D(CPG,i,j);
			tmpR += cvGetReal2D(CPR,i,j);
		}
		ColorBar[255-i][0] = tmpB/(double)15;
		ColorBar[255-i][1] = tmpG/(double)15;
		ColorBar[255-i][2] = tmpR/(double)15;
	}

	//边框变灰//
	IplImage* CBR = cvCreateImage(cvGetSize(ColorBarO),ColorBarO->depth,1);
	IplImage* CBG = cvCreateImage(cvGetSize(ColorBarO),ColorBarO->depth,1);
	IplImage* CBB = cvCreateImage(cvGetSize(ColorBarO),ColorBarO->depth,1);
	cvSplit(ColorBarO,CBB,CBG,CBR,NULL);
	for (i=0; i<454; i++)
	{
		cvSetReal2D(CBR,i,0,127);
		cvSetReal2D(CBR,i,1,127);
		cvSetReal2D(CBR,i,35,127);
		cvSetReal2D(CBR,i,36,127);

		cvSetReal2D(CBG,i,0,127);
		cvSetReal2D(CBG,i,1,127);
		cvSetReal2D(CBG,i,35,127);
		cvSetReal2D(CBG,i,36,127);

		cvSetReal2D(CBB,i,0,127);
		cvSetReal2D(CBB,i,1,127);
		cvSetReal2D(CBB,i,35,127);
		cvSetReal2D(CBB,i,36,127);		
	}
	for (j=0; j<37; j++)
	{
		cvSetReal2D(CBR,0,j,127);
		cvSetReal2D(CBR,1,j,127);
		cvSetReal2D(CBR,452,j,127);
		cvSetReal2D(CBR,453,j,127);

		cvSetReal2D(CBG,0,j,127);
		cvSetReal2D(CBG,1,j,127);
		cvSetReal2D(CBG,452,j,127);
		cvSetReal2D(CBG,453,j,127);

		cvSetReal2D(CBG,0,j,127);
		cvSetReal2D(CBG,1,j,127);
		cvSetReal2D(CBG,452,j,127);
		cvSetReal2D(CBG,453,j,127);
	}
	cvMerge(CBB,CBG,CBR,NULL,ColorBarO);

	//////*温度字符识别*//////
	//字符区域分割//
	IplImage* MaxImg = cvCreateImage(cvSize(137,45),SrcImg->depth,SrcIR->nChannels);
	IplImage* MinImg = cvCreateImage(cvSize(82,45),SrcImg->depth,SrcIR->nChannels);
	cvSetImageROI(SrcIR,cvRect(632,26,137,45));
	cvCopy(SrcIR,MaxImg);
	cvResetImageROI(SrcIR);
	cvSetImageROI(SrcIR,cvRect(687,529,82,45));
	cvCopy(SrcIR,MinImg);
	cvResetImageROI(SrcIR);

	//灰度化//
	IplImage* MaxImgGr = cvCreateImage(cvGetSize(MaxImg),MaxImg->depth,1);
	IplImage* MinImgGr = cvCreateImage(cvGetSize(MinImg),MinImg->depth,1);
	cvCvtColor(MaxImg,MaxImgGr,CV_BGR2GRAY);
	cvCvtColor(MinImg,MinImgGr,CV_BGR2GRAY);

	//顶帽变换(形态学处理)//
	IplConvKernel* Elm = cvCreateStructuringElementEx(7,7,3,3,CV_SHAPE_ELLIPSE);
	IplImage* MaxImgMo = cvCreateImage(cvGetSize(MaxImg),MaxImg->depth,1);
	IplImage* MinImgMo = cvCreateImage(cvGetSize(MinImg),MinImg->depth,1);
	IplImage* Maxtemp = cvCreateImage(cvGetSize(MaxImg),MaxImg->depth,1);
	IplImage* Mintemp = cvCreateImage(cvGetSize(MinImg),MinImg->depth,1);
	cvZero(Maxtemp);
	cvZero(Mintemp);
	cvMorphologyEx(MaxImgGr,MaxImgMo,Maxtemp,Elm,CV_MOP_TOPHAT,1);
	cvMorphologyEx(MinImgGr,MinImgMo,Mintemp,Elm,CV_MOP_TOPHAT,1);
	cvReleaseStructuringElement(&Elm);

	//二值化//
	IplImage* MaxImgBW = cvCreateImage(cvGetSize(MaxImg),MaxImg->depth,1);
	IplImage* MinImgBW = cvCreateImage(cvGetSize(MinImg),MinImg->depth,1);
	cvThreshold(MaxImgMo,MaxImgBW,205,255,CV_THRESH_BINARY);
	cvThreshold(MinImgMo,MinImgBW,205,255,CV_THRESH_BINARY);

	//最小外接矩分割后用于识别//
	CvRect Maxrect = MinRect(MaxImgBW);
	CvRect Minrect = MinRect(MinImgBW);
	IplImage* Maximgct = cvCreateImage(cvSize(Maxrect.width,Maxrect.height),8,1);
	IplImage* Minimgct = cvCreateImage(cvSize(Minrect.width,Minrect.height),8,1);
	cvSetImageROI(MaxImgBW,Maxrect);
	cvCopy(MaxImgBW,Maximgct);
	cvResetImageROI(MaxImgBW);
	cvSetImageROI(MinImgBW,Minrect);
	cvCopy(MinImgBW,Minimgct);
	cvResetImageROI(MinImgBW);

	int MaxH = Maxrect.height, MaxW = Maxrect.width;
	int MinH = Minrect.height, MinW = Minrect.width;

	//三通道化后用于重组//
	IplImage* MMaxImgt = cvCreateImage(cvSize(MaxW+8,MaxH+14),8,1);
	cvZero(MMaxImgt);
	cvSetImageROI(MMaxImgt,cvRect(2,5,MaxW,MaxH));
	cvCopy(Maximgct,MMaxImgt);
	cvResetImageROI(MMaxImgt);
	IplImage* MaxImgCl = cvCreateImage(cvSize(MaxW+8,MaxH+14),8,3);
	cvCvtColor(MMaxImgt,MaxImgCl,CV_GRAY2BGR);

	IplImage* MMinImgt = cvCreateImage(cvSize(MinW+8,MinH+14),8,1);
	cvZero(MMinImgt);
	cvSetImageROI(MMinImgt,cvRect(2,5,MinW,MinH));
	cvCopy(Minimgct,MMinImgt);
	cvResetImageROI(MMinImgt);
	IplImage* MinImgCl = cvCreateImage(cvSize(MinW+8,MinH+14),8,3);
	cvCvtColor(MMinImgt,MinImgCl,CV_GRAY2BGR);

	//字符识别//
	MinMaxRecognize(Maximgct,Minimgct,MaxTmp,MinTmp);


	//////*图像区域重组*//////
	IplImage* PreSrc = cvCreateImage(cvGetSize(SrcIR),SrcIR->depth,SrcIR->nChannels);
	cvCopy(SrcIR,PreSrc);

	cvSetImageROI(PreSrc,cvRect(732,73,37,454));
	cvCopy(ColorBarO,PreSrc);
	cvResetImageROI(PreSrc);

	int NMaxW = MaxImgCl->width, NMaxH = MaxImgCl->height;
	cvSetImageROI(PreSrc,cvRect(769-NMaxW,72-NMaxH,NMaxW,NMaxH));
	cvCopy(MaxImgCl,PreSrc);
	cvResetImageROI(PreSrc);

	int NMinW = MinImgCl->width, NMinH = MinImgCl->height;
	cvSetImageROI(PreSrc,cvRect(769-NMinW,529,NMinW,NMinH));
	cvCopy(MinImgCl,PreSrc);
	cvResetImageROI(PreSrc);

	MMHW[0] = NMaxW; MMHW[1] = NMaxH;
	MMHW[2] = NMinW; MMHW[3] = NMinH;

	int HH = SrcIR->height, WW = SrcIR->width;
	IplImage* ReSrc = cvCreateImage(cvSize(WW/2,HH/2),SrcIR->depth,SrcIR->nChannels);
	cvResize(PreSrc,ReSrc,CV_INTER_CUBIC);

	IplImage* PreGray = cvCreateImage(cvGetSize(SrcIR),SrcIR->depth,SrcIR->nChannels);
	cvCopy(SrcIR,PreGray);
	IplImage* SubMax = cvCreateImage(cvGetSize(MaxImgCl),MaxImgCl->depth,MaxImgCl->nChannels);
	IplImage* SubMin = cvCreateImage(cvGetSize(MinImgCl),MinImgCl->depth,MinImgCl->nChannels);
	for (i=0; i<NMaxH; i++)
	{
		for (j=0; j<NMaxW; j++)
		{ 
			CV_IMAGE_ELEM(SubMax,uchar,i,3*j) = 127;
			CV_IMAGE_ELEM(SubMax,uchar,i,3*j+1) = 127;
			CV_IMAGE_ELEM(SubMax,uchar,i,3*j+2) = 127;
		}
	}
	for (i=0; i<NMinH; i++)
	{
		for (j=0; j<NMinW; j++)
		{
			CV_IMAGE_ELEM(SubMin,uchar,i,3*j) = 127;
			CV_IMAGE_ELEM(SubMin,uchar,i,3*j+1) = 127;
			CV_IMAGE_ELEM(SubMin,uchar,i,3*j+2) = 127;
		}
	}

	cvSetImageROI(PreGray,cvRect(769-NMaxW,72-NMaxH,NMaxW,NMaxH));
	cvCopy(SubMax,PreGray);
	cvResetImageROI(PreGray);

	cvSetImageROI(PreGray,cvRect(769-NMinW,529,NMinW,NMinH));
	cvCopy(SubMin,PreGray);
	cvResetImageROI(PreGray);

	cvSetImageROI(PreGray,cvRect(732,73,37,454));
	cvCopy(ColorBarO,PreGray);
	cvResetImageROI(PreGray);

	//PreTmp = cvCreateImage(cvGetSize(ReSrc),ReSrc->depth,ReSrc->nChannels);
	cvResize(PreGray,PreTmp,CV_INTER_CUBIC);

}

void CMUTDlg::ImageSeparateK(IplImage* SrcImg, IplImage* PreTmp, IplImage* ScaleImg, double& MaxTmp, double& MinTmp, double ColorBar[][3], int size2)
{
	int i,j;
	IplImage* SrcIR = cvCreateImage(cvGetSize(SrcImg),SrcImg->depth,SrcImg->nChannels);
	cvCopy(SrcImg,SrcIR);
	//cvShowImage("SrcIR",SrcIR);

	IplImage* tmpOrg = cvCreateImage(cvSize(320,240),SrcIR->depth,SrcIR->nChannels);
	cvSetImageROI(SrcIR,cvRect(0,0,320,240));
	cvCopy(SrcIR,tmpOrg);
	cvResetImageROI(SrcIR);
	cvResize(tmpOrg,PreTmp,CV_INTER_CUBIC);

	IplImage* ColorPure = cvCreateImage(cvSize(14,207),SrcIR->depth,SrcIR->nChannels);
	cvSetImageROI(SrcIR,cvRect(321,9,14,207));
	cvCopy(SrcIR,ColorPure);
	cvResetImageROI(SrcIR);
	IplImage* ColorResz = cvCreateImage(cvSize(14,256),SrcIR->depth,SrcIR->nChannels);
	cvResize(ColorPure,ColorResz,CV_INTER_CUBIC);

	IplImage* CPR = cvCreateImage(cvGetSize(ColorResz),ColorResz->depth,1);
	IplImage* CPG = cvCreateImage(cvGetSize(ColorResz),ColorResz->depth,1);
	IplImage* CPB = cvCreateImage(cvGetSize(ColorResz),ColorResz->depth,1);
	cvSplit(ColorResz,CPB,CPG,CPR,NULL);

	for (i=0; i<256; i++)
	{
		double tmpB=0, tmpG=0, tmpR=0;
		for (j=0; j<14; j++)
		{
			tmpB += cvGetReal2D(CPB,i,j);
			tmpG += cvGetReal2D(CPG,i,j);
			tmpR += cvGetReal2D(CPR,i,j);
		}
		ColorBar[255-i][0] = tmpB/(double)14;
		ColorBar[255-i][1] = tmpG/(double)14;
		ColorBar[255-i][2] = tmpR/(double)14;
	}
	
	IplImage* MaxImg = cvCreateImage(cvSize(40,13),SrcIR->depth,SrcIR->nChannels);
	IplImage* MinImg = cvCreateImage(cvSize(40,10),SrcIR->depth,SrcIR->nChannels);
	cvSetImageROI(ScaleImg,cvRect(32,0,40,13));
	cvCopy(ScaleImg,MaxImg);
	cvResetImageROI(ScaleImg);
	cvSetImageROI(ScaleImg,cvRect(32,212,40,10));
	cvCopy(ScaleImg,MinImg);
	cvResetImageROI(ScaleImg);
	/*cvShowImage("MaxImg",MaxImg);
	cvShowImage("MinImg",MinImg);*/

	//灰度化//
	IplImage* MaxImgGr = cvCreateImage(cvGetSize(MaxImg),MaxImg->depth,1);
	IplImage* MinImgGr = cvCreateImage(cvGetSize(MinImg),MinImg->depth,1);
	cvCvtColor(MaxImg,MaxImgGr,CV_BGR2GRAY);
	cvCvtColor(MinImg,MinImgGr,CV_BGR2GRAY);

	//二值化//
	IplImage* MaxImgBW = cvCreateImage(cvGetSize(MaxImg),MaxImg->depth,1);
	IplImage* MinImgBW = cvCreateImage(cvGetSize(MinImg),MinImg->depth,1);
	cvThreshold(MaxImgGr,MaxImgBW,205,255,CV_THRESH_BINARY_INV);
	cvThreshold(MinImgGr,MinImgBW,205,255,CV_THRESH_BINARY_INV);
	
	//最小外接矩分割后用于识别//
	CvRect Maxrect = MinRect(MaxImgBW);
	CvRect Minrect = MinRect(MinImgBW);
	IplImage* Maximgct = cvCreateImage(cvSize(Maxrect.width,Maxrect.height),8,1);
	IplImage* Minimgct = cvCreateImage(cvSize(Minrect.width,Minrect.height),8,1);
	cvSetImageROI(MaxImgBW,Maxrect);
	cvCopy(MaxImgBW,Maximgct);
	cvResetImageROI(MaxImgBW);
	cvSetImageROI(MinImgBW,Minrect);
	cvCopy(MinImgBW,Minimgct);
	cvResetImageROI(MinImgBW);

	int MaxH = Maxrect.height, MaxW = Maxrect.width;
	int MinH = Minrect.height, MinW = Minrect.width;

	//字符识别//
	MinMaxRecognizeK(Maximgct,Minimgct,MaxTmp,MinTmp);
}

CvRect CMUTDlg::MinRect(IplImage* src)
{
	int i,j,flag=0;
	int Top,Bottom,Left,Right;
	int m=src->height,n=src->width;
	for(i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			if(CV_IMAGE_ELEM(src,uchar,i,j)!=0)
			{
				Top=i;
				flag=1;
				break;
			}
		}
		if(flag==1)
		{
			flag=0;
			break;
		}
	}
	for(i=m-1;i>=0;i--)
	{
		for(j=0;j<n;j++)
		{
			if(CV_IMAGE_ELEM(src,uchar,i,j)!=0)
			{
				Bottom=i;
				flag=1;
				break;
			}
		}
		if(flag==1)
		{
			flag=0;
			break;
		}
	}
	for(j=0;j<n;j++)
	{
		for(i=0;i<m;i++)
		{
			if(CV_IMAGE_ELEM(src,uchar,i,j)!=0)
			{
				Left=j;
				flag=1;
				break;
			}
		}
		if(flag==1)
		{
			flag=0;
			break;
		}
	}
	for(j=n-1;j>=0;j--)
	{
		for(i=0;i<m;i++)
		{
			if(CV_IMAGE_ELEM(src,uchar,i,j)!=0)
			{
				Right=j;
				flag=1;
				break;
			}
		}
		if(flag==1)
		{
			flag=0;
			break;
		}
	}
	CvRect regtmp;
	regtmp.x=Left;
	regtmp.y=Top;
	regtmp.width=Right-Left+1;
	regtmp.height=Bottom-Top+1;
	return regtmp;
}

void CMUTDlg::MinMaxIdx(double arr[], int num, int &IdxMin, int &IdxMax)
{
	int i;
	IdxMin = IdxMax = 0;
	for (i=1; i<num; i++)
	{
		if (arr[i] < arr[IdxMin])
			IdxMin=i;
		if (arr[i] > arr[IdxMax])
			IdxMax=i;
	}
}

void CMUTDlg::InverseMap(IplImage* src, IplImage* dst, double& MaxT, double& MinT)
{
	IplImage* Src = cvCreateImage(cvSize(400,300),src->depth,src->nChannels);
	cvCopy(src,Src);
	IplImage* SB = cvCreateImage(cvGetSize(Src),8,1);
	IplImage* SG = cvCreateImage(cvGetSize(Src),8,1);
	IplImage* SR = cvCreateImage(cvGetSize(Src),8,1);
	cvSplit(Src,SB,SG,SR,NULL);

	IplImage* GrayImg = cvCreateImage(cvGetSize(Src),32,1);
	cvZero(GrayImg);
	int H = Src->height,W = Src->width;
	int i,j;
	double X;
	double gray;
	for (i=0; i<H; i++)
	{
		for (j=0; j<W; j++)
		{
			double Rr=CV_IMAGE_ELEM(SR,uchar,i,j);
			double Gg=CV_IMAGE_ELEM(SG,uchar,i,j);
			double Bb=CV_IMAGE_ELEM(SB,uchar,i,j);
			if (Rr<=14)
			{
				X = Bb;
				gray = Para[0][0]*X*X+Para[0][1]*X+Para[0][2];
			}
			else if ((14 < Rr) && (Rr <= 62))
			{
				X = Rr;
				gray = Para[1][0]*X*X+Para[1][1]*X+Para[1][2];
			}
			else if ((62 < Rr) && (Rr <= 178))
			{
				X = Rr;
				gray = Para[2][0]*X*X+Para[2][1]*X+Para[2][2];
			}
			else if (Rr>178)
			{
				if (Gg <= 21)
				{
					X = Gg;
					gray = Para[3][0]*X*X+Para[3][1]*X+Para[3][2];
				}
				else if ((21 < Gg) && (Gg <= 238))
				{
					X = Gg;
					gray = Para[4][0]*X*X+Para[4][1]*X+Para[4][2];
				}
				else if (Gg > 238)
				{
					X = Bb; 
					gray = Para[5][0]*X*X+Para[5][1]*X+Para[5][2];
				}
			}
			cvSetReal2D(GrayImg,i,j,gray);
		}
	}
	cvNormalize(GrayImg,dst,MinT,MaxT,CV_MINMAX);
}

void CMUTDlg::MinMaxRecognize(IplImage* MaxImgBW, IplImage* MinImgBW, double& MaxT, double& MinT)
{
	int i,j;
	IplImage* Maxcpy = cvCreateImage(cvGetSize(MaxImgBW),MaxImgBW->depth,MaxImgBW->nChannels);
	IplImage* Mincpy = cvCreateImage(cvGetSize(MinImgBW),MinImgBW->depth,MinImgBW->nChannels);
	cvCopy(MaxImgBW,Maxcpy);
	cvCopy(MinImgBW,Mincpy);

	double MaxH = Maxcpy->height, MaxW = Maxcpy->width;
	double MinH = Mincpy->height, MinW = Mincpy->width;
	double MaxR = MaxW/MaxH, MinR = MinW/MinH;


	if (MaxH < 25)
	{
		int MaxNH = 28, MaxNW = (double)MaxNH*MaxR;
		IplImage* Maximgctr = cvCreateImage(cvSize(MaxNW,MaxNH),8,1);
		cvResize(Maxcpy,Maximgctr,CV_INTER_NN);
		IplImage* Maximgctrr = cvCreateImage(cvSize(MaxNW-40,MaxNH),8,1);
		cvSetImageROI(Maximgctr,cvRect(0,0,MaxNW-40,MaxNH));
		cvCopy(Maximgctr,Maximgctrr);
		cvResetImageROI(Maximgctr);
		StringRecognize(Maximgctrr,MaxT);
	}
	else
	{
		IplImage* Maximgctrr = cvCreateImage(cvSize(MaxW-40,MaxH),8,1);
		cvSetImageROI(Maxcpy,cvRect(0,0,MaxW-40,MaxH));
		cvCopy(Maxcpy,Maximgctrr);
		cvResetImageROI(Maxcpy);
		StringRecognize(Maximgctrr,MaxT);
	}
	if (MinH < 25)
	{
		int MinNH = 28, MinNW = (double)MinNH*MinR;
		IplImage* Minimgctr = cvCreateImage(cvSize(MinNW,MinNH),8,1);
		cvResize(Mincpy,Minimgctr,CV_INTER_NN);
		StringRecognize(Minimgctr,MinT);
	}
	else
		StringRecognize(Mincpy,MinT);
	MinT = MinT * 0.1;
	MaxT = MaxT * 0.1;
}


void CMUTDlg::StringRecognize(IplImage* StringImg, double& TmpNum)
{
	double NumR = 0;

	CvRect RectMin = MinRect(StringImg);
	IplImage* RBWNum = cvCreateImage(cvSize(RectMin.width,RectMin.height),StringImg->depth,StringImg->nChannels);
	cvSetImageROI(StringImg,RectMin);
	cvCopy(StringImg,RBWNum);
	cvResetImageROI(StringImg);
	cvNormalize(RBWNum,RBWNum,1,0,CV_MINMAX);

	int i, j;
	int W = RBWNum->width, H=RBWNum->height;
	int *SumCol = new int[W];
	for (j=0; j<W; j++)
	{
		int tmpt = 0;
		for (i=0; i<H; i++)
		{
			tmpt += cvGetReal2D(RBWNum,i,j);
		}
		SumCol[j] = tmpt;
	}
	double idx = 0;
	double num = 0;
	int flag = 0;
	int left = 0;
	int NW = 0;
	int Case = 0;
	int Sign = 1;

	//CString nnn="";

	for (j=0; j<W; j++)
	{
		if ((SumCol[j] == 0 && flag == 0) || (j==W-1))
		{
			NW = j-left;
			IplImage* NumImg = cvCreateImage(cvSize(NW,H),RBWNum->depth,RBWNum->nChannels);
			cvSetImageROI(RBWNum,cvRect(left,0,NW,H));
			cvCopy(RBWNum,NumImg);
			cvResetImageROI(RBWNum);

			Case = NumRecognize(NumImg,idx,num);
			switch (Case)
			{
			case -1:
				Sign = -1;
				break;
			case 0:
				break;
			case 1:
				NumR = NumR*pow(10,idx) + num;
				idx++;
				break;
			}
			flag = 1;
		}
		if (SumCol[j] != 0 && flag == 1)
		{
			left = j;
			flag = 0;
		}
	}
	NumR = Sign * NumR;
	TmpNum = NumR;
}

int CMUTDlg::NumRecognize(IplImage* NumImg, double &idx, double &Num)
{
	CvMat Matimg;
	int Case = 0;
	if (cvCountNonZero(NumImg) <= 30 && idx != 0)
		return Case = 0;

	Num = 0;
	int i, j;
	int W = NumImg->width, H=NumImg->height;
	int flag=0;
	for (i=0; i<H; i++)
	{
		int tmpt = 0;
		for (j=0; j<W; j++)
		{
			tmpt += cvGetReal2D(NumImg,i,j);
		}
		if (tmpt > 0)
			flag++;
	}
	if (flag <= 4 && idx ==0)
		return Case = -1;

	IplImage* NNumImg;

	NNumImg = cvCreateImage(cvGetSize(NumImg),NumImg->depth,1);
	cvCopy(NumImg,NNumImg);

	IplImage* result = cvCreateImage(cvSize(24-NNumImg->width+1,45-NNumImg->height+1),32,1);
	double tmin[10], tmax[10];
	for (i=0; i<10; i++)
	{
		tmin[i] = 0;
		tmax[i] = 0;
	}
	CvPoint Lmin, Lmax;
	int IdxMin=0, IdxMax=0;
	if (W < 40)
	{
		if (idx == 0)
		{
			for (i=0; i<10; i++)
			{
				cvMatchTemplate(NumTemplMR[i],NNumImg,result,3);
				cvMinMaxLoc(result,&tmin[i],&tmax[i],&Lmin,&Lmax);
			}
		}
		else
		{
			for (i=0; i<10; i++)
			{
				cvMatchTemplate(NumTemplMR[i],NNumImg,result,3);
				cvMinMaxLoc(result,&tmin[i],&tmax[i],&Lmin,&Lmax);
			}
		}
		MinMaxIdx(tmax,10,IdxMin,IdxMax);
		Num = IdxMax;
		idx = 1;
	}
	else
	{
		int SW[2],left[2];
		SW[0] = W/2; SW[1] = W - SW[0];
		left[0] = 0; left[1]=SW[0];
		CString SWnn=_T("");
		for (i=0; i<2; i++)
		{
			if (SW[i] < 24)
			{
				NNumImg = cvCreateImage(cvSize(24,45),NumImg->depth,NumImg->nChannels);
				cvZero(NNumImg);
				cvSetImageROI(NNumImg,cvRect((24-SW[i])/2,(45-H)/2,SW[i],H));
				cvSetImageROI(NumImg,cvRect(left[i],0,SW[i],H));
				cvCopy(NumImg,NNumImg);
				cvResetImageROI(NNumImg);
				cvResetImageROI(NumImg);

			}
			else
			{
				NNumImg = cvCreateImage(cvSize(SW[i],45),NumImg->depth,NumImg->nChannels);
				cvZero(NNumImg);
				cvSetImageROI(NNumImg,cvRect(0,(45-H)/2,SW[i],H));
				cvSetImageROI(NumImg,cvRect(left[i],0,SW[i],H));
				cvCopy(NumImg,NNumImg);
				cvResetImageROI(NNumImg);
				cvResetImageROI(NumImg);
			}
			result = cvCreateImage(cvSize(NNumImg->width-24+1,NNumImg->height-45+1),32,1);
			if (idx == 0)
			{
				for (j=1; j<10; j++)
				{
					cvMatchTemplate(NNumImg,NumTemplMR[j],result,3);
					cvMinMaxLoc(result,&tmin[j],&tmax[j],&Lmin,&Lmax);
				}
				idx = 1;
			}
			else
			{
				for (j=0; j<10; j++)
				{
					cvMatchTemplate(NNumImg,NumTemplMR[j],result,3);
					cvMinMaxLoc(result,&tmin[j],&tmax[j],&Lmin,&Lmax);
				}
			}
			MinMaxIdx(tmax,10,IdxMin,IdxMax);
			Num = Num*pow(10,(double)i) + IdxMax;			
		}
		idx = 2;
	}
	return Case = 1;
}


void CMUTDlg::PseColor2Gray(IplImage* PseImg, IplImage* GrayImgr, double ColorBar[][3], int size)
{
	IplImage* Src = cvCreateImage(cvGetSize(PseImg),PseImg->depth,PseImg->nChannels);
	cvCopy(PseImg,Src);
	IplImage* SB = cvCreateImage(cvGetSize(Src),8,1);
	IplImage* SG = cvCreateImage(cvGetSize(Src),8,1);
	IplImage* SR = cvCreateImage(cvGetSize(Src),8,1);
	cvSplit(Src,SB,SG,SR,NULL);

	IplImage* Gray = cvCreateImage(cvGetSize(PseImg),16,1);

	double Dis[256];

	for (int i=0; i<300; i++)
	{
		for (int j=0; j<400; j++)
		{
			if (DialogCloseFlag)
			{
				return;
			}
			double r = cvGetReal2D(SR,i,j);
			double g = cvGetReal2D(SG,i,j);
			double b = cvGetReal2D(SB,i,j);
			for (int k=0; k<256; k++)
			{
				Dis[k] = pow((ColorBar[k][0]-b),2.0) + pow((ColorBar[k][1]-g),2.0) + pow((ColorBar[k][2]-r),2.0);
			}
			double min = Dis[0];
			int idx = 0;
			for (int k=1; k<256; k++)
			{
				if (min >= Dis[k])
				{
					min = Dis[k];
					idx = k;
				}
			}
			cvSetReal2D(Gray,i,j,(double)(idx));
		}
		((CProgressCtrl*)GetDlgItem(IDC_IR_PROGRESS1))->SetPos(int(i/3));
		//m_IR_Progress1.SetPos(int(i/3));
	}
	((CProgressCtrl*)GetDlgItem(IDC_IR_PROGRESS1))->SetPos(0);
	//m_IR_Progress1.SetPos(0);
	cvConvert(Gray,GrayImgr);
	//cvShowImage("GrayImgr",GrayImgr);
}
void CMUTDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ThreadIRImageOpenFlag1 = FALSE;
	ThreadIRImageOpenFlag2 = FALSE;
	ThreadIRImageOpenFlag3 = FALSE;
	ThreadIRImageRecogFinishFlag1 = FALSE;
	ThreadIRImageRecogFinishFlag2 = FALSE;
	ThreadIRImageRecogFinishFlag3 = FALSE;
	//MUTIMG1 = NULL;
	//MUTIMG2 = NULL;
	//MUTIMG3 = NULL;
	SMinT1 = "";
	SMinT2 = "";
	SMinT3 = "";
	DialogCloseFlag = TRUE;
	CDialog::OnClose();
}

void CMUTDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码4
	OnClose();
	OnCancel();
}

void CMUTDlg::OnBnClickedConform1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MUTIMG1 != NULL)
	{
		CString SMaxtmp,SMintmp;
		GetDlgItem(IDC_MAXT1)->GetWindowText(SMaxtmp);
		GetDlgItem(IDC_MINT1)->GetWindowText(SMintmp);
		MaxT1 = atof(CStringToChar(SMaxtmp.GetBuffer()));
		MinT1 = atof(CStringToChar(SMintmp.GetBuffer()));

		ThreadIRImageOpenFlag1 = TRUE;
		m_pThread_OpenIRImage1 = AfxBeginThread(ThreadProc_OpenIRImage1,this,0,0,0,NULL);
		hEvent_OpenIRImage1 = CreateEvent(NULL,FALSE,TRUE,stringToCString("event"));

		NumFlag1 = 1;
	}
	else
	{
		MessageBox(_T("请先载入图像1！"));
		return;
	}
}

void CMUTDlg::OnBnClickedConform2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MUTIMG2 != NULL)
	{
		CString SMaxtmp,SMintmp;
		GetDlgItem(IDC_MAXT2)->GetWindowText(SMaxtmp);
		GetDlgItem(IDC_MINT2)->GetWindowText(SMintmp);
		MaxT2 = atof(CStringToChar(SMaxtmp.GetBuffer()));
		MinT2 = atof(CStringToChar(SMintmp.GetBuffer()));

		ThreadIRImageOpenFlag2 = TRUE;
		m_pThread_OpenIRImage2 = AfxBeginThread(ThreadProc_OpenIRImage2,this,0,0,0,NULL);
		hEvent_OpenIRImage2 = CreateEvent(NULL,FALSE,TRUE,stringToCString("event"));

		NumFlag2 = 1;
	}
	else
	{
		MessageBox(_T("请先载入图像2！"));
		return;
	}
}

void CMUTDlg::OnBnClickedConform3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MUTIMG3 != NULL)
	{
		CString SMaxtmp,SMintmp;
		GetDlgItem(IDC_MAXT3)->GetWindowText(SMaxtmp);
		GetDlgItem(IDC_MINT3)->GetWindowText(SMintmp);
		MaxT3 = atof(CStringToChar(SMaxtmp.GetBuffer()));
		MinT3 = atof(CStringToChar(SMintmp.GetBuffer()));

		ThreadIRImageOpenFlag3 = TRUE;
		m_pThread_OpenIRImage3 = AfxBeginThread(ThreadProc_OpenIRImage3,this,0,0,0,NULL);
		hEvent_OpenIRImage3 = CreateEvent(NULL,FALSE,TRUE,stringToCString("event"));

		NumFlag3 = 1;
	}
	else
	{
		MessageBox(_T("请先载入图像3！"));
		return;
	}
}


void CMUTDlg::MinMaxRecognizeK(IplImage* MaxImgBW, IplImage* MinImgBW, double& MaxT, double& MinT)
{
	int i,j;
	IplImage* Maxcpy = cvCreateImage(cvGetSize(MaxImgBW),MaxImgBW->depth,MaxImgBW->nChannels);
	IplImage* Mincpy = cvCreateImage(cvGetSize(MinImgBW),MinImgBW->depth,MinImgBW->nChannels);
	cvCopy(MaxImgBW,Maxcpy);
	cvCopy(MinImgBW,Mincpy);

	double MaxH = Maxcpy->height, MaxW = Maxcpy->width;
	double MinH = Mincpy->height, MinW = Mincpy->width;
	double MaxR = MaxW/MaxH, MinR = MinW/MinH;

	StringRecognizeK(Maxcpy,MaxT);
	StringRecognizeK(Mincpy,MinT);
	MinT = MinT * 0.1;
	MaxT = MaxT * 0.1;
}

void CMUTDlg::StringRecognizeK(IplImage* StringImg, double& TmpNum)
{
	double NumR = 0;

	CvRect RectMin = MinRect(StringImg);
	IplImage* RBWNum = cvCreateImage(cvSize(RectMin.width,RectMin.height),StringImg->depth,StringImg->nChannels);
	cvSetImageROI(StringImg,RectMin);
	cvCopy(StringImg,RBWNum);
	cvResetImageROI(StringImg);
	cvNormalize(RBWNum,RBWNum,1,0,CV_MINMAX);

	int i, j;
	int W = RBWNum->width, H=RBWNum->height;
	int *SumCol = new int[W];
	for (j=0; j<W; j++)
	{
		int tmpt = 0;
		for (i=0; i<H; i++)
		{
			tmpt += cvGetReal2D(RBWNum,i,j);
		}
		SumCol[j] = tmpt;
	}
	double idx = 0;
	double num = 0;
	int flag = 0;
	int left = 0;
	int NW = 0;
	int Case = 0;
	int Sign = 1;

	//CString nnn="";

	for (j=0; j<W; j++)
	{
		if ((SumCol[j] == 0 && flag == 0) || (j==W-1))
		{
			NW = j-left;
			IplImage* NumImg = cvCreateImage(cvSize(NW,H),RBWNum->depth,RBWNum->nChannels);
			cvSetImageROI(RBWNum,cvRect(left,0,NW,H));
			cvCopy(RBWNum,NumImg);
			cvResetImageROI(RBWNum);

			Case = NumRecognizeK(NumImg,idx,num);
			switch (Case)
			{
			case -1:
				Sign = -1;
				break;
			case 0:
				break;
			case 1:
				NumR = NumR*pow(10,idx) + num;
				idx++;
				break;
			}
			flag = 1;
		}
		if (SumCol[j] != 0 && flag == 1)
		{
			left = j;
			flag = 0;
		}
	}
	NumR = Sign * NumR;
	TmpNum = NumR;
}

int CMUTDlg::NumRecognizeK(IplImage* NumImg, double& idx, double& Num)
{
	CvMat Matimg;
	int Case = 0;
	int NZO = cvCountNonZero(NumImg);
	if (NZO <= 5 && idx != 0)
		return Case = 0;

	Num = 0;
	int i, j;
	int W = NumImg->width, H=NumImg->height;
	int flag=0;
	for (i=0; i<H; i++)
	{
		int tmpt = 0;
		for (j=0; j<W; j++)
		{
			tmpt += cvGetReal2D(NumImg,i,j);
		}
		if (tmpt > 0)
			flag++;
	}
	if (flag <= 4 && idx ==0)
		return Case = -1;

	IplImage* NNumImg;

	NNumImg = cvCreateImage(cvGetSize(NumImg),NumImg->depth,1);
	cvCopy(NumImg,NNumImg);

	IplImage* result = cvCreateImage(cvSize(9-NNumImg->width+1,12-NNumImg->height+1),32,1);
	double tmin[10], tmax[10];
	for (i=0; i<10; i++)
	{
		tmin[i] = 0;
		tmax[i] = 0;
	}
	CvPoint Lmin, Lmax;
	int IdxMin=0, IdxMax=0;
	if (W < 8)
	{
		if (idx == 0)
		{
			for (i=0; i<10; i++)
			{
				cvMatchTemplate(NumTemplMK[i],NNumImg,result,3);
				cvMinMaxLoc(result,&tmin[i],&tmax[i],&Lmin,&Lmax);
			}
		}
		else
		{
			for (i=0; i<10; i++)
			{
				cvMatchTemplate(NumTemplMK[i],NNumImg,result,3);
				cvMinMaxLoc(result,&tmin[i],&tmax[i],&Lmin,&Lmax);
			}
		}
		MinMaxIdx(tmax,10,IdxMin,IdxMax);
		Num = IdxMax;
		idx = 1;
	}
	else
	{
		int SW[2],left[2];
		SW[0] = W/2; SW[1] = W - SW[0];
		left[0] = 0; left[1]=SW[0];
		CString SWnn= _T("");
		for (i=0; i<2; i++)
		{
			NNumImg = cvCreateImage(cvSize(SW[i],12),NumImg->depth,NumImg->nChannels);
			cvZero(NNumImg);
			cvSetImageROI(NNumImg,cvRect(0,(12-H)/2,SW[i],H));
			cvSetImageROI(NumImg,cvRect(left[i],0,SW[i],H));
			cvCopy(NumImg,NNumImg);
			cvResetImageROI(NNumImg);
			cvResetImageROI(NumImg);
			result = cvCreateImage(cvSize(NNumImg->width-9+1,NNumImg->height-12+1),32,1);
			if (idx == 0)
			{
				for (j=1; j<10; j++)
				{
					cvMatchTemplate(NNumImg,NumTemplMK[j],result,3);
					cvMinMaxLoc(result,&tmin[j],&tmax[j],&Lmin,&Lmax);
				}
				idx = 1;
			}
			else
			{
				for (j=0; j<10; j++)
				{
					cvMatchTemplate(NNumImg,NumTemplMK[j],result,3);
					cvMinMaxLoc(result,&tmin[j],&tmax[j],&Lmin,&Lmax);
				}
			}
			MinMaxIdx(tmax,10,IdxMin,IdxMax);
			Num = Num*pow(10,(double)i) + IdxMax;			
		}
		idx = 2;
	}
	return Case = 1;
}
