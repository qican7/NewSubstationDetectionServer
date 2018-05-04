
// NewSubstationDetectionServerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CNewSubstationDetectionServerDlg �Ի���
class CNewSubstationDetectionServerDlg : public CDialogEx
{
// ����
public:
	CNewSubstationDetectionServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_NEWSUBSTATIONDETECTIONSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
