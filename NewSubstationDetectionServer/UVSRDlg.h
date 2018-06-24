#include "cv.h"
#include "highgui.h"
#include "ImageContainer.h"
#pragma once

#include "resource.h"

// CUVSRDlg 对话框

class CUVSRDlg : public CDialog
{
	DECLARE_DYNAMIC(CUVSRDlg)

public:
	CUVSRDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUVSRDlg();


// 对话框数据
	enum { IDD = IDD_UVSR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
