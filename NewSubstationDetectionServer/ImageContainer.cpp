// ImageContainer.cpp : implementation file
//

#include "stdafx.h"

//#include "PowerSystem.h"
#include "ImageContainer.h"
#include "UVSRDlg.h"
//#include "PowerSystemDlg.h"
#include "AnalysisDlg.h"

extern BOOL UVFlag;
extern BOOL IRFlag;
extern IplImage* UVsrc;
extern IplImage* IRsrc;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static double  m_zScaling = 1;
/////////////////////////////////////////////////////////////////////////////
// CImageContainer dialog


CImageContainer::CImageContainer(CWnd* pParent /*=NULL*/)
	: CDialog(CImageContainer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImageContainer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CImageContainer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageContainer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImageContainer, CDialog)
	//{{AFX_MSG_MAP(CImageContainer)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageContainer message handlers

void CImageContainer::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int pos,min,max,thumbwidth;
	SCROLLINFO vinfo;
	GetScrollInfo(SB_HORZ,&vinfo);	
	pos = vinfo.nPos;
	min = vinfo.nMin;
	max = vinfo.nMax;
	thumbwidth = vinfo.nPage;
	switch (nSBCode)
	{
		break;
	case SB_THUMBTRACK: //拖动滚动块
		ScrollWindow(-(nPos-pos),0);
		SetScrollPos(SB_HORZ,nPos);	
		break;
	case SB_LINELEFT : //单击左箭头
		SetScrollPos(SB_HORZ,pos-1);
		if (pos !=0)
		{
			ScrollWindow(1,0);
		}
		break;
	case SB_LINERIGHT: //单击右箭头
		SetScrollPos(SB_HORZ,pos+1);
		if (pos+thumbwidth <max)
		{
			ScrollWindow(-1,0);	
		}
		break;
	case SB_PAGELEFT: //在滚动块的左方空白滚动区域单击,增量为6
		
		SetScrollPos(SB_HORZ,pos-6);
		if (pos+thumbwidth >0)
		{
			ScrollWindow(6,0);
		}
		break;
	case SB_PAGERIGHT: //在滚动块的右方空白滚动区域单击,增量为6
		SetScrollPos(SB_HORZ,pos+6);
		if (pos+thumbwidth <max)
		{
			ScrollWindow(-6,0);
		}
		break;
	}	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CImageContainer::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int pos,min,max,thumbwidth;
	SCROLLINFO vinfo;
	GetScrollInfo(SB_VERT,&vinfo);
	pos = vinfo.nPos;
	min = vinfo.nMin;
	max = vinfo.nMax;
	thumbwidth = vinfo.nPage;
	switch (nSBCode)
	{
	case SB_THUMBTRACK:
		pos = GetScrollPos(SB_VERT);
		ScrollWindow(0,-(nPos-pos));
		SetScrollPos(SB_VERT,nPos);
		break;
		
	case SB_LINELEFT:
		pos = GetScrollPos(SB_VERT);
		SetScrollPos(SB_VERT,pos-1);
		if (pos !=0)
		{
			ScrollWindow(0,1);
		}
		break;
	case SB_LINERIGHT:
		pos = GetScrollPos(SB_VERT);
		SetScrollPos(SB_VERT,pos+1);
		if (pos+thumbwidth <max)
		{
			ScrollWindow(0,-1);	
		}
		break;
	case SB_PAGELEFT: //在滚动块的上方空白滚动区域单击,增量为6
		
		SetScrollPos(SB_VERT,pos-6);
		if (pos+thumbwidth >0)
		{
			ScrollWindow(0,6);
		}
		break;
	case SB_PAGERIGHT: //在滚动块的下方空白滚动区域单击,增量为6
		
		SetScrollPos(SB_VERT,pos+6);
		if (pos+thumbwidth < max)
		{
			ScrollWindow(0,-6);
		}
		break;
	}	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CImageContainer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if (CUVSRDlg::UVsrcSR_scaling != NULL)
	{
		//CPowerSystemDlg* pWnd=(CPowerSystemDlg*)AfxGetApp()->GetMainWnd();//这句话很关键
		CAnalysisDlg* pWnd=(CAnalysisDlg*)AfxGetApp()->GetMainWnd();//这句话很关键

		if (zDelta > 0) //放大图像
		{	
			//m_zScaling = m_zScaling + 0.1*zDelta/120;  
			m_zScaling = m_zScaling + 0.3;
			if (m_zScaling > 3)
			{
				m_zScaling = 3;
				//return FALSE;
			}
			int width = CUVSRDlg::UVsrcSR_scaling->width * m_zScaling;       
		    //目标图像的宽为源图象宽的m_zScaling倍    
			int height = CUVSRDlg::UVsrcSR_scaling->height * m_zScaling; 
			//目标图像的高为源图象高的m_zScaling倍  
			IplImage* dst = cvCreateImage(cvSize(width,height), CUVSRDlg::UVsrcSR_scaling->depth, CUVSRDlg::UVsrcSR_scaling->nChannels);    
		   // 构造目标图象    
			cvResize(CUVSRDlg::UVsrcSR_scaling, dst, CV_INTER_CUBIC);     
			//cvShowImage("dst",dst);
			//cvWaitKey(0);
		    //缩放源图像到目标图像
			CBitmap*m_pBitmap = new CBitmap();  

			CUVSRDlg::IplImageToCBitmap(dst, &m_pBitmap); 
			
			if (UVFlag)
			{
				/*				CRect framerect_src;
				pWnd->DialogUVIP->DialogUVSR->m_Frame.GetClientRect(framerect_src);
				pWnd->DialogUVIP->DialogUVSR->m_Image.SetParent(&(pWnd->DialogUVIP->DialogUVSR->m_ImageContainer));
				CRect bmprect_src;
				pWnd->DialogUVIP->DialogUVSR->m_Image.GetClientRect(bmprect_src);

				int width1 = CUVSRDlg::UVsrcSR_scaling->width;
				int height1 = CUVSRDlg::UVsrcSR_scaling->height;
				pWnd->DialogUVIP->DialogUVSR->m_Image.MoveWindow((framerect_src.Width()-dst->width)/2,(framerect_src.Height()-dst->height)/2,dst->width,dst->height);
				pWnd->DialogUVIP->DialogUVSR->m_Image.SetBitmap((HBITMAP)(*m_pBitmap));*/  
			}
			if (IRFlag)
			{
				/*				CRect framerect_src;
				pWnd->DialogIRIP->DialogIRSR->m_Frame.GetClientRect(framerect_src);
				pWnd->DialogIRIP->DialogIRSR->m_Image.SetParent(&(pWnd->DialogIRIP->DialogIRSR->m_ImageContainer));
				CRect bmprect_src;
				pWnd->DialogIRIP->DialogIRSR->m_Image.GetClientRect(bmprect_src);

				int width1 = CUVSRDlg::UVsrcSR_scaling->width;
				int height1 = CUVSRDlg::UVsrcSR_scaling->height;
				pWnd->DialogIRIP->DialogIRSR->m_Image.MoveWindow((framerect_src.Width()-dst->width)/2,(framerect_src.Height()-dst->height)/2,dst->width,dst->height);
				pWnd->DialogIRIP->DialogIRSR->m_Image.SetBitmap((HBITMAP)(*m_pBitmap)); */ 


				CRect framerect_src;
				pWnd->DialogIRSR->m_Frame.GetClientRect(framerect_src);
				pWnd->DialogIRSR->m_Image.SetParent(&(pWnd->DialogIRSR->m_ImageContainer));
				CRect bmprect_src;
				pWnd->DialogIRSR->m_Image.GetClientRect(bmprect_src);

				int width1 = CUVSRDlg::UVsrcSR_scaling->width;
				int height1 = CUVSRDlg::UVsrcSR_scaling->height;
				pWnd->DialogIRSR->m_Image.MoveWindow((framerect_src.Width()-dst->width)/2,(framerect_src.Height()-dst->height)/2,dst->width,dst->height);
				pWnd->DialogIRSR->m_Image.SetBitmap((HBITMAP)(*m_pBitmap)); 
			}

		}
		else  //缩小图像
		{
			m_zScaling = m_zScaling - 0.3;  
			if (m_zScaling < 1)
			{
				m_zScaling = 1;
				//return FALSE;
			}

			int width = CUVSRDlg::UVsrcSR_scaling->width * m_zScaling;       
			//目标图像的宽为源图象宽的m_zScaling倍    
			int height = CUVSRDlg::UVsrcSR_scaling->height * m_zScaling; 
			//目标图像的高为源图象高的m_zScaling倍  
			IplImage*dst = cvCreateImage(cvSize(width,height), CUVSRDlg::UVsrcSR_scaling->depth, CUVSRDlg::UVsrcSR_scaling->nChannels);    
			//构造目标图象    
			cvResize(CUVSRDlg::UVsrcSR_scaling, dst, CV_INTER_CUBIC);                        
			//缩放源图像到目标图像
			CBitmap*m_pBitmap = new CBitmap();  
		
			CUVSRDlg::IplImageToCBitmap(dst, &m_pBitmap); 

			if (UVFlag)
			{
/*				CRect framerect_src;
				pWnd->DialogUVIP->DialogUVSR->m_Frame.GetClientRect(framerect_src);
				pWnd->DialogUVIP->DialogUVSR->m_Image.SetParent(&(pWnd->DialogUVIP->DialogUVSR->m_ImageContainer));
				CRect bmprect_src;
				pWnd->DialogUVIP->DialogUVSR->m_Image.GetClientRect(bmprect_src);

				int width1 = CUVSRDlg::UVsrcSR_scaling->width;
				int height1 = CUVSRDlg::UVsrcSR_scaling->height;
				pWnd->DialogUVIP->DialogUVSR->m_Image.MoveWindow((framerect_src.Width()-dst->width)/2,(framerect_src.Height()-dst->height)/2,dst->width,dst->height);
				pWnd->DialogUVIP->DialogUVSR->m_Image.SetBitmap((HBITMAP)(*m_pBitmap)); */ 
			}
			if (IRFlag)
			{
/*				CRect framerect_src;
				pWnd->DialogIRIP->DialogIRSR->m_Frame.GetClientRect(framerect_src);
				pWnd->DialogIRIP->DialogIRSR->m_Image.SetParent(&(pWnd->DialogIRIP->DialogIRSR->m_ImageContainer));
				CRect bmprect_src;
				pWnd->DialogIRIP->DialogIRSR->m_Image.GetClientRect(bmprect_src);

				int width1 = CUVSRDlg::UVsrcSR_scaling->width;
				int height1 = CUVSRDlg::UVsrcSR_scaling->height;
				pWnd->DialogIRIP->DialogIRSR->m_Image.MoveWindow((framerect_src.Width()-dst->width)/2,(framerect_src.Height()-dst->height)/2,dst->width,dst->height);
				pWnd->DialogIRIP->DialogIRSR->m_Image.SetBitmap((HBITMAP)(*m_pBitmap)); */ 

				CRect framerect_src;
				pWnd->DialogIRSR->m_Frame.GetClientRect(framerect_src);
				pWnd->DialogIRSR->m_Image.SetParent(&(pWnd->DialogIRSR->m_ImageContainer));
				CRect bmprect_src;
				pWnd->DialogIRSR->m_Image.GetClientRect(bmprect_src);

				int width1 = CUVSRDlg::UVsrcSR_scaling->width;
				int height1 = CUVSRDlg::UVsrcSR_scaling->height;
				pWnd->DialogIRSR->m_Image.MoveWindow((framerect_src.Width()-dst->width)/2,(framerect_src.Height()-dst->height)/2,dst->width,dst->height);
				pWnd->DialogIRSR->m_Image.SetBitmap((HBITMAP)(*m_pBitmap)); 
			}

		}
	}

	return TRUE;
//	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

