// AnalysisDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NewSubstationDetectionServer.h"
#include "AnalysisDlg.h"
#include "afxdialogex.h"
#include "Utils.h"
#include "changeColor.h"


IplImage* colorImage_ori;
IplImage* grayImage_ori;

//�¶���ֵ��ʵ�����Ǹ����豸���ͻ��
float first_grayValue = 100;  //��һ���¶���ֵ
float second_grayValue = 255; //�ڶ����¶���ֵ
float third_grayValue = 300;  //�������¶���ֵ


int total_grayValue = 0;

static int flag[600][800] = {1}; //�洢���ص�ı�־��0��δ���� 1���ѷ���
//int flag[240][320] = {1}; //�洢���ص�ı�־��0��δ���� 1���ѷ���
int AbnormalDimension[3] = {0}; //�洢�쳣��������

float referTemp_edit = 0;
CString deviceType;
CString enviTemp_static;


int flag_choiceArea = 0;

//��һ��ͼƬ�ؼ��ĸߺͿ�
int pictureControl_height = 0;
int pictureControl_width = 0;

//ͼƬ�ĸߺͿ�
int picture_height = 0;
int picture_width = 0;

int detectStartRow = 60;  //���ü��������ʵ��Ӧ����Ӧ����Ϊȫͼ
int detectEndRow = 220;
int detectStartCol = 40;
int detectEndCol = 80;

CToolTipCtrl  m_tooltip; //��ʾ��Ϣ

//���ָ�����ͼƬ����ʵλ��
int position_x = 0;
int position_y = 0;

//������������
int position_click_x = 0; 
int position_click_y = 0;

//����Ҽ�ѡ���ѡ����
int position_areaStart_x = 0;
int position_areaStart_y = 0;
int position_areaEnd_x = 0;
int position_areaEnd_y = 0;

CRect rectL; //picControlλ��
int start_x = 0, start_y = 0, end_x = 0, end_y = 0; //����Ϊ��㣬���½�Ϊ�յ�  picControl����Դ��ڵ�λ��

void initArray(int* array,int value);

//�洢�쳣���ĵ�����
int center_x = 0;
int center_y = 0;
int maxGrayValue = 0; //�洢�쳣�������Ҷ�����ֵ

CDC* pDC; //����ѡ����ʹ��
// CAnalysisDlg �Ի���

IMPLEMENT_DYNAMIC(CAnalysisDlg, CDialogEx)

CAnalysisDlg::CAnalysisDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAnalysisDlg::IDD, pParent)
{

}

CAnalysisDlg::~CAnalysisDlg()
{
}

void CAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_analysis_pic, m_analysis_pic);
	DDX_Control(pDX, IDC_analysis_slider, m_analysis_slider);
	DDX_Control(pDX, IDC_analysis_choiceDeviceType, m_analysis_choiceDeviceType);
	DDX_Control(pDX, IDC_analysisChoiceReferTemp, m_analysis_choiceReferTemp);
	DDX_Control(pDX, IDC_analysis_list, m_analysis_list);
	DDX_Control(pDX, IDC_enviTemp, m_enviTemp);
}


BEGIN_MESSAGE_MAP(CAnalysisDlg, CDialogEx)
	ON_BN_CLICKED(IDC_analysis_btn, &CAnalysisDlg::OnBnClickedanalysisbtn)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_analysis_btn_choiceArea, &CAnalysisDlg::OnBnClickedanalysisbtnchoicearea)
END_MESSAGE_MAP()


// CAnalysisDlg ��Ϣ�������

void CAnalysisDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	//DrawPicToHDC(data.image,IDC_analysis_pic);

	//colorImage_ori = cvLoadImage("test.jpg");
	colorImage_ori = cvLoadImage("11.jpg");
	DrawPicToHDC(colorImage_ori,IDC_analysis_pic);

	grayImage_ori = IplImage_colorToGray(colorImage_ori);

	//��ȡͼƬ�ĸߺͿ�
	picture_height = grayImage_ori->height;
	picture_width = grayImage_ori->width;

	//��ʼ������
	pDC = this->GetDC();

	//��ɫ���
	//CRect   rect;  
	//GetClientRect(rect); 
	//pDC->FillSolidRect(rect, RGB(255,0,0));

	CPen pen(PS_SOLID,10,RGB(0,255,0));   
	pDC->SelectObject(pen); 


}


BOOL CAnalysisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//��ȡpictureControl�ĳߴ�
	CRect rect_1;
	CWnd *pWnd = GetDlgItem(IDC_analysis_pic);	//IDC_pictureΪpicture�ؼ�ID
	pWnd->GetClientRect(&rect_1);		//rectΪ�ؼ��Ĵ�С
	pictureControl_height = rect_1.Height();
	pictureControl_width = rect_1.Width();

	//��ȡ�ؼ��������Ļ��λ��
	GetDlgItem(IDC_analysis_pic)->GetWindowRect(&rectL);
	ScreenToClient(rectL);//ת��Ϊ�Ի����ϵ����λ��
	start_x = rectL.left;
	start_y = rectL.top;
	end_x = rectL.right;
	end_y = rectL.bottom;


	//��ʼ��m_tooltip
	EnableToolTips(TRUE);
	m_tooltip.Create(this);
	m_tooltip.AddTool(this,TTS_ALWAYSTIP);
	m_tooltip.SetTipTextColor(RGB(0,0,0));
	m_tooltip.SetDelayTime(100);
	m_tooltip.Activate(true);

	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С 
	//list control�ؼ���ʼ��
	CRect rect; 
	m_analysis_list.GetClientRect(&rect);   

	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_analysis_list.SetExtendedStyle(m_analysis_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   

	// Ϊ�б���ͼ�ؼ��������   
	m_analysis_list.InsertColumn(0, _T("���"), LVCFMT_CENTER, rect.Width()/18, 0);   
	m_analysis_list.InsertColumn(1, _T("�豸����"), LVCFMT_CENTER, rect.Width()/9, 1); 
	m_analysis_list.InsertColumn(2, _T("�쳣����"), LVCFMT_CENTER, rect.Width()/9, 2);
	m_analysis_list.InsertColumn(3, _T("�쳣���"), LVCFMT_CENTER, rect.Width()/9, 2); 
	m_analysis_list.InsertColumn(4, _T("����¶�(��C)"), LVCFMT_CENTER, rect.Width()/9, 4);
	m_analysis_list.InsertColumn(5, _T("�����¶�(��C)"), LVCFMT_CENTER, rect.Width()/9, 3);
	m_analysis_list.InsertColumn(6, _T("�ο��¶�(��C)"), LVCFMT_CENTER, rect.Width()/9, 5);
	m_analysis_list.InsertColumn(7, _T("����²�(%)"), LVCFMT_CENTER, rect.Width()/9, 6);
	m_analysis_list.InsertColumn(8, _T("���ϵȼ�"), LVCFMT_CENTER, rect.Width()/6, 8);


	//λ�����ѡ��
	m_analysis_choiceDeviceType.InsertString(0, _T("������"));
	m_analysis_choiceDeviceType.InsertString(1, _T("�����׹�"));
	m_analysis_choiceDeviceType.InsertString(2, _T("��ѹ���ع�"));
	m_analysis_choiceDeviceType.InsertString(3, _T("������·��"));
	m_analysis_choiceDeviceType.SetCurSel(0);

	//slider��ʼ������
	m_analysis_slider.SetRange(0,100);//���û�����Χ
	m_analysis_slider.SetTicFreq(10);//ÿ10����λ��һ�̶�
	m_analysis_slider.SetPos(50);

	//((CWnd*)GetDlgItem(IDC_abnormalPic_oustValue))->SetWindowText(intToCString(50));
	//((CWnd*)GetDlgItem(IDC_abnormalPic_areaSize))->SetWindowText(intToCString(0));

	 GetDlgItem(IDC_enviTemp)->GetWindowText(enviTemp_static);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CAnalysisDlg::OnBnClickedanalysisbtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString referTemp;  
	m_analysis_choiceReferTemp.GetWindowText(referTemp); 
	referTemp_edit = CStringToFloat(referTemp);

	if (referTemp.IsEmpty())
	{
		AfxMessageBox(_T("�ο��¶Ȳ���Ϊ��\r\n���������ͼ����ѡ��ο��¶�"));
	}

	//��ȡ�豸����
	int deviceType_index = m_analysis_choiceDeviceType.GetCurSel();
	m_analysis_choiceDeviceType.GetLBText(deviceType_index,deviceType);

	//���¼������
	if (position_areaStart_y == position_areaEnd_y && position_areaStart_x == position_areaEnd_x)
	{
		//û��ѡ��������Ĭ��Ϊȫ��
	}else{
		detectStartRow =  (position_areaStart_y - start_y) * picture_height / pictureControl_height;
		detectStartCol = (position_areaStart_x - start_x) * picture_width / pictureControl_width;
		detectEndRow = (position_areaEnd_y - start_y) * picture_height / pictureControl_height;
		detectEndCol = (position_areaEnd_x - start_x) * picture_width / pictureControl_width;

		AfxMessageBox(intToCString(picture_width) + _T(" ") + intToCString(pictureControl_width));
		AfxMessageBox(intToCString(position_areaStart_y) + _T(" ") + intToCString(position_areaStart_x) + _T(" ") + intToCString(position_areaEnd_y)+ _T(" ") + intToCString(position_areaEnd_x));
		AfxMessageBox(intToCString(detectStartRow) + _T(" ") + intToCString(detectStartCol) + _T(" ") + intToCString(detectEndRow) + _T(" ") + intToCString(detectEndCol));
	}
	

	//����쳣������ʾ��ͼ��ؼ���
	detectAbnormalAreaAndDrawToPicControl();

	//����쳣����
	detectAbnormalArea(colorImage_ori);

	//CString data = intToCString(rectL.left) +_T(",")+ intToCString(rectL.top) +_T(",")+ intToCString(rectL.right) +_T(",")+ intToCString(rectL.bottom);
	//AfxMessageBox(Save2Float(referTemp_edit));
}


//����Ƶ֡��ʾ��pictureControl
void CAnalysisDlg::DrawPicToHDC(IplImage *img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC(); 
	HDC hDC= pDC->GetSafeHdc(); 
	CRect rect; 
	GetDlgItem(ID)->GetClientRect(&rect); 
	CvvImage cimg; 

	cimg.CopyOf(img); 
	cimg.DrawToHDC(hDC,&rect); 

	ReleaseDC(pDC); 
}




void CAnalysisDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);

	nPos = m_analysis_slider.GetPos ();  
	//((CWnd*)GetDlgItem(IDC_abnormalPic_oustValue))->SetWindowText(intToCString(nPos));
	UpdateData(true);  //���½�������

	IplImage* colorImage = cvCloneImage(colorImage_ori);
	IplImage* grayImage = IplImage_colorToGray(colorImage);


	int temp = 0;
	int maxValue = 0;
	int areaSize = 0;

	//��ȡ����������Ҷ�ֵ
	for (int i=detectStartRow;i<=detectEndRow;i++)
	{
		for (int j=detectStartCol;j<=detectEndCol;j++)
		{
			temp = getGray(grayImage,i,j);
			if (temp > maxValue)
			{
				maxValue = temp;
			}

		}
	}

	//��ȡ����������Ҷ�ֵ
	for (int i=detectStartRow;i<=detectEndRow;i++)
	{
		for (int j=detectStartCol;j<=detectEndCol;j++)
		{
			temp = getGray(grayImage,i,j);
			if ((float)temp/(float)maxValue > (float)nPos/100)
			{
				changeColor(colorImage,i,j,0,0,0);
				areaSize ++;
			}else{
				//changeColor(colorImage_3,i,j,0,0,0);
			}

		}
	}

	//((CWnd*)GetDlgItem(IDC_abnormalPic_areaSize))->SetWindowText(intToCString(areaSize));
	DrawPicToHDC(colorImage,IDC_analysis_pic);
}


void CAnalysisDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnMouseMove(nFlags, point);

	if (flag_choiceArea == 0)
	{
		if ((point.x >= start_x) && (point.x <= end_x) && (point.y >= start_y) && (point.y <= end_y))
		{
			position_x = (point.x - start_x)*picture_width/pictureControl_width;
			position_y = (point.y - start_y)*picture_height/pictureControl_height;

			int imageValue = getGray(grayImage_ori,position_y, position_x); //����Ϊrow��col
			//float temp = getTempFromValue_1(imageValue);
			m_tooltip.UpdateTipText(intToCString(imageValue),this);
		}
	}else if(position_areaStart_x !=0 && position_areaStart_y != 0){

		if ((point.x >= position_areaStart_x) && (point.x <= end_x) && (point.y >= position_areaStart_y) && (point.y <= end_y))
		{
			DrawPicToHDC(cvCloneImage(colorImage_ori),IDC_analysis_pic); //ˢ��picControl�е�ͼƬ
			position_areaEnd_x = point.x;
			position_areaEnd_y = point.y;

			//����������
			pDC->MoveTo(position_areaStart_x,position_areaStart_y);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
			pDC->LineTo(position_areaStart_x,position_areaEnd_y);  
			pDC->LineTo(position_areaEnd_x,position_areaEnd_y); 
			pDC->LineTo(position_areaEnd_x,position_areaStart_y); 
			pDC->LineTo(position_areaStart_x,position_areaStart_y); 


		}
	}

	
}

//m_tooltip����ʹ��
BOOL CAnalysisDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	
	/*if (MousePt1.x >= 30 && MousePt1.x < 385 && MousePt1.y >= 20 && MousePt1.y < 290)
	{*/
			m_tooltip.RelayEvent(pMsg);
		//}
	

	return CDialogEx::PreTranslateMessage(pMsg);
}


//����������¼�
void CAnalysisDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnLButtonDown(nFlags, point);

	if ((point.x >= start_x) && (point.x <= end_x) && (point.y >= start_y) && (point.y <= end_y))
	{
		position_click_x = (point.x - start_x)*picture_width/pictureControl_width;
		position_click_y = (point.y - start_y)*picture_height/pictureControl_height;

		int imageValue = getGray(grayImage_ori,position_click_y, position_click_x); //����Ϊrow��col
		//float temp = getTempFromValue_1(imageValue);

		((CWnd*)GetDlgItem(IDC_deviceType))->SetWindowText(intToCString(imageValue));
	}
}

//AfxMessageBox(intToCString(imageValue));


//����Ҽ�������ͷţ�ѡ�����������Ϊ��ѡ����
void CAnalysisDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnRButtonDown(nFlags, point);

	//DrawPicToHDC(colorImage_ori,IDC_analysis_pic); //ˢ��picControl�е�ͼƬ

	if ((point.x >= start_x) && (point.x <= end_x) && (point.y >= start_y) && (point.y <= end_y))
	{
		DrawPicToHDC(cvCloneImage(colorImage_ori),IDC_analysis_pic); //ˢ��picControl�е�ͼƬ
		position_areaStart_x = point.x;
		position_areaStart_y = point.y;

		flag_choiceArea = 1;   //
	}else{
		AfxMessageBox(_T("����ѡ��������Ͻǳ���ͼ��Χ��������ѡ��"));
	}
}


void CAnalysisDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnRButtonUp(nFlags, point);

	if ((point.x >= start_x) && (point.x <= end_x) && (point.y >= start_y) && (point.y <= end_y))
	{
		DrawPicToHDC(cvCloneImage(colorImage_ori),IDC_analysis_pic); //ˢ��picControl�е�ͼƬ
		position_areaEnd_x = point.x;
		position_areaEnd_y = point.y;

		//����������
		pDC->MoveTo(position_areaStart_x,position_areaStart_y);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
		pDC->LineTo(position_areaStart_x,position_areaEnd_y);  
		pDC->LineTo(position_areaEnd_x,position_areaEnd_y); 
		pDC->LineTo(position_areaEnd_x,position_areaStart_y); 
		pDC->LineTo(position_areaStart_x,position_areaStart_y); 

		flag_choiceArea = 0;
 
	}else{
		AfxMessageBox(_T("����ѡ��������½ǳ���ͼ��Χ��������ѡ��"));
	}
}

//ѡ������ť����¼�
void CAnalysisDlg::OnBnClickedanalysisbtnchoicearea()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str; 
	GetDlgItem(IDC_analysis_btn_choiceArea)->GetWindowText(str);

	if (str == _T("ѡ������"))
	{
		GetDlgItem(IDC_analysis_btn_choiceArea)->SetWindowText(_T("����ѡ��"));
		flag_choiceArea = 1;
	}else if (str == _T("����ѡ��"))
	{
		GetDlgItem(IDC_analysis_btn_choiceArea)->SetWindowText(_T("ѡ������"));
		flag_choiceArea = 0;
	}

}

void CAnalysisDlg::insertIntoList(CString deviceType, int x,int y,float AbnormalDimension,float areaMaxTemp,float enviTemp,float referTemp,float DiffTemp,int abnormalLevel){

	int nIndex = m_analysis_list.GetItemCount() + 1; //����������ţ���1��ʼ
	CString t;
	t.Format(_T("%d"), nIndex);


	CString centerPositon = _T("(") + intToCString(x) + _T(",") + intToCString(y) + _T(")");
	CString abnormalType;

	if (abnormalLevel == 0)
	{
		abnormalType = _T("һ��ȱ��");
	}else if (abnormalLevel == 1)
	{
		abnormalType = _T("����ȱ��");
	}else if (abnormalLevel == 2)
	{
		abnormalType = _T("��ͬ����ȱ��");
	}


	//�����б�����
	m_analysis_list.InsertItem(0,(LPCTSTR)t);
	m_analysis_list.SetItemText(0,1, deviceType);
	m_analysis_list.SetItemText(0,2, centerPositon);
	m_analysis_list.SetItemText(0,3,intToCString(AbnormalDimension));
	m_analysis_list.SetItemText(0,4,Save2Float(areaMaxTemp)); 
	m_analysis_list.SetItemText(0,5, Save2Float(enviTemp)); 
	m_analysis_list.SetItemText(0,6, Save2Float(referTemp)); 
	m_analysis_list.SetItemText(0,7, Save2Float(DiffTemp)); 
	m_analysis_list.SetItemText(0,8, abnormalType); 
}

//����쳣�����쳣��Ϣ���뵽�б���
void CAnalysisDlg::detectAbnormalAreaAndDrawToPicControl(){

	IplImage* image_color = cvCloneImage(colorImage_ori);
	IplImage* image_gray = IplImage_colorToGray(image_color);

	int src_rows = image_gray->height;
	int src_cols = image_gray->width;


	int space = 8;
	int grayValue = 0;
	

	for (int i=detectStartRow;i<=detectEndRow;i++)
	{
		for (int j=detectStartCol;j<=detectEndCol;j++)
		{

			grayValue = getGray(image_gray,i,j);
			if (abs(grayValue-first_grayValue) < space){


				if (checkPixelTemp(grayImage_ori,i,j,first_grayValue,5))
				{
					changeColor(image_color,i,j,0,0,0);
				}

			}else if (abs(grayValue-second_grayValue) < space){


				if (checkPixelTemp(grayImage_ori,i,j,second_grayValue,5))
				{
					changeColor(image_color,i,j,62,178,102);
				}


			}else if (abs(grayValue-third_grayValue) < space){


				if (checkPixelTemp(grayImage_ori,i,j,third_grayValue,5))
				{
					changeColor(image_color,i,j,0,97,56);
				}

			}

		}

	}

	DrawPicToHDC(image_color,IDC_analysis_pic);

	//�ͷ�IplImage
	cvReleaseImage( &image_color );
	cvReleaseImage( &image_gray );
}

//��������Ƿ�Ϊ��Ե����
bool CAnalysisDlg::checkPixelTemp(IplImage* &src, int x, int y, int value,int space)
{
	int big_count = 0;
	int small_count = 0;
	int x_range = src->width - 1;
	int y_range = src->height - 1;
	for (int i=x-1;i<x+2;i+=1)
	{
		for (int j=y-1;j<y+2;j+=1)
		{
			if (i>=0 && i<=x_range && j>=0 && j<=y_range) //����±��Ƿ�Խ��
			{
				if ((getGray(src,i,j)-value) > space)
				{
					big_count ++;
				}else 
				{
					//small_count ++;
				}
			}
		}
	}

	if (big_count >= 3)
	{
		return true;
	}
	return false;
}


//����ͳ���쳣���򲢽��쳣��Ϣ���뵽�б���
void CAnalysisDlg::detectAbnormalArea(IplImage* colorImage_ori){


    IplImage* colorImage = cvCloneImage(colorImage_ori);
    IplImage* grayImage = IplImage_colorToGray(colorImage);

	int grayValue = 0;
	//��������쳣
	for (int i=detectStartRow;i<=detectEndRow;i++)
	{
		for (int j=detectStartCol;j<=detectEndCol;j++)
		{
			grayValue = getGray(grayImage,i,j);
			if (grayValue >= first_grayValue && grayValue <= second_grayValue)
			{
				changeColor(colorImage,i,j,255,0,0); //��ɫ

			}else if (grayValue >= second_grayValue && grayValue <= third_grayValue)
			{
				changeColor(colorImage,i,j,0,255,0); //��ɫ

			}else if (grayValue > third_grayValue){

				changeColor(colorImage,i,j,0,0,255); //��ɫ

			}else{
				changeColor(colorImage,i,j,0,0,0);
			}
		}
	}

	
	//���ü������
	for (int i=detectStartRow;i<=detectEndRow;i++)
	{
		for (int j=detectStartCol;j<=detectEndCol;j++)
		{
			flag[i][j] = 0;
		}
	}

	
	vector<int> rgb;
	int firstAbnormalCount = 0;
	int secondAbnormalCount = 0;
	int thirdAbnormalCount = 0;

	//cvShowImage("colorImage",colorImage);

	float areaMaxTemp = 0;
	float areaMinTemp = 100;
	float areaAvgTemp = 0;
	float referDiffTemp = 0;
	

	//ͳ���쳣����
	for (int i=detectStartRow;i<=detectEndRow;i++)
	{
		for (int j=detectStartCol;j<=detectEndCol;j++)
		{
			if (flag[i][j] == 0)
			{
				rgb = getRGB(colorImage,i,j);
				if (rgb[0] == 255)
				{
					maxGrayValue = getGray(grayImage,i,j); //�洢����������Ҷ�ֵ
					center_x = i;
					center_y = j;
					//total_grayValue += getGray(grayImage,i,j); //�洢��������ܻҶ�ֵ
					rgb.clear();
					flag[i][j] = 1;
					findArea(colorImage,grayImage,0,firstAbnormalCount,i,j);
					//�����Ͻ���ʾfirstAbnormalCount
					if (AbnormalDimension[0]>20)
					{
						firstAbnormalCount ++;
						//areaMaxTemp = getTempFromValue_1(maxGrayValue); //��ȡ��������¶�
						//areaMinTemp = getTempFromValue_1(minGrayValue); //��ȡ��������¶�
						//areaAvgTemp = getTempFromValue_1(total_grayValue/AbnormalDimension[0]); //��ȡ����ƽ���¶�
						//referDiffTemp = (T1 - T2)/(T1 - T0) T1:���� T2:���� T0:����
						referDiffTemp = (float(areaMaxTemp) - 10)/(float(areaMaxTemp) - referTemp_edit);
						insertIntoList(deviceType,i,j,AbnormalDimension[0],maxGrayValue,CStringToFloat(enviTemp_static),referTemp_edit,referDiffTemp,0);
						//AfxMessageBox(intToCString(i)+_T(" ")+intToCString(j));
					}
					
					//������ԭ
					AbnormalDimension[0] = 1;
					maxGrayValue = 0;
					center_x = 0;
					center_y = 0;
					
					
				}else if (rgb[1] == 255)
				{
					maxGrayValue = getGray(grayImage,i,j); //�洢����������Ҷ�ֵ
					center_x = i;
					center_y = j;
					//total_grayValue += getGray(grayImage,i,j); //�洢��������ܻҶ�ֵ
					rgb.clear();
					flag[i][j] = 1;
					findArea(colorImage,grayImage,1,firstAbnormalCount,i,j);
					if (AbnormalDimension[1]>20)
					{
						secondAbnormalCount ++;

						//areaMaxTemp = getTempFromValue_1(maxGrayValue); //��ȡ��������¶�
						//areaMinTemp = getTempFromValue_1(minGrayValue); //��ȡ��������¶�
						//areaAvgTemp = getTempFromValue_1(total_grayValue/AbnormalDimension[1]); //��ȡ����ƽ���¶�
						//referDiffTemp = (areaMaxTemp - CStringToInt(m_abnormalDeviceTemp))/(areaMaxTemp - CStringToInt(m_abnormalEnviTemp));

						referDiffTemp = (float(areaMaxTemp) - 10)/(float(areaMaxTemp) - referTemp_edit);
						insertIntoList(deviceType,i,j,AbnormalDimension[1],maxGrayValue,CStringToFloat(enviTemp_static),referTemp_edit,referDiffTemp,1);

						//AfxMessageBox(intToCString(i)+_T(" ")+intToCString(j));
						//AfxMessageBox(_T("AbnormalDimension[1]")+intToCString(AbnormalDimension[1]));
					}

					//������ԭ
					AbnormalDimension[1] = 0;
					maxGrayValue = 0;
					center_x = 0;
					center_y = 0;
					
				}else if (rgb[2] == 255)
				{

					maxGrayValue = getGray(grayImage,i,j); //�洢����������Ҷ�ֵ
					center_x = i;
					center_y = j;
					//total_grayValue += getGray(grayImage,i,j); //�洢��������ܻҶ�ֵ
					rgb.clear();
					flag[i][j] = 1;
					findArea(colorImage,grayImage,2,firstAbnormalCount,i,j);

					if (AbnormalDimension[2]>20)
					{
						thirdAbnormalCount++;
						//areaMaxTemp = getTempFromValue_1(maxGrayValue); //��ȡ��������¶�
						//areaMinTemp = getTempFromValue_1(minGrayValue); //��ȡ��������¶�
						//areaAvgTemp = getTempFromValue_1(total_grayValue/AbnormalDimension[1]); //��ȡ����ƽ���¶�
						//referDiffTemp = (areaMaxTemp - CStringToInt(m_abnormalDeviceTemp))/(areaMaxTemp - CStringToInt(m_abnormalEnviTemp));

						referDiffTemp = (float(areaMaxTemp) - 10)/(float(areaMaxTemp) - referTemp_edit);
						insertIntoList(deviceType,i,j,AbnormalDimension[2],maxGrayValue,CStringToFloat(enviTemp_static),referTemp_edit,referDiffTemp,2);

						//AfxMessageBox(intToCString(i)+_T(" ")+intToCString(j));
						//AfxMessageBox(_T("AbnormalDimension[2]")+intToCString(AbnormalDimension[2]));
					}

					AbnormalDimension[2] = 0;
					maxGrayValue = 0;
					center_x = 0;
					center_y = 0;
				}
			}
		}
	}


	/*AfxMessageBox(_T("firstAbnormalCount")+intToCString(firstAbnormalCount)); 
	AfxMessageBox(_T("secondAbnormalCount")+intToCString(secondAbnormalCount));
	AfxMessageBox(_T("thirdAbnormalCount")+intToCString(thirdAbnormalCount));*/
}


vector<int> rgb; //��ʱ�洢rgbֵ
int temp_grayValue = 0;  //��ʱ�洢�Ҷ�ֵ

void CAnalysisDlg::findArea(IplImage* colorImage,IplImage* grayImage,int index,int AbnormalCount,int x,int y){



	for (int i=x-1;i<=x+1;i++)
	{
		for (int j=y-1;j<=y+1;j++)
		{
			if ((flag[i][j] == 0) && (i>=detectStartRow) && (i<=detectEndRow) && (j>=detectStartCol) && (j<=detectEndCol))
			{
				rgb = getRGB(colorImage,i,j);
				if (rgb[index] == 255)
				{
					temp_grayValue = getGray(grayImage,i,j);

					if (temp_grayValue > maxGrayValue) //��ȡ��������¶�ֵ
					{
						maxGrayValue = temp_grayValue;
						center_x = i;
						center_y = j;
					}

					//������ԭ
					rgb.clear();
					flag[i][j] = 1;
					AbnormalDimension[index] ++;
					findArea(colorImage,grayImage,index,AbnormalCount,i,j);
				}
			}	
		}
	}
}

void initArray(int* array,int value){

	for (int i=0;i<sizeof(array)/sizeof(array[0]);i++)
	{
		array[i] = value;
	}

}

