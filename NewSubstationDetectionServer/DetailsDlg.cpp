// DetailsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NewSubstationDetectionServer.h"
#include "DetailsDlg.h"
#include "afxdialogex.h"
#include "AnalysisDlg.h"


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
END_MESSAGE_MAP()


// CDetailsDlg 消息处理程序


BOOL CDetailsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

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



	 ((CWnd*)GetDlgItem(IDC_deviceType))->SetWindowText(data.deviceType);
	 ((CWnd*)GetDlgItem(IDC_abnormalCenter))->SetWindowText(data.position);
	 ((CWnd*)GetDlgItem(IDC_abnormalDimension))->SetWindowText(data.abnormalDimension);
	 ((CWnd*)GetDlgItem(IDC_areaMaxTemp))->SetWindowText(data.areaMaxTemp);
	 ((CWnd*)GetDlgItem(IDC_areaAvgTemp))->SetWindowText(data.areaAvgTemp);
	 ((CWnd*)GetDlgItem(IDC_enviTemp))->SetWindowText(data.enviTemperature);
	 ((CWnd*)GetDlgItem(IDC_diffTemp))->SetWindowText(data.diffTemp);
	 ((CWnd*)GetDlgItem(IDC_tempSpeed))->SetWindowText(data.speed);
	 ((CWnd*)GetDlgItem(IDC_abnormalType))->SetWindowText(data.abnormalType);
	 ((CWnd*)GetDlgItem(IDC_detectTime))->SetWindowText(data.Time);

}

//将视频帧显示到pictureControl
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
