// AnalysisDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NewSubstationDetectionServer.h"
#include "AnalysisDlg.h"
#include "afxdialogex.h"
#include "Utils.h"
#include "changeColor.h"
#include "IRReportDlg.h"




IplImage* colorImage_ori;
IplImage* grayImage_ori;

//温度阈值在实际中是根据设备类型获得
float first_grayValue = 100;  //第一个温度阈值
float second_grayValue = 255; //第二个温度阈值
float third_grayValue = 300;  //第三个温度阈值


int total_grayValue = 0;

static int flag[600][800] = {1}; //存储像素点的标志，0：未访问 1：已访问
//int flag[240][320] = {1}; //存储像素点的标志，0：未访问 1：已访问
int AbnormalDimension[3] = {0}; //存储异常区域的面积

float referTemp_edit = 0;
CString deviceType;
CString enviTemp_static;


int flag_choiceArea = 0;

//第一个图片控件的高和宽
int pictureControl_height = 0;
int pictureControl_width = 0;

//图片的高和宽
int picture_height = 0;
int picture_width = 0;

int detectStartRow = 60;  //设置检测区域，在实际应用中应设置为全图
int detectEndRow = 220;
int detectStartCol = 40;
int detectEndCol = 80;

CToolTipCtrl  m_tooltip; //提示信息

//鼠标指向相对图片的真实位置
int position_x = 0;
int position_y = 0;

//鼠标点击点的坐标
int position_click_x = 0; 
int position_click_y = 0;

//鼠标右键选择候选区域
int position_areaStart_x = 0;
int position_areaStart_y = 0;
int position_areaEnd_x = 0;
int position_areaEnd_y = 0;

CRect rectL; //picControl位置
int start_x = 0, start_y = 0, end_x = 0, end_y = 0; //左上为起点，右下角为终点  picControl的相对窗口的位置

void initArray(int* array,int value);

//存储异常中心的坐标
int center_x = 0;
int center_y = 0;
int maxGrayValue = 0; //存储异常区域最大灰度像素值

CDC* pDC; //画候选区域使用
// CAnalysisDlg 对话框

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


// CAnalysisDlg 消息处理程序

void CAnalysisDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	//DrawPicToHDC(data.image,IDC_analysis_pic);

	//colorImage_ori = cvLoadImage("test.jpg");
	colorImage_ori = cvLoadImage("11.jpg");
	DrawPicToHDC(colorImage_ori,IDC_analysis_pic);

	grayImage_ori = IplImage_colorToGray(colorImage_ori);

	//获取图片的高和宽
	picture_height = grayImage_ori->height;
	picture_width = grayImage_ori->width;

	//初始化画笔
	pDC = this->GetDC();

	//颜色填充
	//CRect   rect;  
	//GetClientRect(rect); 
	//pDC->FillSolidRect(rect, RGB(255,0,0));

	CPen pen(PS_SOLID,10,RGB(0,255,0));   
	pDC->SelectObject(pen); 


}


BOOL CAnalysisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//获取pictureControl的尺寸
	CRect rect_1;
	CWnd *pWnd = GetDlgItem(IDC_analysis_pic);	//IDC_picture为picture控件ID
	pWnd->GetClientRect(&rect_1);		//rect为控件的大小
	pictureControl_height = rect_1.Height();
	pictureControl_width = rect_1.Width();

	//获取控件相对于屏幕的位置
	GetDlgItem(IDC_analysis_pic)->GetWindowRect(&rectL);
	ScreenToClient(rectL);//转化为对话框上的相对位置
	start_x = rectL.left;
	start_y = rectL.top;
	end_x = rectL.right;
	end_y = rectL.bottom;


	//初始化m_tooltip
	EnableToolTips(TRUE);
	m_tooltip.Create(this);
	m_tooltip.AddTool(this,TTS_ALWAYSTIP);
	m_tooltip.SetTipTextColor(RGB(0,0,0));
	m_tooltip.SetDelayTime(100);
	m_tooltip.Activate(true);

	// 获取编程语言列表视图控件的位置和大小 
	//list control控件初始化
	CRect rect; 
	m_analysis_list.GetClientRect(&rect);   

	// 为列表视图控件添加全行选中和栅格风格   
	m_analysis_list.SetExtendedStyle(m_analysis_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   

	// 为列表视图控件添加三列   
	m_analysis_list.InsertColumn(0, _T("编号"), LVCFMT_CENTER, rect.Width()/18, 0);   
	m_analysis_list.InsertColumn(1, _T("设备类型"), LVCFMT_CENTER, rect.Width()/9, 1); 
	m_analysis_list.InsertColumn(2, _T("异常中心"), LVCFMT_CENTER, rect.Width()/9, 2);
	m_analysis_list.InsertColumn(3, _T("异常面积"), LVCFMT_CENTER, rect.Width()/9, 2); 
	m_analysis_list.InsertColumn(4, _T("最高温度(°C)"), LVCFMT_CENTER, rect.Width()/9, 4);
	m_analysis_list.InsertColumn(5, _T("环境温度(°C)"), LVCFMT_CENTER, rect.Width()/9, 3);
	m_analysis_list.InsertColumn(6, _T("参考温度(°C)"), LVCFMT_CENTER, rect.Width()/9, 5);
	m_analysis_list.InsertColumn(7, _T("相对温差(%)"), LVCFMT_CENTER, rect.Width()/9, 6);
	m_analysis_list.InsertColumn(8, _T("故障等级"), LVCFMT_CENTER, rect.Width()/6, 8);


	//位置添加选项
	m_analysis_choiceDeviceType.InsertString(0, _T("连接器"));
	m_analysis_choiceDeviceType.InsertString(1, _T("充油套管"));
	m_analysis_choiceDeviceType.InsertString(2, _T("高压开关柜"));
	m_analysis_choiceDeviceType.InsertString(3, _T("空气断路器"));
	m_analysis_choiceDeviceType.SetCurSel(0);

	//slider初始化设置
	m_analysis_slider.SetRange(0,100);//设置滑动范围
	m_analysis_slider.SetTicFreq(10);//每10个单位画一刻度
	m_analysis_slider.SetPos(50);

	//((CWnd*)GetDlgItem(IDC_abnormalPic_oustValue))->SetWindowText(intToCString(50));
	//((CWnd*)GetDlgItem(IDC_abnormalPic_areaSize))->SetWindowText(intToCString(0));

	 GetDlgItem(IDC_enviTemp)->GetWindowText(enviTemp_static);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAnalysisDlg::OnBnClickedanalysisbtn()
{
	// TODO: 在此添加控件通知处理程序代码


	CString referTemp;  
	m_analysis_choiceReferTemp.GetWindowText(referTemp); 
	referTemp_edit = CStringToFloat(referTemp);

	//清空list中的内容
	m_analysis_list.DeleteAllItems();

	if (referTemp.IsEmpty())
	{
		AfxMessageBox(_T("参考温度不能为空\r\n请输入或在图像上选择参考温度"));
	}

	//获取设备名称
	int deviceType_index = m_analysis_choiceDeviceType.GetCurSel();
	m_analysis_choiceDeviceType.GetLBText(deviceType_index,deviceType);

	//更新检测区域
	if (position_areaStart_y == position_areaEnd_y && position_areaStart_x == position_areaEnd_x)
	{
		//没有选择检测区域，默认为全局
	}else{
		detectStartRow =  (position_areaStart_y - start_y) * picture_height / pictureControl_height;
		detectStartCol = (position_areaStart_x - start_x) * picture_width / pictureControl_width;
		detectEndRow = (position_areaEnd_y - start_y) * picture_height / pictureControl_height;
		detectEndCol = (position_areaEnd_x - start_x) * picture_width / pictureControl_width;

		AfxMessageBox(intToCString(picture_width) + _T(" ") + intToCString(pictureControl_width));
		AfxMessageBox(intToCString(position_areaStart_y) + _T(" ") + intToCString(position_areaStart_x) + _T(" ") + intToCString(position_areaEnd_y)+ _T(" ") + intToCString(position_areaEnd_x));
		AfxMessageBox(intToCString(detectStartRow) + _T(" ") + intToCString(detectStartCol) + _T(" ") + intToCString(detectEndRow) + _T(" ") + intToCString(detectEndCol));
	}
	

	//检测异常并将显示到图像控件中
	detectAbnormalAreaAndDrawToPicControl();

	//检测异常区域
	detectAbnormalArea(colorImage_ori);

	//CString data = intToCString(rectL.left) +_T(",")+ intToCString(rectL.top) +_T(",")+ intToCString(rectL.right) +_T(",")+ intToCString(rectL.bottom);
	//AfxMessageBox(Save2Float(referTemp_edit));
}


//将视频帧显示到pictureControl
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);

	nPos = m_analysis_slider.GetPos ();  
	//((CWnd*)GetDlgItem(IDC_abnormalPic_oustValue))->SetWindowText(intToCString(nPos));
	UpdateData(true);  //更新界面数据

	IplImage* colorImage = cvCloneImage(colorImage_ori);
	IplImage* grayImage = IplImage_colorToGray(colorImage);


	int temp = 0;
	int maxValue = 0;
	int areaSize = 0;

	//获取检测区域最大灰度值
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

	//获取检测区域最大灰度值
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnMouseMove(nFlags, point);

	if (flag_choiceArea == 0)
	{
		if ((point.x >= start_x) && (point.x <= end_x) && (point.y >= start_y) && (point.y <= end_y))
		{
			position_x = (point.x - start_x)*picture_width/pictureControl_width;
			position_y = (point.y - start_y)*picture_height/pictureControl_height;

			int imageValue = getGray(grayImage_ori,position_y, position_x); //参数为row和col
			//float temp = getTempFromValue_1(imageValue);
			m_tooltip.UpdateTipText(intToCString(imageValue),this);
		}
	}else if(position_areaStart_x !=0 && position_areaStart_y != 0){

		if ((point.x >= position_areaStart_x) && (point.x <= end_x) && (point.y >= position_areaStart_y) && (point.y <= end_y))
		{
			DrawPicToHDC(cvCloneImage(colorImage_ori),IDC_analysis_pic); //刷新picControl中的图片
			position_areaEnd_x = point.x;
			position_areaEnd_y = point.y;

			//画矩形区域
			pDC->MoveTo(position_areaStart_x,position_areaStart_y);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
			pDC->LineTo(position_areaStart_x,position_areaEnd_y);  
			pDC->LineTo(position_areaEnd_x,position_areaEnd_y); 
			pDC->LineTo(position_areaEnd_x,position_areaStart_y); 
			pDC->LineTo(position_areaStart_x,position_areaStart_y); 


		}
	}

	
}

//m_tooltip配置使用
BOOL CAnalysisDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	/*if (MousePt1.x >= 30 && MousePt1.x < 385 && MousePt1.y >= 20 && MousePt1.y < 290)
	{*/
			m_tooltip.RelayEvent(pMsg);
		//}
	

	return CDialogEx::PreTranslateMessage(pMsg);
}


//鼠标左键点击事件
void CAnalysisDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnLButtonDown(nFlags, point);

	if ((point.x >= start_x) && (point.x <= end_x) && (point.y >= start_y) && (point.y <= end_y))
	{
		position_click_x = (point.x - start_x)*picture_width/pictureControl_width;
		position_click_y = (point.y - start_y)*picture_height/pictureControl_height;

		int imageValue = getGray(grayImage_ori,position_click_y, position_click_x); //参数为row和col
		//float temp = getTempFromValue_1(imageValue);

		((CWnd*)GetDlgItem(IDC_deviceType))->SetWindowText(intToCString(imageValue));
	}
}

//AfxMessageBox(intToCString(imageValue));


//鼠标右键点击和释放，选择矩形区域作为候选区域
void CAnalysisDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnRButtonDown(nFlags, point);

	//DrawPicToHDC(colorImage_ori,IDC_analysis_pic); //刷新picControl中的图片

	if ((point.x >= start_x) && (point.x <= end_x) && (point.y >= start_y) && (point.y <= end_y))
	{
		DrawPicToHDC(cvCloneImage(colorImage_ori),IDC_analysis_pic); //刷新picControl中的图片
		position_areaStart_x = point.x;
		position_areaStart_y = point.y;

		flag_choiceArea = 1;   //
	}else{
		AfxMessageBox(_T("您所选区域的左上角超出图像范围，请重新选择！"));
	}
}


void CAnalysisDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnRButtonUp(nFlags, point);

	if ((point.x >= start_x) && (point.x <= end_x) && (point.y >= start_y) && (point.y <= end_y))
	{
		DrawPicToHDC(cvCloneImage(colorImage_ori),IDC_analysis_pic); //刷新picControl中的图片
		position_areaEnd_x = point.x;
		position_areaEnd_y = point.y;

		//画矩形区域
		pDC->MoveTo(position_areaStart_x,position_areaStart_y);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
		pDC->LineTo(position_areaStart_x,position_areaEnd_y);  
		pDC->LineTo(position_areaEnd_x,position_areaEnd_y); 
		pDC->LineTo(position_areaEnd_x,position_areaStart_y); 
		pDC->LineTo(position_areaStart_x,position_areaStart_y); 

		flag_choiceArea = 0;
 
	}else{
		AfxMessageBox(_T("您所选区域的右下角超出图像范围，请重新选择！"));
	}
}

//选择区域按钮点击事件
void CAnalysisDlg::OnBnClickedanalysisbtnchoicearea()
{
	// TODO: 在此添加控件通知处理程序代码


	////超分辨率重建
	
	DialogIRSR = new CUVSRDlg;
	DialogIRSR->Create(IDD_UVSR_DIALOG,this);
	DialogIRSR->ShowWindow(SW_SHOW);

	//CUVSRDlg dlg;
	//dlg.DoModal();

	//测试生成报告
	//CIRReportDlg dlg;
	//dlg.DoModal();

	//CString str; 
	//GetDlgItem(IDC_analysis_btn_choiceArea)->GetWindowText(str);

	//if (str == _T("选择区域"))
	//{
	//	GetDlgItem(IDC_analysis_btn_choiceArea)->SetWindowText(_T("结束选择"));
	//	flag_choiceArea = 1;
	//}else if (str == _T("结束选择"))
	//{
	//	GetDlgItem(IDC_analysis_btn_choiceArea)->SetWindowText(_T("选择区域"));
	//	flag_choiceArea = 0;
	//}

}

void CAnalysisDlg::insertIntoList(CString deviceType, int x,int y,float AbnormalDimension,float areaMaxTemp,float enviTemp,float referTemp,float DiffTemp,int abnormalLevel){

	int nIndex = m_analysis_list.GetItemCount() + 1; //插入数据序号，从1开始
	CString t;
	t.Format(_T("%d"), nIndex);


	CString centerPositon = _T("(") + intToCString(x) + _T(",") + intToCString(y) + _T(")");
	CString abnormalType;

	if (abnormalLevel == 0)
	{
		abnormalType = _T("一般缺陷");
	}else if (abnormalLevel == 1)
	{
		abnormalType = _T("严重缺陷");
	}else if (abnormalLevel == 2)
	{
		abnormalType = _T("视同紧急缺陷");
	}


	//插入列表数据
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

//检测异常并将异常信息插入到列表中
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

	//释放IplImage
	cvReleaseImage( &image_color );
	cvReleaseImage( &image_gray );
}

//检查像素是否为边缘像素
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
			if (i>=0 && i<=x_range && j>=0 && j<=y_range) //半段下标是否越界
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


//检测和统计异常区域并将异常信息插入到列表中
void CAnalysisDlg::detectAbnormalArea(IplImage* colorImage_ori){


    IplImage* colorImage = cvCloneImage(colorImage_ori);
    IplImage* grayImage = IplImage_colorToGray(colorImage);

	int grayValue = 0;
	//检测三级异常
	for (int i=detectStartRow;i<=detectEndRow;i++)
	{
		for (int j=detectStartCol;j<=detectEndCol;j++)
		{
			grayValue = getGray(grayImage,i,j);
			if (grayValue >= first_grayValue && grayValue <= second_grayValue)
			{
				changeColor(colorImage,i,j,255,0,0); //蓝色

			}else if (grayValue >= second_grayValue && grayValue <= third_grayValue)
			{
				changeColor(colorImage,i,j,0,255,0); //绿色

			}else if (grayValue > third_grayValue){

				changeColor(colorImage,i,j,0,0,255); //红色

			}else{
				changeColor(colorImage,i,j,0,0,0);
			}
		}
	}

	
	//设置检测区域
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
	

	//统计异常区域
	for (int i=detectStartRow;i<=detectEndRow;i++)
	{
		for (int j=detectStartCol;j<=detectEndCol;j++)
		{
			if (flag[i][j] == 0)
			{
				rgb = getRGB(colorImage,i,j);
				if (rgb[0] == 255)
				{
					maxGrayValue = getGray(grayImage,i,j); //存储该区域的最大灰度值
					center_x = i;
					center_y = j;
					//total_grayValue += getGray(grayImage,i,j); //存储该区域的总灰度值
					rgb.clear();
					flag[i][j] = 1;
					findArea(colorImage,grayImage,0,firstAbnormalCount,i,j);
					//在右上角显示firstAbnormalCount
					if (AbnormalDimension[0]>20)
					{
						firstAbnormalCount ++;
						//areaMaxTemp = getTempFromValue_1(maxGrayValue); //获取区域最高温度
						//areaMinTemp = getTempFromValue_1(minGrayValue); //获取区域最高温度
						//areaAvgTemp = getTempFromValue_1(total_grayValue/AbnormalDimension[0]); //获取区域平均温度
						//referDiffTemp = (T1 - T2)/(T1 - T0) T1:发热 T2:正常 T0:环境
						referDiffTemp = (float(areaMaxTemp) - 10)/(float(areaMaxTemp) - referTemp_edit);
						insertIntoList(deviceType,i,j,AbnormalDimension[0],maxGrayValue,CStringToFloat(enviTemp_static),referTemp_edit,referDiffTemp,0);
						//AfxMessageBox(intToCString(i)+_T(" ")+intToCString(j));
					}
					
					//参数还原
					AbnormalDimension[0] = 1;
					maxGrayValue = 0;
					center_x = 0;
					center_y = 0;
					
					
				}else if (rgb[1] == 255)
				{
					maxGrayValue = getGray(grayImage,i,j); //存储该区域的最大灰度值
					center_x = i;
					center_y = j;
					//total_grayValue += getGray(grayImage,i,j); //存储该区域的总灰度值
					rgb.clear();
					flag[i][j] = 1;
					findArea(colorImage,grayImage,1,firstAbnormalCount,i,j);
					if (AbnormalDimension[1]>20)
					{
						secondAbnormalCount ++;

						//areaMaxTemp = getTempFromValue_1(maxGrayValue); //获取区域最高温度
						//areaMinTemp = getTempFromValue_1(minGrayValue); //获取区域最高温度
						//areaAvgTemp = getTempFromValue_1(total_grayValue/AbnormalDimension[1]); //获取区域平均温度
						//referDiffTemp = (areaMaxTemp - CStringToInt(m_abnormalDeviceTemp))/(areaMaxTemp - CStringToInt(m_abnormalEnviTemp));

						referDiffTemp = (float(areaMaxTemp) - 10)/(float(areaMaxTemp) - referTemp_edit);
						insertIntoList(deviceType,i,j,AbnormalDimension[1],maxGrayValue,CStringToFloat(enviTemp_static),referTemp_edit,referDiffTemp,1);

						//AfxMessageBox(intToCString(i)+_T(" ")+intToCString(j));
						//AfxMessageBox(_T("AbnormalDimension[1]")+intToCString(AbnormalDimension[1]));
					}

					//参数还原
					AbnormalDimension[1] = 0;
					maxGrayValue = 0;
					center_x = 0;
					center_y = 0;
					
				}else if (rgb[2] == 255)
				{

					maxGrayValue = getGray(grayImage,i,j); //存储该区域的最大灰度值
					center_x = i;
					center_y = j;
					//total_grayValue += getGray(grayImage,i,j); //存储该区域的总灰度值
					rgb.clear();
					flag[i][j] = 1;
					findArea(colorImage,grayImage,2,firstAbnormalCount,i,j);

					if (AbnormalDimension[2]>20)
					{
						thirdAbnormalCount++;
						//areaMaxTemp = getTempFromValue_1(maxGrayValue); //获取区域最高温度
						//areaMinTemp = getTempFromValue_1(minGrayValue); //获取区域最高温度
						//areaAvgTemp = getTempFromValue_1(total_grayValue/AbnormalDimension[1]); //获取区域平均温度
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


vector<int> rgb; //暂时存储rgb值
int temp_grayValue = 0;  //暂时存储灰度值

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

					if (temp_grayValue > maxGrayValue) //获取区域最大温度值
					{
						maxGrayValue = temp_grayValue;
						center_x = i;
						center_y = j;
					}

					//参数还原
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

