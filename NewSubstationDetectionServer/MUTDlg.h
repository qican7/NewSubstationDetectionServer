#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"
#include "afxcmn.h"
#include "resource.h"
using namespace std;
#pragma once


// CMUTDlg �Ի���

class CMUTDlg : public CDialog
{
	DECLARE_DYNAMIC(CMUTDlg)

public:
	CMUTDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMUTDlg();

// �Ի�������
	enum { IDD = IDD_MULIMG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual void PreInitDialog();
public:
	virtual BOOL OnInitDialog();
public:
	CFont MUT_Font_Caption;
	CFont MUT_Font_Button;
	CPoint MousePtM;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedLoadimg1();
	afx_msg void OnBnClickedLoadimg2();
	afx_msg void OnBnClickedLoadimg3();
	void ClimgCvtTpimg(IplImage* PreTmp, IplImage* GrayImgr, IplImage* TmpImg, double& MaxTmp, double& MinTmp, double ColorBar[][3], int size2);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();
};
