// DetailsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NewSubstationDetectionServer.h"
#include "DetailsDlg.h"
#include "afxdialogex.h"
#include "AnalysisDlg.h"
#include "SuperResolutionDlg.h"

int picControl_start_x = 0, picControl_start_y = 0, picControl_end_x = 0, picControl_end_y = 0;
int picControl_height = 0, picControl_width = 0;



// CDetailsDlg 对话框

IMPLEMENT_DYNAMIC(CDetailsDlg, CDialogEx)

CDetailsDlg::CDetailsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDetailsDlg::IDD, pParent)
{

}

CDetailsDlg::~CDetailsDlg()
{
}

void CDetailsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_pic_1, m_pic_1);
	DDX_Control(pDX, IDC_pic_2, m_pic_2);
	DDX_Control(pDX, IDC_pic_3, m_pic_3);
	DDX_Control(pDX, IDC_pic_4, m_pic_4);
	DDX_Control(pDX, IDC_deviceType, m_deviceType);
	DDX_Control(pDX, IDC_abnormalCenter, m_abnormalCenter);
	DDX_Control(pDX, IDC_abnormalDimension, m_abnormalDimension);
	DDX_Control(pDX, IDC_areaMaxTemp, m_areaMaxTemp);
	DDX_Control(pDX, IDC_areaAvgTemp, m_areaAvgTemp);
	DDX_Control(pDX, IDC_enviTemp, m_enviTemp);
	DDX_Control(pDX, IDC_diffTemp, m_diffTemp);
	DDX_Control(pDX, IDC_tempSpeed, m_tempSpeed);
	DDX_Control(pDX, IDC_abnormalType, m_abnormalType);
	DDX_Control(pDX, IDC_detectTime, m_detectTime);



}


BEGIN_MESSAGE_MAP(CDetailsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_btn_abnormalAnalysis, &CDetailsDlg::OnBnClickedbtnabnormalanalysis)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CDetailsDlg 消息处理程序


BOOL CDetailsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//图像缩放参数初始化
	//获取控件相对于屏幕的位置
	GetDlgItem(IDC_pic_1)->GetWindowRect(&rectL);
	ScreenToClient(rectL);//转化为对话框上的相对位置
	picControl_start_x = rectL.left;
	picControl_start_y = rectL.top;
	picControl_end_x = rectL.right;
	picControl_end_y = rectL.bottom;

	//获取pictureControl的尺寸
	CRect rect_1;
	CWnd *pWnd = GetDlgItem(IDC_pic_1);	//IDC_picture为picture控件ID
	pWnd->GetClientRect(&rect_1);		//rect为控件的大小
	picControl_height = rect_1.Height();
	picControl_width = rect_1.Width();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDetailsDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	image = cvLoadImage("test.jpg");
	DrawPicToHDC(image, IDC_pic_1);
	DrawPicToHDC(image, IDC_pic_2);
	DrawPicToHDC(image, IDC_pic_3);
	DrawPicToHDC(image, IDC_pic_4);

	image->width;



	 //((CWnd*)GetDlgItem(IDC_deviceType))->SetWindowText(data.deviceType);
	 //((CWnd*)GetDlgItem(IDC_abnormalCenter))->SetWindowText(data.position);
	 //((CWnd*)GetDlgItem(IDC_abnormalDimension))->SetWindowText(data.abnormalDimension);
	 //((CWnd*)GetDlgItem(IDC_areaMaxTemp))->SetWindowText(data.areaMaxTemp);
	 //((CWnd*)GetDlgItem(IDC_areaAvgTemp))->SetWindowText(data.areaAvgTemp);
	 //((CWnd*)GetDlgItem(IDC_enviTemp))->SetWindowText(data.enviTemperature);
	 //((CWnd*)GetDlgItem(IDC_diffTemp))->SetWindowText(data.diffTemp);
	 //((CWnd*)GetDlgItem(IDC_tempSpeed))->SetWindowText(data.speed);
	 //((CWnd*)GetDlgItem(IDC_abnormalType))->SetWindowText(data.abnormalType);
	 //((CWnd*)GetDlgItem(IDC_detectTime))->SetWindowText(data.Time);

}

//将图像显示到pictureControl
void CDetailsDlg::DrawPicToHDC(IplImage *img, UINT ID)
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

//异常分析按钮点击事件
void CDetailsDlg::OnBnClickedbtnabnormalanalysis()
{
	// TODO: 在此添加控件通知处理程序代码

	CAnalysisDlg dlg;
	dlg.data.image = image;
	dlg.DoModal();

}


// 鼠标左键点击事件，放大红外图像
void CDetailsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnLButtonDown(nFlags, point);

	if (point.x - picControl_start_x > 0 && point.y - picControl_start_y > 0 && point.x - picControl_end_x < 0 && point.y - picControl_end_y < 0)
	{
		CSuperResolutionDlg dlg;
		dlg.data.image = image;
		dlg.DoModal();

	}
}
