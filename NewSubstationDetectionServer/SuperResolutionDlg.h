#pragma once
#include "afxwin.h"

#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"

using namespace std; 
using namespace cv; 


// CSuperResolutionDlg �Ի���

struct transDataSuperResolution  //���������
{  
	IplImage* image;

}; 

class CSuperResolutionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSuperResolutionDlg)

public:
	CSuperResolutionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSuperResolutionDlg();

// �Ի�������
	enum { IDD = IDD_superResolutionDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_superResolutionPic;
	transDataSuperResolution data;
	afx_msg void DrawPicToHDC(IplImage *img, UINT ID);
	afx_msg void OnPaint();
};
