// IUCommonParamComandDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "InspectUsefulTools.h"
/*! @file  IUCommonInfoDlg.cpp
    @brief 共通コマンド実行画面クラス
*/
#include "IUCommonParamComandDlg.h"

#include <io.h>
#include <fcntl.h>

#define TEMP_FILE_PREFIX (_T("IUC"))

IMPLEMENT_DYNAMIC(CIUCommonParamComandDlg, CDialog)

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CIUCommonParamComandDlg::CIUCommonParamComandDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIUCommonParamComandDlg::IDD, pParent)
{
	m_strParam1Format = _T("%s");
	m_strParam2Format = _T("%s");
	m_strParam3Format = _T("%s");
	m_strParam4Format = _T("%s");

	m_nParamLineNumber = 1;
}

/*!
 @brief ダイアログ データの交換と有効性チェックのためにフレームワークが呼び出します。

 @param [in]    pDX     CDataExchange オブジェクトへのポインタ
*/
void CIUCommonParamComandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CIUCommonParamComandDlg, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDD_IU_COM_PARAM_CMD_EXEC_BUTTON, &CIUCommonParamComandDlg::OnBnClickedIuComParamCmdExecButton)
END_MESSAGE_MAP()

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CIUCommonParamComandDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CIUCommonParamComandDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
        return 0;
        //  必ず０で戻ること!!
        //  0以外で戻ると各コントロールへ次の処理が回らなくなる
    }
    return CDialog::PreTranslateMessage(pMsg);
}

/*!
 @brief 更新ボタンをクリックすると呼び出されます。
*/
void CIUCommonParamComandDlg::OnBnClickedIuComParamCmdExecButton()
{
	// 親ダイアログにイベントを送信
	CWnd *pParent = this->GetParent();
	if (pParent != NULL) {
		pParent->SendMessage(WM_COMMAND, MAKEWPARAM(IDD_IU_COM_COMMAND_EXEC_BUTTON, BN_CLICKED));
	}
}

/*!
 @brief リサイズイベント\n
 WM_SIZEで呼び出されます。

 @param [in]    nType  リサイズタイプ
 @param [in]    cx     幅
 @param [in]    cy     高さ
 */
void CIUCommonParamComandDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	RECT stRect;

	int nButtonWidth = 0;
	CButton *pUpdateBtn = ((CButton *) this->GetDlgItem(IDD_IU_COM_PARAM_CMD_EXEC_BUTTON));
	// 更新ボタンをウィンドウサイズに合わせる
	if (pUpdateBtn != NULL && IsWindow(pUpdateBtn->GetSafeHwnd()) == TRUE) {
		pUpdateBtn->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		int nWidth = stRect.right - stRect.left;
		stRect.right = cx;
		stRect.left = cx - nWidth;
		pUpdateBtn->MoveWindow(&stRect);
		nButtonWidth = nWidth;
	}

	CStatic *pTitleLabel = ((CStatic *) this->GetDlgItem(IDD_IU_COM_PARAM_CMD_TITLELABEL));
	// タイトルエリアをウィンドウサイズに合わせる
	if (pTitleLabel != NULL && IsWindow(pTitleLabel->GetSafeHwnd()) == TRUE) {
		pTitleLabel->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		stRect.right = cx - nButtonWidth;
		pTitleLabel->MoveWindow(&stRect);
	}
}

/*!
 @brief 実行コマンドの設定

 @param [in]    strCommandLine     実行コマンド名
*/
void CIUCommonParamComandDlg::SetCommandLine(CString strCommandLine)
{
	m_strCommandLine = strCommandLine;
}

/*!
 @brief タイトルラベルに文字列を設定

 @param [in]    strTitle     タイトル
*/
void CIUCommonParamComandDlg::SetTitle(CString &strTitle)
{
	CString strTempTitle = strTitle;
	strTempTitle += _T(":");

	GetDlgItem(IDD_IU_COM_PARAM_CMD_TITLELABEL)->SetWindowText(strTempTitle);
}

/*!
 @brief パラメータ設定

 @param [in]    nParam         パラメータ番号(1以上)
 @param [in]    pszParamTitle  パラメータ名
 @param [in]    pszParamFormat コマンド設定時のフォーマット (-t %s)等 
 */
void CIUCommonParamComandDlg::SetCmdParam(int nParam, LPCTSTR pszParamTitle, LPCTSTR pszParamDefault, LPCTSTR pszParamFormat)
{
	CString strParamTitle;
	if (pszParamTitle != NULL) {
		strParamTitle += pszParamTitle;
		strParamTitle += _T(":");
	}

	switch (nParam) {
		case 1:
			if (pszParamTitle != NULL) {
				GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM1_LABEL)->SetWindowText(strParamTitle);
			}
			if (pszParamFormat != NULL) {
				m_strParam1Format = pszParamFormat;
			}
			GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM1_LABEL)->ShowWindow(SW_SHOW);
			GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM1_EDIT)->ShowWindow(SW_SHOW);
			if (pszParamDefault != NULL) {
				GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM1_EDIT)->SetWindowText(pszParamDefault);
			}
			m_nParamLineNumber = max(m_nParamLineNumber, 2);
			break;
		case 2:
			if (pszParamTitle != NULL) {
				GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM2_LABEL)->SetWindowText(strParamTitle);
			}
			if (pszParamFormat != NULL) {
				m_strParam2Format = pszParamFormat;
			}
			GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM2_LABEL)->ShowWindow(SW_SHOW);
			GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM2_EDIT)->ShowWindow(SW_SHOW);
			if (pszParamDefault != NULL) {
				GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM2_EDIT)->SetWindowText(pszParamDefault);
			}
			m_nParamLineNumber = max(m_nParamLineNumber, 2);
			break;
		case 3:
			if (pszParamTitle != NULL) {
				GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM3_LABEL)->SetWindowText(strParamTitle);
			}
			if (pszParamFormat != NULL) {
				m_strParam3Format = pszParamFormat;
			}
			GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM3_LABEL)->ShowWindow(SW_SHOW);
			GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM3_EDIT)->ShowWindow(SW_SHOW);
			if (pszParamDefault != NULL) {
				GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM3_EDIT)->SetWindowText(pszParamDefault);
			}
			m_nParamLineNumber = max(m_nParamLineNumber, 3);
			break;
		case 4:
			if (pszParamTitle != NULL) {
				GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM4_LABEL)->SetWindowText(strParamTitle);
			}
			if (pszParamFormat != NULL) {
				m_strParam4Format = pszParamFormat;
			}
			GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM4_LABEL)->ShowWindow(SW_SHOW);
			GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM4_EDIT)->ShowWindow(SW_SHOW);
			if (pszParamDefault != NULL) {
				GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM4_EDIT)->SetWindowText(pszParamDefault);
			}
			m_nParamLineNumber = max(m_nParamLineNumber, 3);
			break;
	}
}

/*!
 @brief コントロールサイズ調整
*/
void CIUCommonParamComandDlg::PackWindowRect()
{
	CRect inRect;
	this->GetWindowRect(&inRect);
	inRect.bottom = inRect.top + m_nParamLineNumber * 24 + 7;

	this->MoveWindow(&inRect);
}

/*!
 @brief 情報取得\n
 指定コマンドを実行して情報を取得します。

 @param [in]    bInit 初期化時の呼び出し

 @return 情報
*/
CString CIUCommonParamComandDlg::GetInformation(BOOL bInit)
{
	if (bInit == TRUE) {
		return _T("");
	}

	CString strResult;
	if (m_strCommandLine.IsEmpty() == true) {
		return _T("");
	}

	CString strCmdLine = m_strCommandLine;
	CString strData;
	GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM1_EDIT)->GetWindowText(strData);
	strData.Remove(_T(';'));
	strData.Remove(_T('&'));
	strData.Remove(_T('('));
	strData.Remove(_T(')'));
	strData.Remove(_T('\''));
	strData.Remove(_T('|'));
	if (strData.IsEmpty() == false) {
		strCmdLine += _T(" ");
		strCmdLine.AppendFormat(m_strParam1Format, strData);
	}

	strData.Empty();
	GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM2_EDIT)->GetWindowText(strData);
	strData.Remove(_T(';'));
	strData.Remove(_T('&'));
	strData.Remove(_T('('));
	strData.Remove(_T(')'));
	strData.Remove(_T('\''));
	strData.Remove(_T('|'));
	if (strData.IsEmpty() == false) {
		strCmdLine += _T(" ");
		strCmdLine.AppendFormat(m_strParam2Format, strData);
	}

	strData.Empty();
	GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM3_EDIT)->GetWindowText(strData);
	strData.Remove(_T(';'));
	strData.Remove(_T('&'));
	strData.Remove(_T('('));
	strData.Remove(_T(')'));
	strData.Remove(_T('\''));
	strData.Remove(_T('|'));
	if (strData.IsEmpty() == false) {
		strCmdLine += _T(" ");
		strCmdLine.AppendFormat(m_strParam3Format, strData);
	}

	strData.Empty();
	GetDlgItem(IDD_IU_COM_PARAM_CMD_PARAM4_EDIT)->GetWindowText(strData);
	strData.Remove(_T(';'));
	strData.Remove(_T('&'));
	strData.Remove(_T('('));
	strData.Remove(_T(')'));
	strData.Remove(_T('\''));
	strData.Remove(_T('|'));
	if (strData.IsEmpty() == false) {
		strCmdLine += _T(" ");
		strCmdLine.AppendFormat(m_strParam4Format, strData);
	}

	// 一時ファイルに結果を出力する
	// 一時ファイルパスの取得
	TCHAR szTmpFilePath[MAX_PATH];
	GetTempPath(MAX_PATH, szTmpFilePath);
	GetTempFileName(szTmpFilePath, TEMP_FILE_PREFIX, 1, szTmpFilePath);

	// 変数の初期化
	SECURITY_ATTRIBUTES sec_attr;
    ZeroMemory(&sec_attr,sizeof(sec_attr));
    sec_attr.bInheritHandle = TRUE;

	// 一時ファイルのオープン
	HANDLE hFile = CreateFile(szTmpFilePath, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ, &sec_attr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL) {
		// ファイルが開けない場合
		CFile::Remove(szTmpFilePath);
		return _T("");
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// CreateProcess()の引数設定 
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdOutput = hFile;
	si.hStdInput = stdin;

	// システム情報取得プロセスの起動
	if (!::CreateProcess(NULL,
						  (LPTSTR)((LPCTSTR)strCmdLine),
						  &sec_attr,
						  &sec_attr,
						  TRUE,
						  CREATE_NO_WINDOW,		// 非表示
						  NULL,
						  NULL,
						  &si,
						  &pi)){
		// プロセス起動に失敗
		::CloseHandle(hFile);
		CFile::Remove(szTmpFilePath);
		return _T("");
	}

	// プロセスが終了するか，終了イベントが来るまで待つ
	if (pi.hProcess) {
		DWORD dwEventNo = ::WaitForSingleObject(pi.hProcess, INFINITE);
		if (dwEventNo == WAIT_FAILED)	{
			// Waitに失敗?
			return _T("");
		}
	}

	// プロセスの終了
	if (pi.hProcess != NULL) {
		::CloseHandle(pi.hProcess);
		pi.hProcess = NULL;
	}
	if (pi.hThread != NULL) {
		::CloseHandle(pi.hThread);
		pi.hThread = NULL;
	}

	// 先頭に移動し、FILE*に変換
	SetFilePointer(hFile, NULL, NULL, FILE_BEGIN);
	int hFileHandle = _open_osfhandle((long) hFile, _O_TEXT);
	FILE *pFile = _tfdopen(hFileHandle, _T("r"));	

	try {
		// 出力結果の読み込み
		CStdioFile inFile(pFile);

		CString strReadData;
		while (inFile.ReadString(strReadData) == TRUE) {
			strResult += strReadData;
			strResult += _T("\r\n");
		}

		// ファイルのクローズと削除
		inFile.Close();
	} catch (...) {
		// 例外は無視
	}
	CFile::Remove(szTmpFilePath);

	return strResult;
}