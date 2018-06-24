#include "cv.h"
#include "highgui.h"
#include "ImageContainer.h"
#pragma once

#include "resource.h"

// CUVSRDlg �Ի���

class CUVSRDlg : public CDialog
{
	DECLARE_DYNAMIC(CUVSRDlg)

public:
	CUVSRDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUVSRDlg();


// �Ի�������
	enum { IDD = IDD_UVSR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_Frame;
	CStatic m_Image;
	CImageContainer	m_ImageContainer;
	IplImage* UVsrcSR;
	static IplImage* UVsrcSR_scaling;
	int m_width,m_height,channel;
	static void IplImageToCBitmap(IplImage* pImg, CBitmap** pBitmap);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
