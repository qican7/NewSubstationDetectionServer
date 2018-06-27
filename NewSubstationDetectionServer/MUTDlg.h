#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"
#include "afxcmn.h"
#include "resource.h"
using namespace std;
#pragma once


// CMUTDlg 对话框

class CMUTDlg : public CDialog
{
	DECLARE_DYNAMIC(CMUTDlg)

public:
	CMUTDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMUTDlg();

// 对话框数据
	enum { IDD = IDD_MULIMG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void PreInitDialog();
public:
	virtual BOOL OnInitDialog();
public:
	CFont MUT_Font_Caption;
	CFont MUT_Font_Button;
	CListCtrl MTListCtrl;
	//IplImage* MUTIMG1;
	//IplImage* MUTIMG2;
	//IplImage* MUTIMG3;
	//IplImage* TMPIMG1;
	//IplImage* TMPIMG2;
	//IplImage* TMPIMG3;
	//CString SMinT1;
	//CString SMaxT1;
	//CString SMinT2;
	//CString SMaxT2;
	//CString SMinT3;
	//CString SMaxT3;
	CPoint MousePtM;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedLoadimg1();
	afx_msg void OnBnClickedLoadimg2();
	afx_msg void OnBnClickedLoadimg3();
	void ClimgCvtTpimg(IplImage* PreTmp, IplImage* GrayImgr, IplImage* TmpImg, double& MaxTmp, double& MinTmp, double ColorBar[][3], int size2);
	void ImageSeparate(IplImage* SrcImg, IplImage* PreTmp, int MMHW[], int size1, double& MaxTmp, double& MinTmp, double ColorBar[][3], int size2);
	CvRect MinRect(IplImage* src);
	void MinMaxIdx(double arr[], int num, int &IdxMin, int &IdxMax);
	void InverseMap(IplImage* src, IplImage* dst, double& MaxT, double& MinT);
	void MinMaxRecognize(IplImage* MaxImgBW, IplImage* MinImgBW, double& MaxT, double& MinT);
	void StringRecognize(IplImage* StringImg, double& TmpNum);
	int NumRecognize(IplImage* NumImg, double &idx, double &Num);
	void PseColor2Gray(IplImage* PseImg, IplImage* GrayImgr, double ColorBar[][3], int size);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedConform1();
	afx_msg void OnBnClickedConform2();
	afx_msg void OnBnClickedConform3();
	void ImageSeparateK(IplImage* SrcImg, IplImage* PreTmp, IplImage* ScaleImg, double& MaxTmp, double& MinTmp, double ColorBar[][3], int size2);
	void MinMaxRecognizeK(IplImage* MaxImgBW, IplImage* MinImgBW, double& MaxT, double& MinT);
	void StringRecognizeK(IplImage* StringImg, double& TmpNum);
	int NumRecognizeK(IplImage* NumImg, double& idx, double& Num);
};
