#pragma once
#include "afxwin.h"

#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"

using namespace std; 
using namespace cv; 


// CSuperResolutionDlg 对话框

struct transDataSuperResolution  //传输的数据
{  
	IplImage* image;

}; 

class CSuperResolutionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSuperResolutionDlg)

public:
	CSuperResolutionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSuperResolutionDlg();

// 对话框数据
	enum { IDD = IDD_superResolutionDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_superResolutionPic;
	transDataSuperResolution data;
	afx_msg void DrawPicToHDC(IplImage *img, UINT ID);
	afx_msg void OnPaint();
};
