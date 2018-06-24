// IRReportDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "PowerSystem.h"
#include "IRReportDlg.h"
#include "msword.h"

 BOOL IR_Complete_Flag;


// CIRReportDlg 对话框
 IplImage* ReportImg;
 IplImage* OrgIR;
 IplImage* ScaleImg;
 int ImgType;
struct IRUnsualInformation
{
	CString ID;
	CString MinTemper;
	CString MaxTemper;
	CString AvgTemper;
	CString RefTemper;
	CString DifTemper;
	CString FaultLevel;
};
 std::vector<IRUnsualInformation> IRUnsualData;

CString m_IRStationName_save;
CString m_IRRunNum_save;
CString m_IRClient_save;
CString m_IRTestAgn_save;
CString m_IRPilot_save;
CString m_IRTestDate_save;
CString m_IRTester_save;
CString m_IRTestSpot_save;
CString m_IRReportDate_save;
CString m_IRReporter_save;
CString m_IRAuditor_save;
CString m_IRApprover_save;

CString m_IRManufacturer_save;
CString m_IROperateDate_save;
CString m_IRUintType_save;
CString m_IRDetectSite_save;
CString m_IRWeather_save;
CString m_IRWindSpeed_save;
CString m_IRTestDis_save;
CString m_IRRadiation_save;
CString m_IRLoadCurrent_save;
CString m_IRRatedCurrent_save;
CString m_IRTemperature_save;
CString m_IRHumidity_save;

CString m_IRUnitExpress_save;
CString m_IRInstrument_save;
CString m_IRFaultExpress_save;


IMPLEMENT_DYNAMIC(CIRReportDlg, CDialog)

CIRReportDlg::CIRReportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIRReportDlg::IDD, pParent)
{

}

CIRReportDlg::~CIRReportDlg()
{
}

void CIRReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIRReportDlg, CDialog)
	ON_BN_CLICKED(IDC_PREVIEW, &CIRReportDlg::OnBnClickedPreview)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &CIRReportDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CIRReportDlg 消息处理程序

BOOL CIRReportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CvRect MainIFWindow = {0,0,1040,590};
	::SetWindowPos(this->m_hWnd,HWND_BOTTOM,MainIFWindow.x,MainIFWindow.y,MainIFWindow.width,MainIFWindow.height,SWP_NOZORDER);
	CenterWindow();

	IF_Font_Caption.CreatePointFont(100,_T("微软雅黑"),NULL);
	IF_Font_Button.CreatePointFont(110,_T("微软雅黑"),NULL);

	GetDlgItem(IDC_STATIC1)->MoveWindow(20,20,80,20);
	GetDlgItem(IDC_STATIC1)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_STATIC2)->MoveWindow(270,20,80,20);
	GetDlgItem(IDC_STATIC2)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_STATIC3)->MoveWindow(520,20,80,20);
	GetDlgItem(IDC_STATIC3)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_STATIC4)->MoveWindow(770,20,80,20);
	GetDlgItem(IDC_STATIC4)->SetFont(&IF_Font_Caption);

	GetDlgItem(IDC_EDIT1)->MoveWindow(100,15,150,30);
	GetDlgItem(IDC_EDIT1)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_EDIT2)->MoveWindow(350,15,150,30);
	GetDlgItem(IDC_EDIT2)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_EDIT3)->MoveWindow(600,15,150,30);
	GetDlgItem(IDC_EDIT3)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_EDIT4)->MoveWindow(850,15,150,30);
	GetDlgItem(IDC_EDIT4)->SetFont(&IF_Font_Caption);

	GetDlgItem(IDC_STATIC5)->MoveWindow(20,70,80,20);
	GetDlgItem(IDC_STATIC5)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_STATIC6)->MoveWindow(270,70,80,20);
	GetDlgItem(IDC_STATIC6)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_STATIC7)->MoveWindow(520,70,80,20);
	GetDlgItem(IDC_STATIC7)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_STATIC8)->MoveWindow(770,70,80,20);
	GetDlgItem(IDC_STATIC8)->SetFont(&IF_Font_Caption);

	GetDlgItem(IDC_EDIT5)->MoveWindow(100,65,150,30);
	GetDlgItem(IDC_EDIT5)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_EDIT6)->MoveWindow(350,65,150,30);
	GetDlgItem(IDC_EDIT6)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_EDIT7)->MoveWindow(600,65,150,30);
	GetDlgItem(IDC_EDIT7)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_EDIT8)->MoveWindow(850,65,150,30);
	GetDlgItem(IDC_EDIT8)->SetFont(&IF_Font_Caption);

	GetDlgItem(IDC_STATIC9)->MoveWindow(20,120,80,20);
	GetDlgItem(IDC_STATIC9)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_STATIC10)->MoveWindow(270,120,80,20);
	GetDlgItem(IDC_STATIC10)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_STATIC11)->MoveWindow(520,120,80,20);
	GetDlgItem(IDC_STATIC11)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_STATIC12)->MoveWindow(770,120,80,20);
	GetDlgItem(IDC_STATIC12)->SetFont(&IF_Font_Caption);

	GetDlgItem(IDC_EDIT9)->MoveWindow(100,115,150,30);
	GetDlgItem(IDC_EDIT9)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_EDIT10)->MoveWindow(350,115,150,30);
	GetDlgItem(IDC_EDIT10)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_EDIT11)->MoveWindow(600,115,150,30);
	GetDlgItem(IDC_EDIT11)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_EDIT12)->MoveWindow(850,115,150,30);
	GetDlgItem(IDC_EDIT12)->SetFont(&IF_Font_Caption);

	GetDlgItem(IDC_STATIC13)->MoveWindow(40,180,90,20);
	GetDlgItem(IDC_STATIC13)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_STATIC14)->MoveWindow(340,180,90,20);
	GetDlgItem(IDC_STATIC14)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_STATIC15)->MoveWindow(640,180,90,20);
	GetDlgItem(IDC_STATIC15)->SetFont(&IF_Font_Caption);

	GetDlgItem(IDC_EDIT13)->MoveWindow(130,175,170,30);
	GetDlgItem(IDC_EDIT13)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_EDIT14)->MoveWindow(430,175,170,30);
	GetDlgItem(IDC_EDIT14)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_EDIT15)->MoveWindow(730,175,170,30);
	GetDlgItem(IDC_EDIT15)->SetFont(&IF_Font_Caption);

	GetDlgItem(IDC_STATIC16)->MoveWindow(40,230,90,20);
	GetDlgItem(IDC_STATIC16)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_STATIC17)->MoveWindow(340,230,90,20);
	GetDlgItem(IDC_STATIC17)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_STATIC18)->MoveWindow(640,230,90,20);
	GetDlgItem(IDC_STATIC18)->SetFont(&IF_Font_Caption);

	GetDlgItem(IDC_EDIT16)->MoveWindow(130,225,170,30);
	GetDlgItem(IDC_EDIT16)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_EDIT17)->MoveWindow(430,225,170,30);
	GetDlgItem(IDC_EDIT17)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_EDIT18)->MoveWindow(730,225,170,30);
	GetDlgItem(IDC_EDIT18)->SetFont(&IF_Font_Caption);

	GetDlgItem(IDC_STATIC19)->MoveWindow(40,280,90,20);
	GetDlgItem(IDC_STATIC19)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_STATIC20)->MoveWindow(340,280,90,20);
	GetDlgItem(IDC_STATIC20)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_STATIC21)->MoveWindow(640,280,80,40);
	GetDlgItem(IDC_STATIC21)->SetFont(&IF_Font_Caption);

	GetDlgItem(IDC_EDIT19)->MoveWindow(130,275,170,30);
	GetDlgItem(IDC_EDIT19)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_EDIT20)->MoveWindow(430,275,170,30);
	GetDlgItem(IDC_EDIT20)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_EDIT21)->MoveWindow(730,275,170,30);
	GetDlgItem(IDC_EDIT21)->SetFont(&IF_Font_Caption);

	GetDlgItem(IDC_STATIC22)->MoveWindow(40,330,90,20);
	GetDlgItem(IDC_STATIC22)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_STATIC23)->MoveWindow(340,330,90,20);
	GetDlgItem(IDC_STATIC23)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_STATIC24)->MoveWindow(640,330,90,20);
	GetDlgItem(IDC_STATIC24)->SetFont(&IF_Font_Caption);

	GetDlgItem(IDC_EDIT22)->MoveWindow(130,325,170,30);
	GetDlgItem(IDC_EDIT22)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_EDIT23)->MoveWindow(430,325,170,30);
	GetDlgItem(IDC_EDIT23)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_EDIT24)->MoveWindow(730,325,170,30);
	GetDlgItem(IDC_EDIT24)->SetFont(&IF_Font_Caption);

	GetDlgItem(IDC_UINT1)->MoveWindow(900,225,30,20);
	GetDlgItem(IDC_UINT1)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_UINT2)->MoveWindow(300,275,20,20);
	GetDlgItem(IDC_UINT2)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_UINT3)->MoveWindow(900,275,20,20);
	GetDlgItem(IDC_UINT3)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_UINT4)->MoveWindow(300,325,20,20);
	GetDlgItem(IDC_UINT4)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_UINT5)->MoveWindow(600,325,20,20);
	GetDlgItem(IDC_UINT5)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_UINT6)->MoveWindow(900,325,20,20);
	GetDlgItem(IDC_UINT6)->SetFont(&IF_Font_Caption);

	GetDlgItem(IDC_STATIC25)->MoveWindow(250,375,90,40);
	GetDlgItem(IDC_STATIC25)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_STATIC26)->MoveWindow(270,425,90,20);
	GetDlgItem(IDC_STATIC26)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_STATIC27)->MoveWindow(270,475,90,20);
	GetDlgItem(IDC_STATIC27)->SetFont(&IF_Font_Caption);

	GetDlgItem(IDC_EDIT25)->MoveWindow(360,370,400,30);
	GetDlgItem(IDC_EDIT25)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_EDIT26)->MoveWindow(360,420,400,30);
	GetDlgItem(IDC_EDIT26)->SetFont(&IF_Font_Caption);
	GetDlgItem(IDC_EDIT27)->MoveWindow(360,470,400,30);
	GetDlgItem(IDC_EDIT27)->SetFont(&IF_Font_Caption);

	GetDlgItem(IDC_PREVIEW)->MoveWindow(780,510,100,40);
	GetDlgItem(IDC_PREVIEW)->SetFont(&IF_Font_Button);
	GetDlgItem(IDC_BUTTON1)->MoveWindow(600,510,100,40);
	GetDlgItem(IDC_BUTTON1)->SetFont(&IF_Font_Button);

	//CBitmap bmp;
	//bmp.LoadBitmap(IDB_DIALOG_BK);
	//m_IR_REPORT_brBk.CreatePatternBrush(&bmp);
	//bmp.DeleteObject();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CIRReportDlg::OnBnClickedPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	GetEditText();
	UpdateData(TRUE);
	TCHAR path[255];
	ZeroMemory(path,255);
	SHGetSpecialFolderPath(0,path,CSIDL_DESKTOPDIRECTORY,0);
	CString strDesktopPath;
	strDesktopPath.Format(_T("%s"), path);

	CString strCurPath;
	CString strDocName;
	::GetCurrentDirectory(MAX_PATH,strCurPath.GetBuffer(MAX_PATH));
	strCurPath.ReleaseBuffer();
	strCurPath.Format(_T("%s"),strCurPath);

	strDocName = strCurPath + _T("\\res\\IRReportTemplate.doc");

	CTime time = CTime::GetCurrentTime();
	CString m_strTime = time.Format("%Y-%m-%d-%H-%M-%S");
	CString m_IRReportName = _T("\\IRReport") + m_strTime + _T(".doc");
	//CString strDocNameCopy = strDesktopPath + m_IRReportName;
	CString strDocNameCopy = strCurPath + "\\红外处理报告\\" + m_IRReportName;
	CopyFile(strDocName,strDocNameCopy,1);


	COleVariant varFilePath(strDocNameCopy);
	COleVariant varstrNull(_T(""));
	COleVariant varZero((short)0);
	COleVariant varTrue(short(1),VT_BOOL);
	COleVariant varFalse(short(0),VT_BOOL);
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	_Application m_App;//定义Word提供的应用程序对象;
	Documents m_Docs;//定义Word提供的文档对象;
	Selection m_Sel;//定义Word提供的选择对象;

	m_Docs.ReleaseDispatch();
	m_Sel.ReleaseDispatch();
	m_App.m_bAutoRelease = true;

	if(!m_App.CreateDispatch(_T("Word.Application")))
	{ 
		AfxMessageBox(_T("创建Word失败，请先安装Microsoft Office Word！")); 
		return; 
	}

	m_App.SetVisible(TRUE);
	m_Docs.AttachDispatch(m_App.GetDocuments());

	CString m_WordVer = _T("Word2003");
	if (m_WordVer == _T("Word2003"))
	{//office2000
		m_Docs.Open(varFilePath,varFalse,varFalse,varFalse,
			varstrNull,varstrNull,varFalse,varstrNull,
			varstrNull,varTrue,varTrue,varTrue);
	}
	else
	{//office2003
		m_Docs.Open( //可看成VB语句set testDoc =  Word.documents.Open(…)
			COleVariant(strDocNameCopy,VT_BSTR),
			varFalse,    // Confirm Conversion.
			varFalse,    // ReadOnly.
			varFalse,    // AddToRecentFiles.
			covOptional, // PasswordDocument.
			covOptional, // PasswordTemplate.
			varTrue,    // Revert.
			covOptional, // WritePasswordDocument.
			covOptional, // WritePasswordTemplate.
			covOptional, // Format. // Last argument for Word 97
			covOptional, // Encoding // New for Word 2000/2002
			varTrue,     // Visible
			covOptional, // OpenConflictDocument
			covOptional, // OpenAndRepair
			varZero,     // DocumentDirection wdDocumentDirection LeftToRight
			covOptional  // NoEncodingDialog
			);
	}
	//打开Word文档； 
	m_Sel.AttachDispatch(m_App.GetSelection());//将Selection类对象m_Sel和Idispatch接口关联起来

	//save word file
	_Document oActiveDoc; 
	oActiveDoc = m_App.GetActiveDocument();

	//表格的正常用法（非标准表格）
	Tables m_tables = oActiveDoc.GetTables();
	////*第一个表格*////
	Table m_table1 = m_tables.Item(1);
	Rows rows = m_table1.GetRows();

	//第1行
	Row  row = rows.Item(1);
	Cells cells = row.GetCells();
	Cell cell = cells.Item(2);//第2列
	Range r = cell.GetRange();
	r.SetText(m_IRStationName);

	cell = cells.Item(4);//第4列
	r = cell.GetRange();
	r.SetText(m_IRRunNum);

	cell = cells.Item(6);//第6列
	r = cell.GetRange();
	r.SetText(m_IRClient);

	cell = cells.Item(8);//第8列
	r = cell.GetRange();
	r.SetText(m_IRTestAgn);

	//第2行
	row = rows.Item(2);
	cells = row.GetCells();
	cell = cells.Item(2);//第2列
	r = cell.GetRange();
	r.SetText(m_IRPilot);

	cell = cells.Item(4);//第4列
	r = cell.GetRange();
	r.SetText(m_IRTestDate);

	cell = cells.Item(6);//第6列
	r = cell.GetRange();
	r.SetText(m_IRTester);

	cell = cells.Item(8);//第8列
	r = cell.GetRange();
	r.SetText(m_IRTestSpot);

	//第3行
	row = rows.Item(3);
	cells = row.GetCells();
	cell = cells.Item(2);//第2列
	r = cell.GetRange();
	r.SetText(m_IRReportDate);

	cell = cells.Item(4);//第4列
	r = cell.GetRange();
	r.SetText(m_IRReporter);

	cell = cells.Item(6);//第6列
	r = cell.GetRange();
	r.SetText(m_IRAuditor);

	cell = cells.Item(8);//第8列
	r = cell.GetRange();
	r.SetText(m_IRApprover);

	////*第二个表格*////
	Table m_table2 = m_tables.Item(2);
	rows = m_table2.GetRows();

	//第1行
	row = rows.Item(1);
	cells = row.GetCells();
	cell = cells.Item(2);//第2列
	r = cell.GetRange();
	r.SetText(m_IRManufacturer);

	cell = cells.Item(4);//第4列
	r = cell.GetRange();
	r.SetText(m_IROperateDate);

	cell = cells.Item(6);//第6列
	r = cell.GetRange();
	r.SetText(m_IRUintType);

	//第2行
	row = rows.Item(2);
	cells = row.GetCells();
	cell = cells.Item(2);//第2列
	r = cell.GetRange();
	r.SetText(m_IRDetectSite);

	cell = cells.Item(4);//第4列
	r = cell.GetRange();
	r.SetText(m_IRWeather);

	cell = cells.Item(6);//第6列
	r = cell.GetRange();
	m_IRWindSpeed = m_IRWindSpeed + _T("m/s");
	r.SetText(m_IRWindSpeed);

	//第3行
	row = rows.Item(3);
	cells = row.GetCells();
	cell = cells.Item(2);//第2列
	r = cell.GetRange();
	m_IRTestDis = m_IRTestDis + _T("m");
	r.SetText(m_IRTestDis);

	cell = cells.Item(4);//第4列
	r = cell.GetRange();
	r.SetText(m_IRRadiation);

	cell = cells.Item(6);//第6列
	r = cell.GetRange();
	m_IRLoadCurrent = m_IRLoadCurrent + _T("A");
	r.SetText(m_IRLoadCurrent);

	//第4行
	row = rows.Item(4);
	cells = row.GetCells();
	cell = cells.Item(2);//第2列
	r = cell.GetRange();
	m_IRRatedCurrent = m_IRRatedCurrent + _T("A");
	r.SetText(m_IRRatedCurrent);

	cell = cells.Item(4);//第4列
	r = cell.GetRange();
	m_IRTemperature = m_IRTemperature + _T("℃");
	r.SetText(m_IRTemperature);

	cell = cells.Item(6);//第6列
	r = cell.GetRange();
	m_IRHumidity = m_IRHumidity + _T("%");
	r.SetText(m_IRHumidity);


	////*第三个表格*////
	Table m_table3 = m_tables.Item(3);
	rows = m_table3.GetRows();

	for (int i=3; i<3+IRUnsualData.size(); i++)
	{
		if (i <= 7)
		{
			row = rows.Item(i);
			cells = row.GetCells();
			cell = cells.Item(1);//第1列
			r = cell.GetRange();
			r.SetText(IRUnsualData[i-3].ID);

			cell = cells.Item(2);//第2列
			r = cell.GetRange();
			r.SetText(IRUnsualData[i-3].DifTemper);
		}
	}
	//第8行
	row = rows.Item(8);
	cells = row.GetCells();
	cell = cells.Item(2);//第2列
	r = cell.GetRange();
	r.SetText(IRUnsualData[0].DifTemper);

	//第9行
	row = rows.Item(9);
	cells = row.GetCells();
	cell = cells.Item(2);//第2列
	r = cell.GetRange();
	r.SetText(IRUnsualData[IRUnsualData.size()-1].DifTemper);

	//第11行
	row = rows.Item(11);
	cells = row.GetCells();
	cell = cells.Item(1);//第2列
	r = cell.GetRange();
	r.SetText(m_IRFaultExpress);

	//第13行
	row = rows.Item(13);
	cells = row.GetCells();
	cell = cells.Item(1);//第2列
	r = cell.GetRange();
	r.SetText(IRUnsualData[0].FaultLevel);


	////*第四个表格*////
	Table m_table4 = m_tables.Item(4);
	rows = m_table4.GetRows();

	//第2行
	row = rows.Item(2);
	cells = row.GetCells();
	cell = cells.Item(1);//第1列
	r = cell.GetRange();
	r.SetText(m_IRUnitExpress);

	cells = row.GetCells();
	cell = cells.Item(2);//第2列 插入图片
	r = cell.GetRange();
	///////////////////////////////////

	/////////////插入图片//////////////////////////
	IplImage* IR_Insert = cvCreateImage(cvSize(420,315),ReportImg->depth,ReportImg->nChannels);
	if (ImgType == 1)
	{
		cvResize(ReportImg,IR_Insert,CV_INTER_CUBIC);
	}
	else if (ImgType == 2)
	{
		IplImage* SScalImg = cvCreateImage(cvSize(70,ReportImg->height),ScaleImg->depth,ScaleImg->nChannels);
		cvResize(ScaleImg,SScalImg,CV_INTER_CUBIC);
		IplImage* JtImg = cvCreateImage(cvSize(ReportImg->width+SScalImg->width,ReportImg->height),ReportImg->depth,ReportImg->nChannels);
		cvSetImageROI(JtImg,cvRect(0,0,ReportImg->width,ReportImg->height));
		cvCopy(ReportImg,JtImg);
		cvResetImageROI(JtImg);
		cvSetImageROI(JtImg,cvRect(ReportImg->width,0,SScalImg->width,SScalImg->height));
		cvCopy(SScalImg,JtImg);
		cvResetImageROI(JtImg);
		cvResize(JtImg,IR_Insert,CV_INTER_CUBIC);
	}
	cvSaveImage("IR_Insert.bmp",IR_Insert);
	CString strPicFile = strCurPath + "\\IR_Insert.bmp";

	//嵌入方式显示图片
	InlineShapes inlineshapes;
	LPDISPATCH pInlineShapes = r.GetInlineShapes();
	inlineshapes.AttachDispatch(pInlineShapes);
	CComVariant vtTrue(true), vtFalse(false), vtZero(0l), vtNone, vtNullString("");
	CComVariant vtOpn(DISP_E_PARAMNOTFOUND, VT_ERROR);
	inlineshapes.AddPicture(strPicFile, &vtFalse, &vtTrue, &vtOpn);
	inlineshapes.ReleaseDispatch();
	///////////////////////////////////////////////

	oActiveDoc.PrintPreview();		//预览，前提是m_App.SetVisible(TRUE);
	DeleteFile(strPicFile);
	cvReleaseImage(&IR_Insert);

	IR_Complete_Flag = TRUE;

	return;
	m_Docs.ReleaseDispatch();//断开关联；
	m_Sel.ReleaseDispatch();

	//退出WORD
	m_App.Quit(covOptional, covOptional, covOptional); 
	m_App.Quit(covOptional, covOptional, covOptional);
	m_App.ReleaseDispatch();

	EndWaitCursor();

	//MessageBox(_T("word表格填写完毕!","提示"),MB_ICONEXCLAMATION);

	UpdateData(FALSE);
}

void CIRReportDlg::GetEditText()
{
	GetDlgItem(IDC_EDIT1)->GetWindowTextW(m_IRStationName);
	GetDlgItem(IDC_EDIT2)->GetWindowTextW(m_IRRunNum);
	GetDlgItem(IDC_EDIT3)->GetWindowTextW(m_IRClient);
	GetDlgItem(IDC_EDIT4)->GetWindowTextW(m_IRTestAgn);
	GetDlgItem(IDC_EDIT5)->GetWindowTextW(m_IRPilot);
	GetDlgItem(IDC_EDIT6)->GetWindowTextW(m_IRTestDate);
	GetDlgItem(IDC_EDIT7)->GetWindowTextW(m_IRTester);
	GetDlgItem(IDC_EDIT8)->GetWindowTextW(m_IRTestSpot);
	GetDlgItem(IDC_EDIT9)->GetWindowTextW(m_IRReportDate);
	GetDlgItem(IDC_EDIT10)->GetWindowTextW(m_IRReporter);
	GetDlgItem(IDC_EDIT11)->GetWindowTextW(m_IRAuditor);
	GetDlgItem(IDC_EDIT12)->GetWindowTextW(m_IRApprover);

	GetDlgItem(IDC_EDIT13)->GetWindowTextW(m_IRManufacturer);
	GetDlgItem(IDC_EDIT14)->GetWindowTextW(m_IROperateDate);
	GetDlgItem(IDC_EDIT15)->GetWindowTextW(m_IRUintType);
	GetDlgItem(IDC_EDIT16)->GetWindowTextW(m_IRDetectSite);
	GetDlgItem(IDC_EDIT17)->GetWindowTextW(m_IRWeather);
	GetDlgItem(IDC_EDIT18)->GetWindowTextW(m_IRWindSpeed);
	GetDlgItem(IDC_EDIT19)->GetWindowTextW(m_IRTestDis);
	GetDlgItem(IDC_EDIT20)->GetWindowTextW(m_IRRadiation);
	GetDlgItem(IDC_EDIT21)->GetWindowTextW(m_IRLoadCurrent);
	GetDlgItem(IDC_EDIT22)->GetWindowTextW(m_IRRatedCurrent);
	GetDlgItem(IDC_EDIT23)->GetWindowTextW(m_IRTemperature);
	GetDlgItem(IDC_EDIT24)->GetWindowTextW(m_IRHumidity);

	GetDlgItem(IDC_EDIT25)->GetWindowTextW(m_IRUnitExpress);
	GetDlgItem(IDC_EDIT26)->GetWindowTextW(m_IRInstrument);
	GetDlgItem(IDC_EDIT27)->GetWindowTextW(m_IRFaultExpress);

	m_IRStationName_save = m_IRStationName;
	m_IRRunNum_save = m_IRRunNum;
	m_IRClient_save = m_IRClient;
	m_IRTestAgn_save = m_IRTestAgn;
	m_IRPilot_save = m_IRPilot;
	m_IRTestDate_save = m_IRTestDate;
	m_IRTester_save = m_IRTester;
	m_IRTestSpot_save = m_IRTestSpot;
	m_IRReportDate_save = m_IRReportDate;
	m_IRReporter_save = m_IRReporter;
	m_IRAuditor_save = m_IRAuditor;
	m_IRApprover_save = m_IRApprover;

	m_IRManufacturer_save = m_IRManufacturer;
	m_IROperateDate_save = m_IROperateDate;
	m_IRUintType_save = m_IRUintType;
	m_IRDetectSite_save = m_IRDetectSite;
	m_IRWeather_save = m_IRWeather;
	m_IRWindSpeed_save = m_IRWindSpeed;
	m_IRTestDis_save = m_IRTestDis;
	m_IRRadiation_save = m_IRRadiation;
	m_IRLoadCurrent_save = m_IRLoadCurrent;
	m_IRRatedCurrent_save = m_IRRatedCurrent;
	m_IRTemperature_save = m_IRTemperature;
	m_IRHumidity_save = m_IRHumidity;

	m_IRUnitExpress_save = m_IRUnitExpress;
	m_IRInstrument_save = m_IRInstrument;
	m_IRFaultExpress_save = m_IRFaultExpress;
}
HBRUSH CIRReportDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if (pWnd == this)
	{
		return m_IR_REPORT_brBk;
	}

	switch(pWnd->GetDlgCtrlID())   
	{   
	case IDC_STATIC1:   
		pDC->SetBkMode(TRANSPARENT);   
		pDC->SetTextColor(RGB(0,0,0));   
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);   
	case IDC_STATIC2:   
		pDC->SetBkMode(TRANSPARENT);   
		pDC->SetTextColor(RGB(0,0,0));   
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);   
	case IDC_STATIC3:   
		pDC->SetBkMode(TRANSPARENT);   
		pDC->SetTextColor(RGB(0,0,0));   
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);   
	case IDC_STATIC4:   
		pDC->SetBkMode(TRANSPARENT);   
		pDC->SetTextColor(RGB(0,0,0));   
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);   
	case IDC_STATIC5:   
		pDC->SetBkMode(TRANSPARENT);   
		pDC->SetTextColor(RGB(0,0,0));   
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);   
	case IDC_STATIC6:   
		pDC->SetBkMode(TRANSPARENT);   
		pDC->SetTextColor(RGB(0,0,0));   
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);   
	case IDC_STATIC7:   
		pDC->SetBkMode(TRANSPARENT);   
		pDC->SetTextColor(RGB(0,0,0));   
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);   
	case IDC_STATIC8:   
		pDC->SetBkMode(TRANSPARENT);   
		pDC->SetTextColor(RGB(0,0,0));   
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);   
	case IDC_STATIC9:   
		pDC->SetBkMode(TRANSPARENT);   
		pDC->SetTextColor(RGB(0,0,0));   
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);   
	case IDC_STATIC10:   
		pDC->SetBkMode(TRANSPARENT);   
		pDC->SetTextColor(RGB(0,0,0));   
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);   

	default:   
		break;   
	}   



	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CIRReportDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
	OnCancel();
}
