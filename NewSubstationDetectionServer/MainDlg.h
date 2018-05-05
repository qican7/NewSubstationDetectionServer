#pragma once
#include "afxcmn.h"

#include <string>
#include "afxwin.h"

#include "cv.h"
#include "highgui.h"

using namespace std; 
using namespace cv; 


// CMainDlg �Ի���

class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainDlg();

// �Ի�������
	enum { IDD = IDD_main };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
	virtual void insertIntoList(CString deviceType,int x,int y,int AbnormalDimension,float areaMaxTemp,float areaAvgTemp,CString referTemp,CString enviTemp, float referDiffTemp,float speed,int abnormalLevel,CString time);
	CStatic m_infrare;
	CStatic m_light;
	virtual void DrawPicToHDC(IplImage *img, UINT ID);
	afx_msg void OnPaint();
	afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnClose();
	afx_msg void OnDestroy();
};
