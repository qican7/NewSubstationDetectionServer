#include "cv.h"
#include "highgui.h"
#include   "resource.h"

#pragma once


// CIRReportDlg 对话框

class CIRReportDlg : public CDialog
{
	DECLARE_DYNAMIC(CIRReportDlg)

public:
	CIRReportDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIRReportDlg();

// 对话框数据
	enum { IDD = IDD_IRREPORT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CFont IF_Font_Caption;
	CFont IF_Font_Button;

	CString m_IRStationName;
	CString m_IRRunNum;
	CString m_IRClient;
	CString m_IRTestAgn;
	CString m_IRPilot;
	CString m_IRTestDate;
	CString m_IRTester;
	CString m_IRTestSpot;
	CString m_IRReportDate;
	CString m_IRReporter;
	CString m_IRAuditor;
	CString m_IRApprover;

	CString m_IRManufacturer;
	CString m_IROperateDate;
	CString m_IRUintType;
	CString m_IRDetectSite;
	CString m_IRWeather;
	CString m_IRWindSpeed;
	CString m_IRTestDis;
	CString m_IRRadiation;
	CString m_IRLoadCurrent;
	CString m_IRRatedCurrent;
	CString m_IRTemperature;
	CString m_IRHumidity;

	CString m_IRUnitExpress;
	CString m_IRInstrument;
	CString m_IRFaultExpress;
	CBrush m_IR_REPORT_brBk;
	void GetEditText();
	afx_msg void OnBnClickedPreview();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButton1();
};
