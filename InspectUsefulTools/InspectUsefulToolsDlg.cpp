/*! @file  InspectUsefulToolsDlg.cpp
    @brief 当ツールメイン画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "InspectUsefulToolsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const ULONG CInspectUsefulToolsDlg::m_uWMNotifyIcon = ::RegisterWindowMessage(_T("__NotifyIconRegMsg__"));  
const ULONG CInspectUsefulToolsDlg::m_uWMTaskBarCreated = ::RegisterWindowMessage(_T("__TaskbarCreated__"));  

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CInspectUsefulToolsDlg::CInspectUsefulToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInspectUsefulToolsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bOpenXL = FALSE;
	m_bOpenBook = FALSE;
	strTempFilePath = "";
}

/*!
 @brief ダイアログ データの交換と有効性チェックのためにフレームワークが呼び出します。

 @param [in]    pDX     CDataExchange オブジェクトへのポインタ
*/
void CInspectUsefulToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MENU_TREE, m_inMenuTreeCtrl);
	DDX_Control(pDX, IDC_CONTENTS_TAB, m_inContentsTabCtrl);
}

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CInspectUsefulToolsDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_MENU_TREE, &CInspectUsefulToolsDlg::OnTvnSelchangedMenuTree)
	ON_WM_DROPFILES()
	ON_WM_CLOSE()
	ON_REGISTERED_MESSAGE(m_uWMTaskBarCreated, &CInspectUsefulToolsDlg::OnTaskBarCreated)
	ON_REGISTERED_MESSAGE(m_uWMNotifyIcon, &CInspectUsefulToolsDlg::OnNotifyTaskBarIcon)
	ON_COMMAND(ID_MENU_CLOSE, &CInspectUsefulToolsDlg::OnMenuClose)
END_MESSAGE_MAP()

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CInspectUsefulToolsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	//SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// メニューの初期化
	InitializeMenuTree();
	InitializeContents();

	// タイトルの設定
	CString strAppName = CString((LPCTSTR)IDS_APP_NAME);
	this->SetWindowText(strAppName);

	CLSID clsid;
	if (CLSIDFromProgID(OLESTR("Excel.Application"), &clsid) == NOERROR){
		// Excel がインストールされている
		LPUNKNOWN lpUnk;
		HRESULT hr;
		LPDISPATCH lpDispatch;
		BOOL bOpenExcel = FALSE;
		if (GetActiveObject(clsid, NULL, &lpUnk) == NOERROR){
			hr = lpUnk->QueryInterface(IID_IDispatch, (LPVOID*)&lpDispatch);
			lpUnk->Release();
			if (hr == NOERROR) {
				//すでにExcelが起動されている状態であればAttachDispatch
				m_inXLApp.AttachDispatch(lpDispatch,TRUE);
				bOpenExcel = TRUE;
			}
		}

		if (m_inXLApp.m_lpDispatch == NULL) {
			bOpenExcel = m_inXLApp.CreateDispatch(clsid);
			m_bOpenXL = TRUE;
		}

		if (bOpenExcel == TRUE) {
			// Excelファイルの初期化
			CWorkbooks inWorkbooks = m_inXLApp.get_Workbooks();
			COleVariant varNull;
			varNull.ChangeType(VT_NULL);

			HRSRC hrSrc = FindResource(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MACRO_EXCEL_RCDATA), RT_RCDATA);
			if (hrSrc != NULL) {
				HGLOBAL hGlobal = LoadResource(AfxGetApp()->m_hInstance, hrSrc);

				if (hGlobal != NULL) {
					char *rcData = (char *)LockResource(hGlobal);

					/* リソースサイズの取得 */
					DWORD dwSize = SizeofResource(AfxGetApp()->m_hInstance, hrSrc);

					TCHAR szFilePath[MAX_PATH];
					GetTempPath(MAX_PATH, szFilePath);
					TCHAR szFileName[MAX_PATH];
					GetTempFileName(szFilePath, _T("SubTools_"), 0, szFileName);

					CFile inFile;
					inFile.Open(szFileName, CFile::modeWrite);
					inFile.Write(rcData, dwSize);
					inFile.Close();
					strTempFilePath = szFileName;

					m_inXLBook = inWorkbooks.Open(szFileName
						, varNull, varNull, varNull, varNull, varNull, varNull, varNull, varNull, varNull, varNull, varNull, varNull,varNull, varNull);
					m_bOpenBook = TRUE;
				}
			}
		}
	}

	return TRUE;
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CInspectUsefulToolsDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
        return 0;
        //  必ず０で戻ること!!
        //  0以外で戻ると各コントロールへ次の処理が回らなくなる
    }
    return CDialog::PreTranslateMessage(pMsg);
}

/*!
 @brief 描画イベントが発生した場合呼びだされます
*/
void CInspectUsefulToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

/*!
 @brief ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、\n
  システムがこの関数を呼び出します。
*/
HCURSOR CInspectUsefulToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*!
 @brief ウィンドウを閉じる際に呼び出されます。
*/
void CInspectUsefulToolsDlg::OnClose()
{
	if (m_bOpenBook == TRUE) {
		// Excel Bookが開いている場合は、閉じる
		COleVariant varNull;
		varNull.ChangeType(VT_NULL);

		try {
			m_inXLBook.Close(varNull, varNull, varNull);
		} catch (...) {

		}
		m_inXLBook.ReleaseDispatch();
		m_bOpenBook = FALSE;
	}

	if (m_bOpenXL == TRUE) {
		// Excelが開いている場合は、閉じる
		try {
			m_inXLApp.Quit();
		} catch (...) {

		}
		m_inXLApp.ReleaseDispatch();
		m_bOpenXL = FALSE;
	}


	if (strTempFilePath.IsEmpty() == FALSE) {
		// 一時ファイルを作成した場合は削除
		CFile::Remove(strTempFilePath);
	}

	DeleteNotifyIcon();

	CDialog::OnClose();
}

/*!
 @brief リサイズイベント\n
 WM_SIZEで呼び出されます。

 @param [in]    nType  リサイズタイプ
 @param [in]    cx     幅
 @param [in]    cy     高さ
 */
void CInspectUsefulToolsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// ツリービューの位置調整
	RECT stRect;
	if (m_inMenuTreeCtrl.m_hWnd != NULL) {
		m_inMenuTreeCtrl.GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		stRect.bottom = stRect.top + cy;
		m_inMenuTreeCtrl.MoveWindow(&stRect);
	}

	if (m_inContentsTabCtrl.m_hWnd != NULL) {
		m_inContentsTabCtrl.GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		stRect.bottom = stRect.top + cy;
		stRect.right = cx - 2;
		m_inContentsTabCtrl.MoveWindow(&stRect);

		CRect    rect;
		m_inContentsTabCtrl.GetClientRect(&rect);
		rect.OffsetRect(1, 3);
		rect.right -= 10;
		rect.bottom -= 9;

		POSITION nMapPosition = m_mapInnerDlg.GetStartPosition();
		while (nMapPosition != NULL) {
			CString strKey;
			void *pDialog;

			m_mapInnerDlg.GetNextAssoc(nMapPosition, strKey, pDialog);
			((CDialog *) pDialog)->MoveWindow(rect);
		}
	}

	if (nType == SIZE_MINIMIZED) {
		this->ShowWindow(SW_HIDE);
		AddNotifyIcon();
	} else {
		DeleteNotifyIcon();
	}
}

/*!
 @brief ツリーメニューの初期化
 　メニューを追加する場合は、この関数に処理を追加する
*/
BOOL CInspectUsefulToolsDlg::InitializeMenuTree()
{
	HTREEITEM hRootItem = m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_TOP));

	// システム情報
	HTREEITEM hSystemItem = m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_SYSTEM), hRootItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_SYSTEM_SYSINFO), hSystemItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_SYSTEM_ENVINFO), hSystemItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_SYSTEM_NETWORKINFO), hSystemItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_SYSTEM_DISKINFO), hSystemItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_SYSTEM_TASKINFO), hSystemItem);	
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_SYSTEM_EVENTLOG), hSystemItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_SYSTEM_USERINFO), hSystemItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_SYSTEM_USERLIST), hSystemItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_SYSTEM_SERVICE), hSystemItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_SYSTEM_PROCESSOR_SPEED), hSystemItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_SYSTEM_ERRORCODE), hSystemItem);

	// 時間
	HTREEITEM hTimeItem = m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_TIME), hRootItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_TIME_TIME_CHANGE), hTimeItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_TIME_STOP_WATCH), hTimeItem);

	// ファイル
	HTREEITEM hFileItem = m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_FILE), hRootItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_FILE_FILE_LOCK), hFileItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_FILE_FILE_TIMESTAMP), hFileItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_FILE_FILE_ATTR_CHANGE), hFileItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_FILE_FILE_TAIL), hFileItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_FILE_FILE_REPLACE), hFileItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_FILE_FILE_COMMONINFO), hFileItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_FILE_FOLDER_OPEN), hFileItem);

	// データ作成
	HTREEITEM hDataItem = m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_DUMMY_FILE), hRootItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_DUMMY_FILE_FILE_SIZE), hDataItem);

	// メモリー
	HTREEITEM hMemoryItem = m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_MEMORY), hRootItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_MEMORY_MEM_LEAK), hMemoryItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_MEMORY_GDI_MEM_LEAK), hMemoryItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_MEMORY_USER_MEM_LEAK), hMemoryItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_MEMORY_THREAD_MEM_LEAK), hMemoryItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_MEMORY_LOCK_MUTEX), hMemoryItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_MEMORY_LOCK_MAPFILE), hMemoryItem);	

	// 暗号化
	HTREEITEM hEncyptItem = m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_ENCRYPT), hRootItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_ENCRYPT_HASH_MD5), hEncyptItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_ENCRYPT_HASH_SHA1), hEncyptItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_ENCRYPT_HASH_SHA256), hEncyptItem);

	// ネットワーク
	HTREEITEM hNetworkItem = m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_NETWORK), hRootItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_NETWORK_PING), hNetworkItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_NETWORK_NSLOOKUP), hNetworkItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_NETWORK_TRACERT), hNetworkItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_NETWORK_NBTSTAT), hNetworkItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_NETWORK_NETSTAT), hNetworkItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_NETWORK_ARP), hNetworkItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_NETWORK_ROUTE), hNetworkItem);

	// イメージ
	HTREEITEM hImageItem = m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_IMAGE), hRootItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_IMAGE_FONT), hImageItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_IMAGE_MATH_GRAPH), hImageItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_IMAGE_CUR_INFO), hImageItem);

	// 表示
	HTREEITEM hMemoItem = m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_DISPLAY), hRootItem);
	m_inMenuTreeCtrl.InsertItem(CString((LPCTSTR) IDS_MENU_DISPLAY_MEMO), hMemoItem);


	// ツリーを開く
	m_inMenuTreeCtrl.Expand(hRootItem, (UINT) -1);

	return TRUE;
}

/*!
 @brief ツリーメニューに対応するコンテンツを定義する
 　ツリーメニューに追加した場合こちらも編集する
*/
BOOL CInspectUsefulToolsDlg::InitializeContents()
{
	CWnd *pWhd = &m_inContentsTabCtrl;

	m_inTopDlg.Create(IDD_TOP_TOOL_DESCRIPT_DIALOG, pWhd);
	m_mapInnerDlg.SetAt(CString((LPCTSTR) IDS_MENU_TOP), &m_inTopDlg);

	CString strTypeName;

	/// -----
	/// システム情報
	/// -----
	// システム情報
	strTypeName = CString((LPCTSTR) IDS_MENU_SYSTEM_SYSINFO);
	m_inSystemInfoDlg.SetInformationType(strTypeName);
	m_inSystemInfoDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inSystemInfoDlg);

	// 環境変数
	strTypeName = CString((LPCTSTR) IDS_MENU_SYSTEM_ENVINFO);
	m_inSystemEnvInfoDlg.SetInformationType(strTypeName);
	m_inSystemEnvInfoDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inSystemEnvInfoDlg);

	// ネットワーク情報
	strTypeName = CString((LPCTSTR) IDS_MENU_SYSTEM_NETWORKINFO);
	m_inSystemNetworkInfoDlg.SetInformationType(strTypeName);
	m_inSystemNetworkInfoDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inSystemNetworkInfoDlg);

	// ディスク情報
	strTypeName = CString((LPCTSTR) IDS_MENU_SYSTEM_TASKINFO);
	m_inSystemDiskInfoDlg.SetInformationType(strTypeName);
	m_inSystemDiskInfoDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inSystemDiskInfoDlg);
	
	// タスク情報
	strTypeName = CString((LPCTSTR) IDS_MENU_SYSTEM_DISKINFO);
	m_inTaskInfoDlg.SetInformationType(strTypeName);
	m_inTaskInfoDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inTaskInfoDlg);

	// イベントログ
	strTypeName = CString((LPCTSTR) IDS_MENU_SYSTEM_EVENTLOG);	
	m_inSystemEventLogDlg.SetInformationType(strTypeName);
	m_inSystemEventLogDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inSystemEventLogDlg);

	// ユーザ情報
	strTypeName = CString((LPCTSTR) IDS_MENU_SYSTEM_USERINFO);	
	m_inUserInfoDlg.Create(CUserInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inUserInfoDlg);

	// ユーザ一覧
	strTypeName = CString((LPCTSTR) IDS_MENU_SYSTEM_USERLIST);	
	m_inSystemUserListDlg.SetInformationType(strTypeName);
	m_inSystemUserListDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inSystemUserListDlg);

	// サービス操作
	strTypeName = CString((LPCTSTR) IDS_MENU_SYSTEM_SERVICE);	
	m_inSysServiceInfoDlg.Create(CSysServiceInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inSysServiceInfoDlg);

	// 演算速度
	strTypeName = CString((LPCTSTR) IDS_MENU_SYSTEM_PROCESSOR_SPEED);	
	m_inProcessSpeedDlg.SetInformationType(strTypeName);
	m_inProcessSpeedDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inProcessSpeedDlg);

	// エラーコード
	strTypeName = CString((LPCTSTR) IDS_MENU_SYSTEM_ERRORCODE);	
	m_inErrorDetailDlg.SetInformationType(strTypeName);
	m_inErrorDetailDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inErrorDetailDlg);


	/// -----
	/// 時間
	/// -----
	// 時間の変更
	strTypeName = CString((LPCTSTR) IDS_MENU_TIME_TIME_CHANGE);	
	m_inTimeChangeTimeDlg.Create(CTimeChangeTimeDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inTimeChangeTimeDlg);

	// ストップウォッチ
	strTypeName = CString((LPCTSTR) IDS_MENU_TIME_STOP_WATCH);	
	m_inTimeStopWatchDlg.Create(CTimeStopWatchDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inTimeStopWatchDlg);


	/// -----
	/// ファイル
	/// -----
	// ファイルロック
	strTypeName = CString((LPCTSTR) IDS_MENU_FILE_FILE_LOCK);	
	m_inFileLockDlg.Create(CFileLockDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inFileLockDlg);

	// タイムスタンプ
	strTypeName = CString((LPCTSTR) IDS_MENU_FILE_FILE_TIMESTAMP);	
	m_inFileTimeStampDlg.Create(CFileTimeStampDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inFileTimeStampDlg);

	// 属性変更
	strTypeName = CString((LPCTSTR) IDS_MENU_FILE_FILE_ATTR_CHANGE);	
	m_inFileAttributeDlg.Create(CFileAttributeDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inFileAttributeDlg);

	// ファイル監視
	strTypeName = CString((LPCTSTR) IDS_MENU_FILE_FILE_TAIL);	
	m_inFileTailDlg.SetInformationType(strTypeName);
	m_inFileTailDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inFileTailDlg);

	// 一括ファイル置換
	strTypeName = CString((LPCTSTR) IDS_MENU_FILE_FILE_REPLACE);	
	m_inFileReplaceDlg.Create(CFileReplaceDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inFileReplaceDlg);

	// 共有ファイル操作情報
	strTypeName = CString((LPCTSTR) IDS_MENU_FILE_FILE_COMMONINFO);	
	m_inFileCommonOpenInfoDlg.SetInformationType(strTypeName);
	m_inFileCommonOpenInfoDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inFileCommonOpenInfoDlg);

	// フォルダオープン
	strTypeName = CString((LPCTSTR) IDS_MENU_FILE_FOLDER_OPEN);	
	m_inFileFolderOpenDlg.Create(CFileFolderOpenDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inFileFolderOpenDlg);


	/// -----
	/// データ作成
	/// -----
	// 指定サイズ
	strTypeName = CString((LPCTSTR) IDS_MENU_DUMMY_FILE_FILE_SIZE);	
	m_inDummyFileSizeDlg.Create(CDummyFileSizeDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inDummyFileSizeDlg);


	/// -----
	/// メモリー
	/// -----
	// メモリーリーク
	strTypeName = CString((LPCTSTR) IDS_MENU_MEMORY_MEM_LEAK);	
	m_inMemLeakDlg.Create(CMemLeakMemoryDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inMemLeakDlg);

	// メモリーリーク　GDI
	strTypeName = CString((LPCTSTR) IDS_MENU_MEMORY_GDI_MEM_LEAK);	
	m_inMemLeakGdiDlg.Create(CMemLeakGdiObjDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inMemLeakGdiDlg);

	// メモリーリーク　USER
	strTypeName = CString((LPCTSTR) IDS_MENU_MEMORY_USER_MEM_LEAK);	
	m_inMemLeakUserDlg.Create(CMemLeakUserObjDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inMemLeakUserDlg);

	// メモリーリーク　Thread
	strTypeName = CString((LPCTSTR) IDS_MENU_MEMORY_THREAD_MEM_LEAK);	
	m_inMemLeakThreadDlg.Create(CMemLeakUserObjDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inMemLeakThreadDlg);

	// ロック Mutex
	strTypeName = CString((LPCTSTR) IDS_MENU_MEMORY_LOCK_MUTEX);	
	m_inMemLockMutexDlg.Create(CIUCommonLockDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inMemLockMutexDlg);

	// ロック Map File
	strTypeName = CString((LPCTSTR) IDS_MENU_MEMORY_LOCK_MAPFILE);	
	m_inMemLockFileMappingDlg.Create(CMemLockFileMappingDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inMemLockFileMappingDlg);


	/// -----
	/// 暗号化
	/// -----
	// MD5
	strTypeName = CString((LPCTSTR) IDS_MENU_ENCRYPT_HASH_MD5);	
	m_inMd5CryptDlg.SetInformationType(strTypeName);
	m_inMd5CryptDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inMd5CryptDlg);

	// SHA-1
	strTypeName = CString((LPCTSTR) IDS_MENU_ENCRYPT_HASH_SHA1);	
	m_inSha1CryptDlg.SetInformationType(strTypeName);
	m_inSha1CryptDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inSha1CryptDlg);

	// SHA-256
	strTypeName = CString((LPCTSTR) IDS_MENU_ENCRYPT_HASH_SHA256);	
	m_inSha256CryptDlg.SetInformationType(strTypeName);
	m_inSha256CryptDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inSha256CryptDlg);


	/// -----
	/// ネットワーク
	/// -----
	// Ping
	strTypeName = CString((LPCTSTR) IDS_MENU_NETWORK_PING);	
	m_inNetworkCmdPingDlg.SetInformationType(strTypeName);
	m_inNetworkCmdPingDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inNetworkCmdPingDlg);

	// Nslookup
	strTypeName = CString((LPCTSTR) IDS_MENU_NETWORK_NSLOOKUP);	
	m_inNetworkCmdNslookupDlg.SetInformationType(strTypeName);
	m_inNetworkCmdNslookupDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inNetworkCmdNslookupDlg);

	// Trace route
	strTypeName = CString((LPCTSTR) IDS_MENU_NETWORK_TRACERT);	
	m_inNetworkCmdTraceDlg.SetInformationType(strTypeName);
	m_inNetworkCmdTraceDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inNetworkCmdTraceDlg);

	// Nbtstat
	strTypeName = CString((LPCTSTR) IDS_MENU_NETWORK_NBTSTAT);	
	m_inNetworkCmdNbtStatDlg.SetInformationType(strTypeName);
	m_inNetworkCmdNbtStatDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inNetworkCmdNbtStatDlg);

	// Netstat
	strTypeName = CString((LPCTSTR) IDS_MENU_NETWORK_NETSTAT);	
	m_inNetworkCmdNetStatDlg.SetInformationType(strTypeName);
	m_inNetworkCmdNetStatDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inNetworkCmdNetStatDlg);

	// Arp
	strTypeName = CString((LPCTSTR) IDS_MENU_NETWORK_ARP);	
	m_inNetworkCmdArpDlg.SetInformationType(strTypeName);
	m_inNetworkCmdArpDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inNetworkCmdArpDlg);

	// Route
	strTypeName = CString((LPCTSTR) IDS_MENU_NETWORK_ROUTE);	
	m_inNetworkCmdRouteDlg.SetInformationType(strTypeName);
	m_inNetworkCmdRouteDlg.Create(CIUCommonInfoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inNetworkCmdRouteDlg);

	/// -----
	/// イメージ
	/// -----
	// フォント
	strTypeName = CString((LPCTSTR) IDS_MENU_IMAGE_FONT);	
	m_inFontImage.Create(CIUCommonImageDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inFontImage);
	// 数式グラフ
	strTypeName = CString((LPCTSTR) IDS_MENU_IMAGE_MATH_GRAPH);	
	m_inMathGraphImage.Create(CIUCommonImageDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inMathGraphImage);
	// カーソル情報
	strTypeName = CString((LPCTSTR) IDS_MENU_IMAGE_CUR_INFO);	
	m_inCursorInfoImage.Create(CIUCommonImageDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inCursorInfoImage);


	/// -----
	/// 表示
	/// -----
	// メモ
	strTypeName = CString((LPCTSTR) IDS_MENU_DISPLAY_MEMO);	
	m_inMemoDlg.Create(CMemoDlg::IDD, pWhd);
	m_mapInnerDlg.SetAt(strTypeName, &m_inMemoDlg);

	return TRUE;
}

/*!
 @brief コンテンツの非表示
*/
BOOL CInspectUsefulToolsDlg::HideContents()
{
	POSITION nMapPosition = m_mapInnerDlg.GetStartPosition();
	while (nMapPosition != NULL) {
		CString strKey;
		void *pDialog;

		m_mapInnerDlg.GetNextAssoc(nMapPosition, strKey, pDialog);
		((CDialog *) pDialog)->ShowWindow(SW_HIDE);
	}

	return TRUE;
}

/*!
 @brief コンテンツの表示
*/
BOOL CInspectUsefulToolsDlg::ShowContents(CDialog *pDialog)
{
	CRect    rect;
	m_inContentsTabCtrl.GetClientRect(&rect);
	rect.OffsetRect(1, 3);
	rect.right -= 10;
	rect.bottom -= 9;

	//EnableThemeDialogTexture(pDialog->GetSafeHwnd(), ETDT_ENABLETAB);
	pDialog->MoveWindow(&rect, FALSE);
	pDialog->ShowWindow(SW_SHOW);

	return TRUE;
}

/*!
 @brief ツリーメニュー選択イベント
*/
void CInspectUsefulToolsDlg::OnTvnSelchangedMenuTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if(pNMTreeView->hdr.idFrom != IDC_MENU_TREE) return;

	switch(pNMTreeView->hdr.code){
		case TVN_SELCHANGED: //ツリーアイテム選択変更時
			{
				HTREEITEM hItem = pNMTreeView->itemNew.hItem;
				TCHAR pszText[256];
				TVITEM ti;
				ti.mask = TVIF_TEXT | TVIF_CHILDREN;
				ti.hItem = hItem;
				ti.pszText = pszText;
				ti.cchTextMax = 256;
				
				if(m_inMenuTreeCtrl.GetItem(&ti)){
					if (ti.cChildren == 0) {
						HideContents();

						//ここで、pszTextに選択アイテムのラベルが入っている。
						void *pDialog = NULL;
						if (m_mapInnerDlg.Lookup(ti.pszText, pDialog) == TRUE) {
							ShowContents((CDialog *) pDialog);
						}


						// タイトルの設定
						CString strAppName = CString((LPCTSTR)IDS_APP_NAME);
						strAppName.Format(_T("%s - [%s]"), strAppName, pszText);
						this->SetWindowText(strAppName);
					} else {
						// 機能メニューではない場合は、トップ画面を表示
						HideContents();

						void *pDialog = NULL;
						if (m_mapInnerDlg.Lookup(CString((LPCTSTR) IDS_MENU_TOP), pDialog) == TRUE) {
							ShowContents((CDialog *) pDialog);
						}

						// タイトルの設定
						CString strAppName = CString((LPCTSTR)IDS_APP_NAME);
						this->SetWindowText(strAppName);
					}
				}
			}
			break;
	}
	*pResult = 0;
}

/*!
 @brief D&Dすると呼び出されます。
*/
void CInspectUsefulToolsDlg::OnDropFiles(HDROP hDropInfo)
{
	POSITION nMapPosition = m_mapInnerDlg.GetStartPosition();
	while (nMapPosition != NULL) {
		CString strKey;
		void *pDialog;

		m_mapInnerDlg.GetNextAssoc(nMapPosition, strKey, pDialog);

		if (((CDialog *) pDialog)->IsWindowVisible() == false) {
			continue;
		}
		((CDialog *) pDialog)->SendMessage(WM_DROPFILES, (WPARAM) hDropInfo);
	}

	CDialog::OnDropFiles(hDropInfo);
}


/*!
 @brief タスクトレイにアイコンを追加
*/
BOOL CInspectUsefulToolsDlg::AddNotifyIcon()  
{  
    CWinApp* pApp = AfxGetApp();  
	HICON hIcon = (HICON)LoadImage(pApp->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME),  IMAGE_ICON, 16, 16, 0);
    m_cTaskTray.cbSize = sizeof(NOTIFYICONDATA);  
    m_cTaskTray.hWnd = GetSafeHwnd();  
    m_cTaskTray.uID = 1;  
    m_cTaskTray.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;  
    m_cTaskTray.hIcon= hIcon;  
    m_cTaskTray.uCallbackMessage = m_uWMNotifyIcon;  
	_tcscpy_s(m_cTaskTray.szTip, sizeof(m_cTaskTray.szTip) / sizeof(m_cTaskTray.szTip[0]), CString((LPCTSTR) IDS_APP_NAME));  
    return Shell_NotifyIcon(NIM_ADD, &m_cTaskTray);  
}  

/*!
 @brief タスクトレイにアイコンを削除
*/
BOOL CInspectUsefulToolsDlg::DeleteNotifyIcon()  
{  
    return Shell_NotifyIcon(NIM_DELETE, &m_cTaskTray);  
}


/** 
 * タスクトレイメッセージハンドラ 
 */  
LRESULT CInspectUsefulToolsDlg::OnNotifyTaskBarIcon(WPARAM,LPARAM lParam)  
{  
    CMenu cMenu;  
    POINT point;  
  
    switch (lParam)  
    {  
    case WM_RBUTTONUP:  
        // TrackPopupMenu か TrackPopupMenuEx を呼び出す前に、ウィンドウを  
        // フォアグラウンドウィンドウにしなければならない  
        // (see the MSDN Article ID: Q135788 )  
        SetForegroundWindow();  
        if( cMenu.LoadMenu(IDR_POPUPMENU))  
        {  
            CMenu* pSubMenu = cMenu.GetSubMenu(0);  
            if (pSubMenu != NULL)  
            {  
                pSubMenu->SetDefaultItem( 0, TRUE);  
                GetCursorPos( &point );  
                pSubMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON,  
                                        point.x, point.y, this);  
                pSubMenu->DestroyMenu();  
            }  
            cMenu.DestroyMenu();  
        }  
        // BUGFIX: 最後に次のクリックに反応できるようにメッセージをポスト  
        // (see the MSDN Article ID: Q135788 )  
        PostMessage( WM_NULL, 0, 0);  
        break;  
    case WM_LBUTTONDBLCLK:
		this->ShowWindow(SW_SHOW);
		this->PostMessage(WM_SYSCOMMAND, SC_RESTORE);
        break;  
    }  
    return NULL;  
}  

/*!
 @brief Win98 以降か IE4 デスクトップ がインストールされている環境であれば、 
  タスクバーを再構築する場合に TaskbarCreated メッセージが送信される 
 */  
LRESULT CInspectUsefulToolsDlg::OnTaskBarCreated(WPARAM, LPARAM)  
{
    AddNotifyIcon();        // タスクバーアイコンの追加  
  
    return NULL;  
}


/*!
 @brief タスクトレイメニューの閉じるメニュー選択イベント
 */ 
void CInspectUsefulToolsDlg::OnMenuClose()
{
	DeleteNotifyIcon();

	this->OnOK();
}
