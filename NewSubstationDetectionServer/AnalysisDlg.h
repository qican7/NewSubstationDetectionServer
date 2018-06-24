#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "stdio.h"
#include "math.h"

#include "UVSRDlg.h"

using namespace std; 
using namespace cv; 


// CAnalysisDlg 对话框

struct transDataFromDetails 
{  
  IplImage* image;

}; 

class CAnalysisDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAnalysisDlg)

public:
	CAnalysisDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAnalysisDlg();

	CUVSRDlg* DialogIRSR;

// 对话框数据
	enum { IDD = IDD_abPicAnalysisDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CStatic m_analysis_pic;
	CSliderCtrl m_analysis_slider;
	CComboBox m_analysis_choiceDeviceType;
	CEdit m_analysis_choiceReferTemp;
	afx_msg void OnBnClickedanalysisbtn();
	CListCtrl m_analysis_list;

	transDataFromDetails data;


	

	virtual void DrawPicToHDC(IplImage *img, UINT ID);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedanalysisbtnchoicearea();

	virtual void detectAbnormalAreaAndDrawToPicControl(); //检测异常并将显示到图像控件中
	virtual bool checkPixelTemp(IplImage* &src, int x, int y, int value,int space); //检查像素是否为边缘像素
	virtual void detectAbnormalArea(IplImage* colorImage_ori); //检测和统计异常区域并将异常信息插入到列表中
	virtual void findArea(IplImage* colorImage,IplImage* grayImage,int index,int AbnormalCount,int x,int y);

	virtual void insertIntoList(CString deviceType, int x,int y,float abnormalDimension,float areaMaxTemp,float enviTemp,float referTemp,float diffTemp,int abnormalLevel);

	CStatic m_enviTemp;
};
