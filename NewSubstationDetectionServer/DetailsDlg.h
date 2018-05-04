#pragma once

#include <string>
#include "afxwin.h"
#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"

using namespace std; 
using namespace cv; 
// CDetailsDlg �Ի���


struct transData 
{  
	CString deviceType;
	CString position;
	CString abnormalDimension;
    CString areaMaxTemp;
	CString areaAvgTemp;
	CString enviTemperature;
	CString diffTemp;
	CString speed;
	CString abnormalType;
	CString Time;

};  

class CDetailsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDetailsDlg)

public:
	CDetailsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDetailsDlg();

	struct transData data; 

// �Ի�������
	enum { IDD = IDD_detailsDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_pic_1;
	CStatic m_pic_2;
	CStatic m_pic_3;
	CStatic m_pic_4;
	CStatic m_deviceType;
	CStatic m_abnormalCenter;
	CStatic m_abnormalDimension;
	CStatic m_areaMaxTemp;
	CStatic m_areaAvgTemp;
	CStatic m_enviTemp;
	CStatic m_diffTemp;
	CStatic m_tempSpeed;
	CStatic m_abnormalType;
	CStatic m_detectTime;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void DrawPicToHDC(IplImage *img, UINT ID);
	afx_msg void OnBnClickedbtnabnormalanalysis();

	IplImage* image;
};
