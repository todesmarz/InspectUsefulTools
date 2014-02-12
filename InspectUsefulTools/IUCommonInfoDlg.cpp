/*! @file  IUCommonInfoDlg.cpp
    @brief 共通情報表示画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "IUCommonInfoDlg.h"
#include "IUCommonComandDlg.h"
#include "IUCommonNumExecDlg.h"
#include "IUCommonHashCalcDlg.h"
#include "IUCommonParamComandDlg.h"
#include "IUCommonFileDlg.h"


#include "SysProcessSpeedDlg.h"
#include "SysErrorDescriptDlg.h"
#include "FileTailDlg.h"

IMPLEMENT_DYNAMIC(CIUCommonInfoDlg, CDialog)

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CIUCommonInfoDlg::CIUCommonInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIUCommonInfoDlg::IDD, pParent)
	, m_strResult(_T(""))
{
	m_pDialog = NULL;
	m_inFindDialog = NULL;
}

/*!
 @brief デストラクタ
*/
CIUCommonInfoDlg::~CIUCommonInfoDlg()
{
	if (m_pDialog != NULL) {
		delete m_pDialog;
		m_pDialog = NULL;
	}

	if (m_inFindDialog != NULL) {
		if (m_inFindDialog->IsTerminating() == FALSE) {
			m_inFindDialog->DestroyWindow();
		}
		m_inFindDialog = NULL;
	}
}

/*!
 @brief ダイアログ データの交換と有効性チェックのためにフレームワークが呼び出します。

 @param [in]    pDX     CDataExchange オブジェクトへのポインタ
*/
void CIUCommonInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// 結果表示エリア
	DDX_Text(pDX, IDD_IU_COM_INFO_RESULT_LABEL, m_strResult);
}

//検索ダイアログ用(追加コード)
UINT CIUCommonInfoDlg::WM_FINDREPLACE = ::RegisterWindowMessage( FINDMSGSTRING );

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CIUCommonInfoDlg, CDialog)
	ON_BN_CLICKED(IDD_IU_COM_COMMAND_EXEC_BUTTON, &CIUCommonInfoDlg::OnBnClickedIuComInfoUpdateButton)
	ON_WM_SIZE()
	//検索ダイアログ用のメッセージ(追加コード)
	ON_REGISTERED_MESSAGE(WM_FINDREPLACE, OnFindReplace)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CIUCommonInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	PerformInformationType();
	if (m_pDialog != NULL) {
		m_pDialog->ShowWindow(TRUE);
	}

	// 情報を表示
	//DisplayInformation(TRUE);
	
	m_strResult.LoadString(IDS_IU_INFORMATION_INIT_MESSAGE);
	UpdateData(FALSE);

	return TRUE;
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CIUCommonInfoDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
        return 0;
        //  必ず０で戻ること!!
        //  0以外で戻ると各コントロールへ次の処理が回らなくなる
    }
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == _T('F')) {
		if((GetAsyncKeyState(VK_CONTROL) & 0x8000) == 0x8000) {
			if (m_inFindDialog != NULL && m_inFindDialog->IsTerminating() == FALSE) {
				m_inFindDialog->ActivateTopParent();
			} else {
				if (m_inFindDialog != NULL && m_inFindDialog->IsTerminating() == TRUE) {
					delete m_inFindDialog;
					m_inFindDialog = NULL;
				}

				m_inFindDialog = new CFindReplaceDialog();
				m_inFindDialog->Create(TRUE, _T(""), _T(""), 1, this);

				m_inFindDialog->ShowWindow(SW_NORMAL);
			}
		}
	}
	}
    return CDialog::PreTranslateMessage(pMsg);
}

/*!
 @brief 更新ボタンをクリックすると呼び出されます。
*/
void CIUCommonInfoDlg::OnBnClickedIuComInfoUpdateButton()
{
	// 情報を表示
	DisplayInformation(FALSE);
}

/*!
 @brief リサイズイベント\n
 WM_SIZEで呼び出されます。

 @param [in]    nType  リサイズタイプ
 @param [in]    cx     幅
 @param [in]    cy     高さ
 */
void CIUCommonInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	RECT stRect;

	int nHeight = 0;
	// 表示ラベルをウィンドウサイズに合わせる
	if (m_pDialog != NULL && IsWindow(m_pDialog->GetSafeHwnd()) == TRUE) {
		m_pDialog->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		stRect.bottom = stRect.bottom - stRect.top;
		stRect.top = 0;
		stRect.left = 0;
		stRect.right = cx;
		m_pDialog->MoveWindow(&stRect);

		nHeight = stRect.bottom;
	}

	CEdit *pResultEdit = ((CEdit *) this->GetDlgItem(IDD_IU_COM_INFO_RESULT_LABEL));
	// 表示ラベルをウィンドウサイズに合わせる
	if (pResultEdit != NULL && IsWindow(pResultEdit->GetSafeHwnd()) == TRUE) {
		pResultEdit->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		stRect.top = nHeight;
		stRect.bottom = cy;
		stRect.right = cx;
		pResultEdit->MoveWindow(&stRect);
	}
}

/*!
 @brief リサイズイベント\n
 WM_SIZEで呼び出されます。

 @param [in]    bShow   表示状態
 @param [in]    nStatus ステータス
 */
void CIUCommonInfoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (m_inFindDialog != NULL && m_inFindDialog->IsTerminating() == FALSE) {
		m_inFindDialog->DestroyWindow();
		m_inFindDialog = NULL;

	}
}

/*!
 @brief 情報の種類を設定します\n

 @param [in]    szType  情報タイプの設定
 */
void CIUCommonInfoDlg::SetInformationType(LPCTSTR szType)
{
	m_strType = szType;
}

/*!
 @brief 情報の種類に基づき画面を作成します
 　追加した場合は、こちらを編集する
 */
void CIUCommonInfoDlg::PerformInformationType()
{
    // バージョン情報の取得 
    OSVERSIONINFO stOSver; 
    stOSver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO); 
    GetVersionEx(&stOSver); 
	BOOL bAdminUser = IsUserAnAdmin();

	CDialog *pDialog = NULL;
	/// -----
	/// システム情報
	/// -----
	if (CString((LPCTSTR) IDS_MENU_SYSTEM_SYSINFO) == m_strType) {
		// システム情報
		pDialog = new CIUCommonComandDlg();
		pDialog->Create(CIUCommonComandDlg::IDD, this);
		((CIUCommonComandDlg *) pDialog)->SetCommandLine(_T("systeminfo"));
		((CIUCommonComandDlg *) pDialog)->SetTitle(m_strType);
	} else if (CString((LPCTSTR) IDS_MENU_SYSTEM_ENVINFO) == m_strType) {
		// 環境変数情報
		pDialog = new CIUCommonComandDlg();
		pDialog->Create(CIUCommonComandDlg::IDD, this);
		((CIUCommonComandDlg *) pDialog)->SetCommandLine(_T("cmd /c SET"));
		((CIUCommonComandDlg *) pDialog)->SetTitle(m_strType);
	} else if (CString((LPCTSTR) IDS_MENU_SYSTEM_NETWORKINFO) == m_strType) {
		// ネットワーク情報
		pDialog = new CIUCommonComandDlg();
		pDialog->Create(CIUCommonComandDlg::IDD, this);
		((CIUCommonComandDlg *) pDialog)->SetCommandLine(_T("ipconfig"));
		((CIUCommonComandDlg *) pDialog)->SetTitle(m_strType);
	} else if (CString((LPCTSTR) IDS_MENU_SYSTEM_DISKINFO) == m_strType) {
		// ディスク情報
		pDialog = new CIUCommonComandDlg();
		pDialog->Create(CIUCommonComandDlg::IDD, this);
		if (stOSver.dwMajorVersion < 6 || bAdminUser == TRUE) {
			((CIUCommonComandDlg *) pDialog)->SetCommandLine(_T("cmd /c \"echo list volume | diskpart\""));
		}
		((CIUCommonComandDlg *) pDialog)->SetTitle(m_strType);
	} else if (CString((LPCTSTR) IDS_MENU_SYSTEM_TASKINFO) == m_strType) {
		// タスク情報
		pDialog = new CIUCommonComandDlg();
		pDialog->Create(CIUCommonComandDlg::IDD, this);
		if (stOSver.dwMajorVersion < 6 || bAdminUser == TRUE) {
			((CIUCommonComandDlg *) pDialog)->SetCommandLine(_T("tasklist /V /FO LIST"));
		}
		((CIUCommonComandDlg *) pDialog)->SetTitle(m_strType);
	} else if (CString((LPCTSTR) IDS_MENU_SYSTEM_EVENTLOG) == m_strType) {
		// イベントログ
		pDialog = new CIUCommonComandDlg();
		pDialog->Create(CIUCommonComandDlg::IDD, this);
		if (stOSver.dwMajorVersion < 6) {
			// XP以前
			((CIUCommonComandDlg *) pDialog)->SetCommandLine(_T("cmd /c \"cscript %systemroot%\\system32\\eventquery.vbs /R 50 /V\""));
		} else {
			// Vista以降
			((CIUCommonComandDlg *) pDialog)->SetCommandLine(_T("cmd /c \"wevtutil qe System /c:50 /f:text & wevtutil qe Application /c:50 /f:text\""));
		}
		((CIUCommonComandDlg *) pDialog)->SetTitle(m_strType);
	} else if (CString((LPCTSTR) IDS_MENU_SYSTEM_USERLIST) == m_strType) {
		// ユーザ一覧
		pDialog = new CIUCommonComandDlg();
		pDialog->Create(CIUCommonComandDlg::IDD, this);
		((CIUCommonComandDlg *) pDialog)->SetCommandLine(_T("net user"));
		((CIUCommonComandDlg *) pDialog)->SetTitle(m_strType);
	} else if (CString((LPCTSTR) IDS_MENU_SYSTEM_PROCESSOR_SPEED) == m_strType) {
		// 演算速度
		pDialog = new CSysProcessSpeedDlg();
		pDialog->Create(CIUCommonNumExecDlg::IDD, this);
		CString strTitle = CString((LPCTSTR) IDS_SYS_PROCESS_SPEED_TITLE);
		((CIUCommonNumExecDlg *) pDialog)->SetTitle(strTitle);
	} else if (CString((LPCTSTR) IDS_MENU_SYSTEM_ERRORCODE) == m_strType) {
		// エラーコード
		pDialog = new CSysErrorDescriptDlg();
		pDialog->Create(CIUCommonNumExecDlg::IDD, this);
		((CIUCommonNumExecDlg *) pDialog)->SetTitle(m_strType);


	/// -----
	/// ファイル
	/// -----
	} else if (CString((LPCTSTR) IDS_MENU_FILE_FILE_TAIL) == m_strType) {
		// ファイル監視
		pDialog = new CFileTailDlg();
		pDialog->Create(CIUCommonFileDlg::IDD, this);
		((CIUCommonFileDlg *) pDialog)->SetTitle(m_strType);
	} else if ( CString((LPCTSTR) IDS_MENU_FILE_FILE_COMMONINFO) == m_strType) {
		// 共有ファイル操作情報
		pDialog = new CIUCommonComandDlg();
		pDialog->Create(CIUCommonComandDlg::IDD, this);
		((CIUCommonComandDlg *) pDialog)->SetCommandLine(_T("openfiles /query /v"));
		((CIUCommonComandDlg *) pDialog)->SetTitle(m_strType);


	/// -----
	/// 暗号化
	/// -----
	} else if (CString((LPCTSTR) IDS_MENU_ENCRYPT_HASH_MD5) == m_strType) {
		// MD5
		pDialog = new CIUCommonHashCalcDlg();
		pDialog->Create(CIUCommonHashCalcDlg::IDD, this);
		((CIUCommonHashCalcDlg *) pDialog)->SetHashSetting(CALG_MD5, 16);
	} else if (CString((LPCTSTR) IDS_MENU_ENCRYPT_HASH_SHA1) == m_strType) {
		// SHA-1
		pDialog = new CIUCommonHashCalcDlg();
		pDialog->Create(CIUCommonHashCalcDlg::IDD, this);
		((CIUCommonHashCalcDlg *) pDialog)->SetHashSetting(CALG_SHA1, 20);
	} else if (CString((LPCTSTR) IDS_MENU_ENCRYPT_HASH_SHA256) == m_strType) {
		// SHA-256
		pDialog = new CIUCommonHashCalcDlg();
		pDialog->Create(CIUCommonHashCalcDlg::IDD, this);
		((CIUCommonHashCalcDlg *) pDialog)->SetHashSetting(CALG_SHA_256, 32);


	} else if (CString((LPCTSTR) IDS_MENU_NETWORK_PING) == m_strType) {
		pDialog = new CIUCommonParamComandDlg();
		pDialog->Create(CIUCommonParamComandDlg::IDD, this);
		((CIUCommonParamComandDlg *) pDialog)->SetCommandLine(_T("ping"));
		((CIUCommonParamComandDlg *) pDialog)->SetCmdParam(1, CString((LPCTSTR) IDS_NETWORK_PING_PARAM1), _T("localhost"));
		((CIUCommonParamComandDlg *) pDialog)->SetCmdParam(3, CString((LPCTSTR) IDS_NETWORK_PING_PARAM2), _T("3"), _T("-n %s"));
		((CIUCommonParamComandDlg *) pDialog)->SetCmdParam(4, CString((LPCTSTR) IDS_NETWORK_PING_PARAM3), _T("10"), _T("-w %s"));
		((CIUCommonParamComandDlg *) pDialog)->SetTitle(m_strType);
		((CIUCommonParamComandDlg *) pDialog)->PackWindowRect();
	} else if (CString((LPCTSTR) IDS_MENU_NETWORK_NSLOOKUP) == m_strType) {
		pDialog = new CIUCommonParamComandDlg();
		pDialog->Create(CIUCommonParamComandDlg::IDD, this);
		((CIUCommonParamComandDlg *) pDialog)->SetCommandLine(_T("nslookup"));
		((CIUCommonParamComandDlg *) pDialog)->SetCmdParam(1, CString((LPCTSTR) IDS_NETWORK_NSLOOKUP_PARAM1), _T("localhost"));
		((CIUCommonParamComandDlg *) pDialog)->SetTitle(m_strType);
		((CIUCommonParamComandDlg *) pDialog)->PackWindowRect();
	} else if (CString((LPCTSTR) IDS_MENU_NETWORK_TRACERT) == m_strType) {
		pDialog = new CIUCommonParamComandDlg();
		pDialog->Create(CIUCommonParamComandDlg::IDD, this);
		((CIUCommonParamComandDlg *) pDialog)->SetCommandLine(_T("tracert"));
		((CIUCommonParamComandDlg *) pDialog)->SetCmdParam(1, CString((LPCTSTR) IDS_NETWORK_TRACERT_PARAM1), _T("localhost"));
		((CIUCommonParamComandDlg *) pDialog)->SetCmdParam(3, CString((LPCTSTR) IDS_NETWORK_TRACERT_PARAM2), _T("10"), _T("-w %s"));
		((CIUCommonParamComandDlg *) pDialog)->SetTitle(m_strType);
		((CIUCommonParamComandDlg *) pDialog)->PackWindowRect();
	} else if (CString((LPCTSTR) IDS_MENU_NETWORK_NBTSTAT) == m_strType) {
		pDialog = new CIUCommonComandDlg();
		pDialog->Create(CIUCommonComandDlg::IDD, this);
		((CIUCommonComandDlg *) pDialog)->SetCommandLine(_T("nbtstat -n"));
		((CIUCommonComandDlg *) pDialog)->SetTitle(m_strType);
	} else if (CString((LPCTSTR) IDS_MENU_NETWORK_NETSTAT) == m_strType) {
		pDialog = new CIUCommonComandDlg();
		pDialog->Create(CIUCommonComandDlg::IDD, this);
		((CIUCommonComandDlg *) pDialog)->SetCommandLine(_T("cmd /c \"netstat -aon & netstat -es\""));
		((CIUCommonComandDlg *) pDialog)->SetTitle(m_strType);
	} else if (CString((LPCTSTR) IDS_MENU_NETWORK_ARP) == m_strType) {
		pDialog = new CIUCommonComandDlg();
		pDialog->Create(CIUCommonComandDlg::IDD, this);
		((CIUCommonComandDlg *) pDialog)->SetCommandLine(_T("arp -a"));
		((CIUCommonComandDlg *) pDialog)->SetTitle(m_strType);
	} else if (CString((LPCTSTR) IDS_MENU_NETWORK_ROUTE) == m_strType) {
		pDialog = new CIUCommonComandDlg();
		pDialog->Create(CIUCommonComandDlg::IDD, this);
		((CIUCommonComandDlg *) pDialog)->SetCommandLine(_T("route print"));
		((CIUCommonComandDlg *) pDialog)->SetTitle(m_strType);
	}

	if (pDialog != NULL) {
		if (m_pDialog != NULL) {
			delete m_pDialog;
			m_pDialog = NULL;
		}

		m_pDialog = pDialog;
	}

	return;
}

/*!
 @brief 情報を表示します

 @param [in]    bInit  初期化処理の呼び出し
 */
void CIUCommonInfoDlg::DisplayInformation(BOOL bInit)
{
	try {
		if (m_pDialog == NULL) {
			return;
		}
		CEdit *pEdit = ((CEdit *) GetDlgItem(IDD_IU_COM_INFO_RESULT_LABEL));
		int nLineNum = pEdit->GetFirstVisibleLine();
		int nSel = pEdit->GetSel();
		int nLineIndex =  pEdit->LineIndex() + 1;
		int nMaxLineNum = pEdit->GetLineCount();
		BOOL bProcessed = FALSE;

		CIUCommonComandDlg *pCommandDlg = dynamic_cast<CIUCommonComandDlg *>(m_pDialog);
		if (bProcessed == FALSE && pCommandDlg != NULL) {
			m_strResult = pCommandDlg->GetInformation(bInit);
			UpdateData(FALSE);

			bProcessed = TRUE;
		}

		CIUCommonNumExecDlg *pNumExecDlg = dynamic_cast<CIUCommonNumExecDlg *>(m_pDialog);
		if (bProcessed == FALSE && pNumExecDlg != NULL) {
			m_strResult = pNumExecDlg->GetInformation(bInit);
			UpdateData(FALSE);

			bProcessed = TRUE;
		}

		CIUCommonHashCalcDlg *pHashCalcDlg = dynamic_cast<CIUCommonHashCalcDlg *>(m_pDialog);
		if (bProcessed == FALSE && pHashCalcDlg != NULL) {
			m_strResult = pHashCalcDlg->GetInformation(bInit);
			UpdateData(FALSE);

			bProcessed = TRUE;
		}

		CIUCommonParamComandDlg *pParamCmdDlg = dynamic_cast<CIUCommonParamComandDlg *>(m_pDialog);
		if (bProcessed == FALSE && pParamCmdDlg != NULL) {
			m_strResult = pParamCmdDlg->GetInformation(bInit);
			UpdateData(FALSE);

			bProcessed = TRUE;
		}

		CIUCommonFileDlg *pFileDlg = dynamic_cast<CIUCommonFileDlg *>(m_pDialog);
		if (bProcessed == FALSE && pFileDlg != NULL) {
			m_strResult = pFileDlg->GetInformation(bInit);
			UpdateData(FALSE);

			bProcessed = TRUE;
		}

		CRect editRect;
		pEdit->GetWindowRect(&editRect);

		CPoint lastLinePos = pEdit->PosFromChar(nMaxLineNum);
		if (editRect.PtInRect(lastLinePos) == TRUE) {
			nLineNum += pEdit->GetLineCount() - nMaxLineNum;
			nLineNum = min(nLineNum, pEdit->GetLineCount());
		}

		pEdit->SetSel(nSel);
		pEdit->LineScroll(nLineNum, HIWORD(nSel) - nLineIndex);
	} catch (CException *pEx) {
		TCHAR szErrorMessage[1024];
		pEx->GetErrorMessage(szErrorMessage, sizeof(szErrorMessage) / sizeof(TCHAR));
		pEx->Delete();

		// メッセージの表示
		AfxMessageBox(szErrorMessage);
	}
}

LONG CIUCommonInfoDlg::OnFindReplace(WPARAM /*wParam*/, LPARAM lParam)
{
	CString wkss,str1,str2;

	CFindReplaceDialog* pFileDlg = CFindReplaceDialog::GetNotifier(lParam);
	UpdateData();
	BOOL bSearchDown = pFileDlg->SearchDown();
	BOOL bMatchCase = pFileDlg->MatchCase();

	if(pFileDlg->IsTerminating()){
		//-------------------
		//閉じた時の処理
		pFileDlg->DestroyWindow();
		m_inFindDialog = NULL;
	}
	else if(pFileDlg->FindNext() && bSearchDown == TRUE){
		//----------------------
		//「次を検索」処理
		CEdit *pEdit = ((CEdit *) GetDlgItem(IDD_IU_COM_INFO_RESULT_LABEL));

		int nCurStartPos, nCurEndPos;
		pEdit->GetSel(nCurStartPos, nCurEndPos);						//現在のカーソル位置取得

		CString strFindValue = pFileDlg->GetFindString();				//検索文字列の取得
		int nFindValueLength = strFindValue.GetLength();					
		CString strTarget = m_strResult.Mid(nCurStartPos + 1);
	
		if (bMatchCase == FALSE) {
			strFindValue.MakeLower();
			strTarget.MakeLower();
		}

		int nFindIndex = strTarget.Find(strFindValue);					//文字列検索
		if(nFindIndex != -1){
			nCurStartPos = nCurStartPos + 1 + nFindIndex;
			pEdit->SetSel(nCurStartPos, nCurStartPos + nFindValueLength);
		}
	} else if(pFileDlg->FindNext() && bSearchDown == FALSE){
		//----------------------
		//「次を検索」処理
		CEdit *pEdit = ((CEdit *) GetDlgItem(IDD_IU_COM_INFO_RESULT_LABEL));

		int nCurStartPos, nCurEndPos;
		pEdit->GetSel(nCurStartPos, nCurEndPos);						//現在のカーソル位置取得

		CString strFindValue = pFileDlg->GetFindString();				//検索文字列の取得
		int nFindValueLength = strFindValue.GetLength();					
		CString strTarget = m_strResult.Left(nCurStartPos + 1);

		if (bMatchCase == FALSE) {
			strFindValue.MakeLower();
			strTarget.MakeLower();
		}

		int nFindIndex = strTarget.Find(strFindValue);					//文字列検索
		if(nFindIndex != -1){
			nCurStartPos = nCurStartPos + 1 + nFindIndex;
			pEdit->SetSel(nCurStartPos, nCurStartPos + nFindValueLength);
		}
	}

	return 1L;
}
