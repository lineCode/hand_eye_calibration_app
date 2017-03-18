// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "hand_eye_calibration_app.h"
#include "hand_eye_calibration_appDoc.h"
#include "hand_eye_calibration_appView.h"
#include "MainFrm.h"
#include "PatternDlg.h"
#include "HandeyeParaDlg.h"
#include "FormView1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;
using namespace std;

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_EDIT_ADDIMAGES, &CMainFrame::OnEditAddimages)
	ON_COMMAND(ID_PATTERN_ATTRIBUTE, &CMainFrame::OnPatternAttribute)
	ON_COMMAND(ID_DETECT_CORNERS, &CMainFrame::OnDetectCorners)
	ON_COMMAND(ID_CALIBRATION, &CMainFrame::OnCalibration)
	ON_COMMAND(ID_HANDEYE_PARAMETER, &CMainFrame::OnHandeyeParameter)
	ON_COMMAND(ID_HANDEYECALIB, &CMainFrame::OnHandeyecalib)
END_MESSAGE_MAP()

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO:  �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // ����������ʽ...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // ����Ϊ FALSE �Ὣ�رհ�ť������ѡ�������Ҳ�
	mdiTabParams.m_bTabIcons = FALSE;    // ����Ϊ TRUE ���� MDI ѡ��������ĵ�ͼ��
	mdiTabParams.m_bAutoColor = TRUE;    // ����Ϊ FALSE ������ MDI ѡ����Զ���ɫ
	mdiTabParams.m_bDocumentMenu = TRUE; // ��ѡ�������ұ�Ե�����ĵ��˵�
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// �������񽫴�������࣬��˽���ʱ��������ͣ��: 
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);

	// ���������á�Outlook��������: 
	//if (!CreateOutlookBar(m_wndNavigationBar, ID_VIEW_NAVIGATION, m_wndTree, m_wndCalendar, 250))
	//{
	//	TRACE0("δ�ܴ�����������\n");
	//	return -1;      // δ�ܴ���
	//}

	// �Ѵ��� Outlook ����Ӧ���������ͣ����
	EnableDocking(CBRS_ALIGN_LEFT);
	EnableAutoHidePanes(CBRS_ALIGN_RIGHT);

	// ����ͣ������
	if (!CreateDockingWindows())
	{
		TRACE0("δ�ܴ���ͣ������\n");
		return -1;
	}

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);

	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	// ������ǿ�Ĵ��ڹ���Ի���
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// ���ĵ�����Ӧ�ó��������ڴ��ڱ������ϵ�˳����н�������
	// ���Ľ��������Ŀ����ԣ���Ϊ��ʾ���ĵ�����������ͼ��
	ModifyStyle(0, FWS_PREFIXTITLE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;
	// �����������
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ����������\n");
		return FALSE; // δ�ܴ���
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	UpdateMDITabbedBarsIcons();
}

BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMFCShellTreeCtrl& tree, CCalendarBar& calendar, int nInitialWidth)
{
	bar.SetMode2003();

	BOOL bNameValid;
	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_SHORTCUTS);
	ASSERT(bNameValid);
	if (!bar.Create(strTemp, this, CRect(0, 0, nInitialWidth, 32000), uiID, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
	{
		return FALSE; // δ�ܴ���
	}

	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();

	if (pOutlookBar == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pOutlookBar->EnableInPlaceEdit(TRUE);

	static UINT uiPageID = 1;

	// �ɸ��������Զ����أ��ɵ�����С�������ܹر�
	DWORD dwStyle = AFX_CBRS_FLOAT | AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE;

	CRect rectDummy(0, 0, 0, 0);
	const DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	tree.Create(dwTreeStyle, rectDummy, &bar, 1200);
	bNameValid = strTemp.LoadString(IDS_FOLDERS);
	ASSERT(bNameValid);
	pOutlookBar->AddControl(&tree, strTemp, 2, TRUE, dwStyle);

	calendar.Create(rectDummy, &bar, 1201);
	bNameValid = strTemp.LoadString(IDS_CALENDAR);
	ASSERT(bNameValid);
	pOutlookBar->AddControl(&calendar, strTemp, 3, TRUE, dwStyle);

	bar.SetPaneStyle(bar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	pOutlookBar->SetImageList(theApp.m_bHiColorIcons ? IDB_PAGES_HC : IDB_PAGES, 24);
	pOutlookBar->SetToolbarImageList(theApp.m_bHiColorIcons ? IDB_PAGES_SMALL_HC : IDB_PAGES_SMALL, 16);
	pOutlookBar->RecalcLayout();

	BOOL bAnimation = theApp.GetInt(_T("OutlookAnimation"), TRUE);
	CMFCOutlookBarTabCtrl::EnableAnimation(bAnimation);

	bar.SetButtonsFont(&afxGlobalData.fontBold);

	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	m_wndOutput.UpdateFonts();
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}


void CMainFrame::OnEditAddimages()
{
	// TODO:  �ڴ���������������
	// ������ļ��Ի���   
	CFileDialog AddImagesDlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, NULL, this);
	const int nMaxFiles = 20;
	const int nMaxPathBuffer = (nMaxFiles * (MAX_PATH + 1)) + 1;
	LPWSTR pc = (LPWSTR)malloc(nMaxPathBuffer * sizeof(WCHAR));
	//std::vector<CString> image_list;

	if (pc)
	{
		AddImagesDlg.GetOFN().lpstrFile = pc;
		AddImagesDlg.GetOFN().lpstrFile[0] = NULL;
		AddImagesDlg.m_ofn.nMaxFile = nMaxPathBuffer;
		// ��ʾ���ļ��Ի���   
		if (IDOK == AddImagesDlg.DoModal())
		{
			// ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����
			POSITION  pos = AddImagesDlg.GetStartPosition();
			while (pos != NULL)
			{
				CString temp = AddImagesDlg.GetNextPathName(pos);
				show_imageList.push_back(temp);//ȡ���ļ�·�� ΪCString����

				int iSize;
				iSize = WideCharToMultiByte(CP_ACP, 0, temp.GetBuffer(), -1, NULL, 0, NULL, NULL);
				char *chr = (char*)malloc((iSize + 1));
				WideCharToMultiByte(CP_ACP, 0, temp.GetBuffer(), -1, chr, iSize, NULL, NULL);

				string str = chr;
				imageList.push_back(str);//����Ϊstring����
				free (chr);
			}
		}
		free(pc);
	}
}


void CMainFrame::OnPatternAttribute()
{
	// TODO:  �ڴ���������������
	INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ   

	CPatternDlg patternDlg;       // ����Ի�����CPatternDlg��ʵ��   
	nRes = patternDlg.DoModal();  // �����Ի���
	if (IDCANCEL == nRes)     // �ж϶Ի����˳��󷵻�ֵ�Ƿ�ΪIDCANCEL���������return�������������ִ��   
		return;

	UpdateData(TRUE);
	//get pattern type
	if (patternDlg.m_IsChess == 0)
		pattern = CHESSBOARD;
	else
		pattern = CIRCLES_GRID;
	//get pattern's edge color
	if (patternDlg.m_IsWhite == 0)
		edge_is_white = TRUE;
	else
		edge_is_white = FALSE;
	//get pattern square size
	squareSize = patternDlg.m_SquareSize;
	//get boardSize
	boardSize.width = patternDlg.m_PatternWidth;
	boardSize.height = patternDlg.m_PatternHeight;

}


void CMainFrame::OnDetectCorners()
{
	// TODO:  �ڴ���������������
	int i;

	if (!imageList.empty())
		nframes = (int)imageList.size();

	namedWindow("Image View", 1);

	for (i = 0;; i++)
	{
		Mat view, viewGray;

		if (i < (int)imageList.size())
			view = imread(imageList[i], 1);

		if (!view.data)
		{
			break;
		}

		imageSize = view.size();

		vector<Point2f> pointbuf;
		cvtColor(view, viewGray, COLOR_BGR2GRAY);
		Mat view_1;
		if (edge_is_white)
			view_1 = view;
		else{
			bitwise_not(view, view_1);
		}

		bool found;
		switch (pattern)
		{
		case CHESSBOARD:
			found = findChessboardCorners(view_1, boardSize, pointbuf,
				CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);
			break;
		case CIRCLES_GRID:
			found = findCirclesGrid(view_1, boardSize, pointbuf);
			break;
		default:;
		}

		// improve the found corners' coordinate accuracy
		if (pattern == CHESSBOARD && found) cornerSubPix(viewGray, pointbuf, Size(11, 11),
			Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

		if (mode == CAPTURING && found )
		{
			imagePoints.push_back(pointbuf);
		}

		if (found)
			drawChessboardCorners(view, boardSize, Mat(pointbuf), found);

		string msg = mode == CAPTURING ? "100/100" :
			mode == CALIBRATED ? "Calibrated" : "Press 'g' to start";
		int baseLine = 0;
		Size textSize = getTextSize(msg, 1, 1, 1, &baseLine);
		Point textOrigin(view.cols - 2 * textSize.width - 10, view.rows - 2 * baseLine - 10);

		if (mode == CAPTURING)
		{
			if (undistortImage)
				msg = format("%d/%d Undist", (int)imagePoints.size(), nframes);
			else
				msg = format("%d/%d", (int)imagePoints.size(), nframes);
		}

		putText(view, msg, textOrigin, 1, 1,
			mode != CALIBRATED ? Scalar(0, 0, 255) : Scalar(0, 255, 0));

		if (mode == CALIBRATED && undistortImage)
		{
			Mat temp = view.clone();
			undistort(temp, view, cameraMatrix, distCoeffs);
		}

		imshow("Image View", view);
		
		image_show.push_back(view);
		if (found){
			view.resize(300, 400);
			image_corner.push_back(view);
			s_imageList.push_back(show_imageList[i]);
		}
		/*else{
			s_imageList.erase(s_imageList.begin() + i);
		}*/
	}
}


static double computeReprojectionErrors(
	const vector<vector<Point3f> >& objectPoints,
	const vector<vector<Point2f> >& imagePoints,
	const vector<Mat>& rvecs, const vector<Mat>& tvecs,
	const Mat& cameraMatrix, const Mat& distCoeffs,
	vector<float>& perViewErrors)
{
	vector<Point2f> imagePoints2;
	int i, totalPoints = 0;
	double totalErr = 0, err;
	perViewErrors.resize(objectPoints.size());

	for (i = 0; i < (int)objectPoints.size(); i++)
	{
		projectPoints(Mat(objectPoints[i]), rvecs[i], tvecs[i],
			cameraMatrix, distCoeffs, imagePoints2);
		err = norm(Mat(imagePoints[i]), Mat(imagePoints2), CV_L2);
		int n = (int)objectPoints[i].size();
		perViewErrors[i] = (float)std::sqrt(err*err / n);
		totalErr += err*err;
		totalPoints += n;
	}

	return std::sqrt(totalErr / totalPoints);
}


static void calcChessboardCorners(Size boardSize, float squareSize, vector<Point3f>& corners, Pattern patternType = CHESSBOARD)
{
	corners.resize(0);

	switch (patternType)
	{
	case CHESSBOARD:
	case CIRCLES_GRID:
		for (int i = 0; i < boardSize.height; i++)
		for (int j = 0; j < boardSize.width; j++)
			corners.push_back(Point3f(float(j*squareSize),
			float(i*squareSize), 0));
		break;

	default:;
	}
}

static bool runCalibration(vector<vector<Point2f> > imagePoints,
	Size imageSize, Size boardSize, Pattern patternType,
	float squareSize, float aspectRatio,
	int flags, Mat& cameraMatrix, Mat& distCoeffs,
	vector<Mat>& rvecs, vector<Mat>& tvecs,
	vector<float>& reprojErrs,
	double& totalAvgErr)
{
	cameraMatrix = Mat::eye(3, 3, CV_64F);
	if (flags & CV_CALIB_FIX_ASPECT_RATIO)
		cameraMatrix.at<double>(0, 0) = aspectRatio;

	distCoeffs = Mat::zeros(8, 1, CV_64F);

	vector<vector<Point3f> > objectPoints(1);
	calcChessboardCorners(boardSize, squareSize, objectPoints[0], patternType);

	objectPoints.resize(imagePoints.size(), objectPoints[0]);

	double rms = calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix,
		distCoeffs, rvecs, tvecs, flags | CV_CALIB_FIX_K4 | CV_CALIB_FIX_K5);
	///*|CV_CALIB_FIX_K3*/|CV_CALIB_FIX_K4|CV_CALIB_FIX_K5);
	printf("RMS error reported by calibrateCamera: %g\n", rms);

	bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);

	totalAvgErr = computeReprojectionErrors(objectPoints, imagePoints,
		rvecs, tvecs, cameraMatrix, distCoeffs, reprojErrs);

	return ok;
}


static void saveCameraParams(const string& filename,
	Size imageSize, Size boardSize,
	float squareSize, float aspectRatio, int flags,
	const Mat& cameraMatrix, const Mat& distCoeffs,
	const vector<Mat>& rvecs, const vector<Mat>& tvecs,
	const vector<float>& reprojErrs,
	const vector<vector<Point2f> >& imagePoints,
	double totalAvgErr)
{
	FileStorage fs(filename, FileStorage::WRITE);

	time_t tt;
	time(&tt);
	struct tm *t2 = localtime(&tt);
	char buf[1024];
	strftime(buf, sizeof(buf)-1, "%c", t2);

	fs << "calibration_time" << buf;

	if (!rvecs.empty() || !reprojErrs.empty())
		fs << "nframes" << (int)std::max(rvecs.size(), reprojErrs.size());
	fs << "image_width" << imageSize.width;
	fs << "image_height" << imageSize.height;
	fs << "board_width" << boardSize.width;
	fs << "board_height" << boardSize.height;
	fs << "square_size" << squareSize;

	if (flags & CV_CALIB_FIX_ASPECT_RATIO)
		fs << "aspectRatio" << aspectRatio;

	if (flags != 0)
	{
		sprintf(buf, "flags: %s%s%s%s",
			flags & CV_CALIB_USE_INTRINSIC_GUESS ? "+use_intrinsic_guess" : "",
			flags & CV_CALIB_FIX_ASPECT_RATIO ? "+fix_aspectRatio" : "",
			flags & CV_CALIB_FIX_PRINCIPAL_POINT ? "+fix_principal_point" : "",
			flags & CV_CALIB_ZERO_TANGENT_DIST ? "+zero_tangent_dist" : "");
		cvWriteComment(*fs, buf, 0);
	}

	fs << "flags" << flags;

	fs << "camera_matrix" << cameraMatrix;
	fs << "distortion_coefficients" << distCoeffs;

	fs << "avg_reprojection_error" << totalAvgErr;
	if (!reprojErrs.empty())
		fs << "per_view_reprojection_errors" << Mat(reprojErrs);

	if (!rvecs.empty() && !tvecs.empty())
	{
		CV_Assert(rvecs[0].type() == tvecs[0].type());
		Mat bigmat((int)rvecs.size(), 6, rvecs[0].type());
		for (int i = 0; i < (int)rvecs.size(); i++)
		{
			Mat r = bigmat(Range(i, i + 1), Range(0, 3));
			Mat t = bigmat(Range(i, i + 1), Range(3, 6));

			CV_Assert(rvecs[i].rows == 3 && rvecs[i].cols == 1);
			CV_Assert(tvecs[i].rows == 3 && tvecs[i].cols == 1);
			//*.t() is MatExpr (not Mat) so we can use assignment operator
			r = rvecs[i].t();
			t = tvecs[i].t();
		}
		cvWriteComment(*fs, "a set of 6-tuples (rotation vector + translation vector) for each view", 0);
		fs << "extrinsic_parameters" << bigmat;
	}

	if (!imagePoints.empty())
	{
		Mat imagePtMat((int)imagePoints.size(), (int)imagePoints[0].size(), CV_32FC2);
		for (int i = 0; i < (int)imagePoints.size(); i++)
		{
			Mat r = imagePtMat.row(i).reshape(2, imagePtMat.cols);
			Mat imgpti(imagePoints[i]);
			imgpti.copyTo(r);
		}
		fs << "image_points" << imagePtMat;
	}
}

//static bool runAndSave(const string& outputFilename,
//	const vector<vector<Point2f> >& imagePoints,
//	Size imageSize, Size boardSize, Pattern patternType, float squareSize,
//	float aspectRatio, int flags, Mat& cameraMatrix,
//	Mat& distCoeffs, bool writeExtrinsics, bool writePoints)
//{
//	vector<Mat> rvecs, tvecs;
//	vector<float> reprojErrs;
//	double totalAvgErr = 0;
//
//	bool ok = runCalibration(imagePoints, imageSize, boardSize, patternType, squareSize,
//		aspectRatio, flags, cameraMatrix, distCoeffs,
//		rvecs, tvecs, reprojErrs, totalAvgErr);
//	printf("%s. avg reprojection error = %.2f\n",
//		ok ? "Calibration succeeded" : "Calibration failed",
//		totalAvgErr);
//
//	if (ok)
//		saveCameraParams(outputFilename, imageSize,
//		boardSize, squareSize, aspectRatio,
//		flags, cameraMatrix, distCoeffs,
//		writeExtrinsics ? rvecs : vector<Mat>(),
//		writeExtrinsics ? tvecs : vector<Mat>(),
//		writeExtrinsics ? reprojErrs : vector<float>(),
//		writePoints ? imagePoints : vector<vector<Point2f> >(),
//		totalAvgErr);
//	return ok;
//}


void CMainFrame::OnCalibration()
{
	// TODO:  �ڴ���������������
	/*runAndSave(outputFilename, imagePoints, imageSize,
		boardSize, pattern, squareSize, aspectRatio,
		flags, cameraMatrix, distCoeffs,
		writeExtrinsics, writePoints);*/
	
	vector<float> reprojErrs;
	double totalAvgErr = 0;

	bool ok = runCalibration(imagePoints, imageSize, boardSize, pattern, squareSize,
		aspectRatio, flags, cameraMatrix, distCoeffs,
		rvecs, tvecs, reprojErrs, totalAvgErr);
	printf("%s. avg reprojection error = %.2f\n",
		ok ? "Calibration succeeded" : "Calibration failed",
		totalAvgErr);

	if (ok)
		saveCameraParams(outputFilename, imageSize,
		boardSize, squareSize, aspectRatio,
		flags, cameraMatrix, distCoeffs,
		writeExtrinsics ? rvecs : vector<Mat>(),
		writeExtrinsics ? tvecs : vector<Mat>(),
		writeExtrinsics ? reprojErrs : vector<float>(),
		writePoints ? imagePoints : vector<vector<Point2f> >(),
		totalAvgErr);

	/*vector<Mat>::iterator iter_r = rvecs.begin(), iter_t = tvecs.begin();
	for (; iter_r != rvecs.end(); iter_r++, iter_t++){
		Mat r;
		Rodrigues(*iter_r, r);
		R.push_back(r);
		T.push_back(*iter_t);
	}*/
}


void CMainFrame::OnHandeyeParameter()
{
	// TODO:  �ڴ���������������
	INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ   
	CHandeyeParaDlg handeyeparaDlg;       // ����Ի�����CHandeyeParaDlg��ʵ��

	handeyeparaDlg.s_imageList = s_imageList;
	handeyeparaDlg.image_corner = image_corner;
	handeyeparaDlg.rvecs = rvecs;
	handeyeparaDlg.tvecs = tvecs;
	nRes = handeyeparaDlg.DoModal();  // �����Ի���
	if (IDCANCEL == nRes)     // �ж϶Ի����˳��󷵻�ֵ�Ƿ�ΪIDCANCEL���������return�������������ִ��   
		return;

	A = handeyeparaDlg.A;
	B = handeyeparaDlg.B;
}

void tensor_product(Mat a, Mat b, Mat& result)
{
	int height = a.rows;
	int width = a.cols;
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			result(Rect(i*width, j*height, width, height)) = a.at<float>(i, j)*b;
		}
	}
}

void schmidt(Mat input)
{
	Mat a1(3, 1, CV_32F, Scalar(0));
	Mat a2(3, 1, CV_32F, Scalar(0));
	Mat a3(3, 1, CV_32F, Scalar(0));
	Mat b1, b2, b3, c1, c2, c3;

	float* p_row0 = input.ptr<float>(0);
	float* p_row1 = input.ptr<float>(1);
	float* p_row2 = input.ptr<float>(2);
	float mo;
	for (int i = 0; i < 3; i++){
		a1.at<float>(i, 0) = p_row0[i];
		a2.at<float>(i, 0) = p_row1[i];
		a3.at<float>(i, 0) = p_row2[i];
	}

	b1 = a1;
	mo = sqrt(b1.dot(b1));
	c1 = b1 / mo;

	b2 = a2 - (a2.dot(c1))*c1;
	mo = sqrt(b2.dot(b2));
	c2 = b2 / mo;

	b3 = a3 - (a3.dot(c1))*c1 - (c3.dot(c2))*c2;
	mo = sqrt(b3.dot(b3));
	c3 = b3 / mo;

	for (int i = 0; i < 3; i++){
		p_row0[i] = a1.at<float>(i, 0);
		p_row1[i] = a2.at<float>(i, 0);
		p_row2[i] = a3.at<float>(i, 0);
	}
}

void CMainFrame::OnHandeyecalib()
{
	// TODO:  �ڴ���������������
	if (A.size() < 2)
		return;
	//���R
	int height = 9 * (int)A.size();
	int width = 9;
	Mat S(height, width, CV_32F, Scalar(0));
	for (int i = 0; i < (int)A.size(); i++){
		Mat s1(9, 9, CV_32F, Scalar(0));
		Mat s2(9, 9, CV_32F, Scalar(0));
		Mat I(3, 3, CV_32F, Scalar(0));
		Mat r1(A[i], Rect(0, 0, 3, 3));
		Mat r2(B[i], Rect(0, 0, 3, 3));

		for (int j = 0; j < 3; j++){
			I.at<float>(j, j) = 1;
		}
		tensor_product(r1, I, s1);
		tensor_product(I, r2.t(), s2);
		S(Rect(0, 9 * i, 9, 9)) = s1 - s2;
	}

	Mat u, v, w;
	SVD::compute(S.t()*S, w, u, v);
	R.create(3, 3, CV_32F);
	float* v_lastrow = v.ptr<float>(8);
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			R.at<float>(i, j) = v_lastrow[i * 3 + j];
		}
	}
	schmidt(R);

	//���T
	Mat C(3 * A.size(), 3, CV_32F, Scalar(0));
	Mat D(3 * A.size(), 1, CV_32F, Scalar(0));
	for (int i = 0; i < (int)A.size(); i++){
		Mat r1(A[i], Rect(0, 0, 3, 3));
		Mat t1(A[i], Rect(3, 0, 1, 3));
		Mat t2(B[i], Rect(3, 0, 1, 3));
		Mat I(3, 3, CV_32F, Scalar(0));
		for (int j = 0; j < 3; j++){
			I.at<float>(j, j) = 1;
		}
		C(Rect(0, 3 * i, 3, 3)) = r1 - I;
		D(Rect(0, 3 * i, 1, 3)) = R*t2 - t1;
	}


	T.create(3, 1, CV_32F);
	T = ((C.t()*C).inv())*C.t()*D;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO:  �ڴ����ר�ô����/����û���
	CRect rc;

	// ��ȡ��ܴ��ڿͻ�����CRect����   
	GetClientRect(&rc);

	//������̬�ָ�ڣ�һ������   
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	// �������洰���е���ͼ   
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(Chand_eye_calibration_appView), CSize(rc.Width() / 2, rc.Height()), pContext))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(FormView1), CSize(rc.Width() / 2, rc.Height()), pContext))
		return FALSE;

//	return TRUE;
	return CMDIFrameWndEx::OnCreateClient(lpcs, pContext);
}
