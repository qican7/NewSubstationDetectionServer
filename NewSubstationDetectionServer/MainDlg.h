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
	virtual void insertIntoList(CString pos, int x,int y,int abnormalDimension,float areaMaxTemp,float areaAvgTemp,float referTemp,float enviTemp,float diffTemp,string deviceType,int abnormalLevel,int year,int month,int day,int hour,int minute,int second);
	CStatic m_infrare;
	CStatic m_light;
	virtual void DrawPicToHDC(IplImage *img, UINT ID);
	afx_msg void OnPaint();
	afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnClose();
	afx_msg void OnDestroy();
};
