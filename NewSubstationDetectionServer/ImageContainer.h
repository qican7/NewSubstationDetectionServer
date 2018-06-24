#if !defined(AFX_IMAGECONTAINER_H__9D63FDCE_B2C6_4A2E_9260_804A27677C4E__INCLUDED_)
#define AFX_IMAGECONTAINER_H__9D63FDCE_B2C6_4A2E_9260_804A27677C4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageContainer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageContainer dialog

#include "resource.h"

class CImageContainer : public CDialog
{
// Construction
public:
	CImageContainer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImageContainer)
	enum { IDD = IDD_UVIMAGECONTAINER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageContainer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CImageContainer)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGECONTAINER_H__9D63FDCE_B2C6_4A2E_9260_804A27677C4E__INCLUDED_)
