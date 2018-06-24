// UVSRDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "PowerSystem.h"
#include "UVSRDlg.h"

BOOL UVFlag = 0;
BOOL IRFlag = 1;
int ScreenWidth = 1280;
int ScreenHeight = 768;
IplImage* UVsrc;
IplImage* IRsrc;

IplImage* CUVSRDlg::UVsrcSR_scaling = NULL;
// CUVSRDlg 对话框

IMPLEMENT_DYNAMIC(CUVSRDlg, CDialog)

CUVSRDlg::CUVSRDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUVSRDlg::IDD, pParent)
{

}

CUVSRDlg::~CUVSRDlg()
{
}

void CUVSRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UVSRCSR_FRAME, m_Frame);
	DDX_Control(pDX, IDC_UVSRCSR_IMAGE, m_Image);
}


BEGIN_MESSAGE_MAP(CUVSRDlg, CDialog)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CUVSRDlg 消息处理程序

BOOL CUVSRDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//************临时读取固定的图片***************//
	UVsrc = cvLoadImage("1.jpg");
	IRsrc = cvLoadImage("1.jpg");

	// TODO:  在此添加额外的初始化
	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	CvRect MainDialogWindow = {0,0,ScreenWidth,ScreenHeight};
	::SetWindowPos(this->m_hWnd,HWND_BOTTOM,MainDialogWindow.x,MainDialogWindow.y,MainDialogWindow.width,MainDialogWindow.height,SWP_NOZORDER);
	CenterWindow();
	GetDlgItem(IDC_UVSRCSR_FRAME)->MoveWindow(0,0,ScreenWidth-10,ScreenHeight-30);//设定位置大小

	m_ImageContainer.Create(IDD_UVIMAGECONTAINER_DIALOG,this);
	CRect framerect_src;
	m_Frame.GetClientRect(framerect_src);
	m_ImageContainer.SetParent(this);
	m_Frame.MapWindowPoints(this,framerect_src);
	m_ImageContainer.MoveWindow(framerect_src);
	m_ImageContainer.ShowWindow(SW_SHOW);

	if (UVFlag)
	{
		UVsrcSR = cvCreateImage(cvGetSize(UVsrc), UVsrc->depth, UVsrc->nChannels);
		UVsrcSR_scaling = cvCreateImage(cvGetSize(UVsrc), UVsrc->depth, UVsrc->nChannels);
		cvCopy(UVsrc,UVsrcSR);
		cvCopy(UVsrc,UVsrcSR_scaling);
	}
	
	if (IRFlag)
	{
		UVsrcSR = cvCreateImage(cvGetSize(IRsrc), IRsrc->depth, IRsrc->nChannels);
		UVsrcSR_scaling = cvCreateImage(cvGetSize(IRsrc), IRsrc->depth, IRsrc->nChannels);
		cvCopy(IRsrc,UVsrcSR);
		cvCopy(IRsrc,UVsrcSR_scaling);
	}

	m_Image.SetParent(&m_ImageContainer);
	CRect bmprect_src;
	m_Image.GetClientRect(bmprect_src);
	//m_Image.MoveWindow(0,0,bmprect_src.Width(),bmprect_src.Height());
	m_Image.MoveWindow((framerect_src.Width()-UVsrcSR->width)/2,(framerect_src.Height()-UVsrcSR->height)/2,UVsrcSR->width,UVsrcSR->height);


	m_width=UVsrcSR->width;
	m_height=UVsrcSR->height;
	channel=UVsrcSR->nChannels;
	CBitmap* m_pBitmap = new CBitmap();  
	IplImageToCBitmap(UVsrcSR_scaling, &m_pBitmap);  
	m_Image.SetBitmap((HBITMAP)(*m_pBitmap));  

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CUVSRDlg::IplImageToCBitmap(IplImage* pImg, CBitmap** pBitmap)   
{  
	if (*pBitmap)  
	{  
		delete *pBitmap;  
		*pBitmap = NULL;  
		*pBitmap = new CBitmap();  
	}  
	DEVMODE stDevmode;  
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &stDevmode);  
	IplImage* pTmpImg = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U,stDevmode.dmBitsPerPel / IPL_DEPTH_8U);  
	cvSetZero(pTmpImg);  
	for (int i=0; i<pImg->height; i++)  
	{  
		for (int j=0; j<pImg->width; j++)  
		{  
			int nIndex = j;  
			CvScalar scaler = cvGetAt(pImg, i, j);  
			cvSetAt(pTmpImg, scaler, i, j);  
		}  
	}  
	int nLen = pTmpImg->imageSize;  
	BYTE* pBytes = new BYTE[nLen];  
	memcpy(pBytes, pTmpImg->imageData, nLen);  
	(*pBitmap)->CreateBitmap(pTmpImg->width, pTmpImg->height, 1, pTmpImg->nChannels*pTmpImg->depth, pBytes);  
	delete [] pBytes; 
	pBytes = NULL;  
}

BOOL CUVSRDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_ImageContainer.OnMouseWheel(nFlags,zDelta,pt);
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
