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
	DDX_Control(pDX, IDC_IR_PROGRESS1, m_IR_Progress1);
}


BEGIN_MESSAGE_MAP(CMUTDlg, CDialog)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_LOADIMG1, &CMUTDlg::OnBnClickedLoadimg1)
	ON_BN_CLICKED(IDC_LOADIMG2, &CMUTDlg::OnBnClickedLoadimg2)
	ON_BN_CLICKED(IDC_LOADIMG3, &CMUTDlg::OnBnClickedLoadimg3)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CMUTDlg::OnBnClickedButton1)

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
	GetDlgItem(IDC_IMG2)->MoveWindow(490,20,400,300);
	GetDlgItem(IDC_IMG3)->MoveWindow(960,20,400,300);


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
					//MTListCtrl.DeleteItem(0);
					Lx.Format(_T("%d"),RealP.x);
					Ly.Format(_T("%d"),RealP.y);
					Lc = _T('(') + Lx + _T(',') + Ly + _T(')');

					double tmptmp = cvGetReal2D(TMPIMG1,RealP.y,RealP.x);
					SStmp.Format(_T("%.2f"),tmptmp);
					//MTListCtrl.InsertItem(0,_T("图1"));
					//MTListCtrl.SetItemText(0,1,Lc);
					//MTListCtrl.SetItemText(0,2,SStmp);
				}
			}
			else if (ImgType1 == 2)
			{
				//MTListCtrl.DeleteItem(0);
				Lx.Format(_T("%d"),RealP.x);
				Ly.Format(_T("%d"),RealP.y);
				Lc = _T('(') + Lx + _T(',') + Ly + _T(')');

				double tmptmp = cvGetReal2D(TMPIMG1,RealP.y,RealP.x);
				SStmp.Format(_T("%.2f"),tmptmp);
				//MTListCtrl.InsertItem(0,_T("图1"));
				//MTListCtrl.SetItemText(0,1,Lc);
				//MTListCtrl.SetItemText(0,2,SStmp);
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
					//MTListCtrl.DeleteItem(1);
					Lx.Format(_T("%d"),RealP.x);
					Ly.Format(_T("%d"),RealP.y);
					Lc = _T('(') + Lx + _T(',') + Ly + _T(')');

					double tmptmp = cvGetReal2D(TMPIMG2,RealP.y,RealP.x);
					SStmp.Format(_T("%.2f"),tmptmp);
					//MTListCtrl.InsertItem(1,_T("图2"));
					//MTListCtrl.SetItemText(1,1,Lc);
					//MTListCtrl.SetItemText(1,2,SStmp);
				}
			}
			else if (ImgType2 == 2)
			{
				//MTListCtrl.DeleteItem(1);
				Lx.Format(_T("%d"),RealP.x);
				Ly.Format(_T("%d"),RealP.y);
				Lc = _T('(') + Lx + _T(',') + Ly + _T(')');

				double tmptmp = cvGetReal2D(TMPIMG2,RealP.y,RealP.x);
				SStmp.Format(_T("%.2f"),tmptmp);
				//MTListCtrl.InsertItem(1,_T("图2"));
				//MTListCtrl.SetItemText(1,1,Lc);
				//MTListCtrl.SetItemText(1,2,SStmp);
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
					//MTListCtrl.DeleteItem(2);
					Lx.Format(_T("%d"),RealP.x);
					Ly.Format(_T("%d"),RealP.y);
					Lc = _T('(') + Lx + _T(',') + Ly + _T(')');

					double tmptmp = cvGetReal2D(TMPIMG3,RealP.y,RealP.x);
					SStmp.Format(_T("%.2f"),tmptmp);
					//MTListCtrl.InsertItem(2,_T("图3"));
					//MTListCtrl.SetItemText(2,1,Lc);
					//MTListCtrl.SetItemText(2,2,SStmp);
				}
			}
			else if (ImgType3 == 2)
			{
				//MTListCtrl.DeleteItem(2);
				Lx.Format(_T("%d"),RealP.x);
				Ly.Format(_T("%d"),RealP.y);
				Lc = _T('(') + Lx + _T(',') + Ly + _T(')');

				double tmptmp = cvGetReal2D(TMPIMG3,RealP.y,RealP.x);
				SStmp.Format(_T("%.2f"),tmptmp);
				//MTListCtrl.InsertItem(2,_T("图3"));
				//MTListCtrl.SetItemText(2,1,Lc);
				//MTListCtrl.SetItemText(2,2,SStmp);
			}
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CMUTDlg::OnBnClickedLoadimg1()
{
	// TODO: 在此添加控件通知处理程序代码
	//GetDlgItem(IDC_FLUKEBAR_FRAME1)->ShowWindow(SW_HIDE);
	CString filepath;
	CString szFilters = _T("image files (*.bmp; *.jpg) |*.bmp;*.jpg|All Files (*.*)|*.*||");
	CFileDialog fileDlg(TRUE,_T("*.bmp"),_T(""),OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,szFilters,NULL);
	char title[] = {"打开图像"};
	fileDlg.m_ofn.lpstrTitle = stringToCString(title);
	if (fileDlg.DoModal() == IDOK)
	{
		filepath = fileDlg.GetPathName();
		MUTIMG1 = cvLoadImage(CStringToChar(filepath));
		ImgRS1 = cvCreateImage(cvSize(320,240),MUTIMG1->depth,MUTIMG1->nChannels);
		int OW = MUTIMG1->width, OH = MUTIMG1->height;
		if (OW == 320 && OH == 240)
		{
			ImgType1 = 1;
			cvResize(MUTIMG1,ImgRS1,CV_INTER_CUBIC);
			MutCvvImage.CopyOf(ImgRS1,1);
			MutCvvImage.DrawToHDC(hDC_img1,&rect_img1);
		}
		else if (OW == 392 && OH == 240)
		{
			//GetDlgItem(IDC_FLUKEBAR_FRAME1)->ShowWindow(SW_SHOW);
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
		ImgRS2 = cvCreateImage(cvSize(320,240),MUTIMG2->depth,MUTIMG2->nChannels);
		int OW = MUTIMG2->width, OH = MUTIMG2->height;
		if (OW == 320 && OH == 240)
		{
			ImgType2 = 1;
			cvResize(MUTIMG2,ImgRS2,CV_INTER_CUBIC);
			MutCvvImage.CopyOf(ImgRS2,1);
			MutCvvImage.DrawToHDC(hDC_img2,&rect_img2);
		}
		else if (OW == 392 && OH == 240)
		{
			//GetDlgItem(IDC_FLUKEBAR_FRAME2)->ShowWindow(SW_SHOW);
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
		ImgRS3 = cvCreateImage(cvSize(320,240),MUTIMG3->depth,MUTIMG3->nChannels);
		int OW = MUTIMG3->width, OH = MUTIMG3->height;
		if (OW == 320 && OH == 240)
		{
			ImgType3 = 1;
			cvResize(MUTIMG3,ImgRS3,CV_INTER_CUBIC);
			MutCvvImage.CopyOf(ImgRS3,1);
			MutCvvImage.DrawToHDC(hDC_img3,&rect_img3);
		}
		else if (OW == 392 && OH == 240)
		{
			//GetDlgItem(IDC_FLUKEBAR_FRAME3)->ShowWindow(SW_SHOW);
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


}

void CMUTDlg::ClimgCvtTpimg(IplImage* PreTmp, IplImage* GrayImgr, IplImage* TmpImg, double& MaxTmp, double& MinTmp, double ColorBar[][3], int size2)
{
	//ImageSeparate(ColImg,PreTmp,MMHW,4,MaxTmp,MinTmp,ColorBar,256);
	//PseColor2Gray(PreTmp,GrayImgr,ColorBar,256);
	cvNormalize(GrayImgr,TmpImg,MaxTmp,MinTmp,CV_MINMAX);
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










