// SuperResolutionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NewSubstationDetectionServer.h"
#include "SuperResolutionDlg.h"
#include "afxdialogex.h"


// CSuperResolutionDlg �Ի���

IMPLEMENT_DYNAMIC(CSuperResolutionDlg, CDialogEx)

CSuperResolutionDlg::CSuperResolutionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSuperResolutionDlg::IDD, pParent)
{

}

CSuperResolutionDlg::~CSuperResolutionDlg()
{
}

void CSuperResolutionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_superResolution_pic, m_superResolutionPic);
}


BEGIN_MESSAGE_MAP(CSuperResolutionDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CSuperResolutionDlg ��Ϣ�������

void CSuperResolutionDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	DrawPicToHDC(data.image, IDC_superResolution_pic);
}

//����Ƶ֡��ʾ��pictureControl
void CSuperResolutionDlg::DrawPicToHDC(IplImage *img, UINT ID)
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



