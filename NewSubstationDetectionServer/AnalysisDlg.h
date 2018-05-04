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

using namespace std; 
using namespace cv; 


// CAnalysisDlg �Ի���

struct transDataFromDetails 
{  
  IplImage* image;

}; 

class CAnalysisDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAnalysisDlg)

public:
	CAnalysisDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAnalysisDlg();

// �Ի�������
	enum { IDD = IDD_abPicAnalysisDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

	virtual void detectAbnormalAreaAndDrawToPicControl(); //����쳣������ʾ��ͼ��ؼ���
	virtual bool checkPixelTemp(IplImage* &src, int x, int y, int value,int space); //��������Ƿ�Ϊ��Ե����
	virtual void detectAbnormalArea(IplImage* colorImage_ori); //����ͳ���쳣���򲢽��쳣��Ϣ���뵽�б���
	virtual void findArea(IplImage* colorImage,IplImage* grayImage,int index,int AbnormalCount,int x,int y);

	virtual void insertIntoList(CString deviceType, int x,int y,float abnormalDimension,float areaMaxTemp,float enviTemp,float referTemp,float diffTemp,int abnormalLevel);

	CStatic m_enviTemp;
};
