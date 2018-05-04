
// NewSubstationDetectionServerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CNewSubstationDetectionServerDlg 对话框
class CNewSubstationDetectionServerDlg : public CDialogEx
{
// 构造
public:
	CNewSubstationDetectionServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_NEWSUBSTATIONDETECTIONSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	CComboBox m_deviceName;
	CComboBox m_failureLevel;
	CComboBox m_position;
	CComboBox m_startYear;
	CComboBox m_startMonth;
	CComboBox m_startDay;
	CComboBox m_endYear;
	CComboBox m_endMonth;
	CComboBox m_endDay;
};
