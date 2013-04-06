/*! @file  IUCommonInfoDlg.cpp
    @brief 共通コマンド実行画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "IUCommonComandDlg.h"

#include <io.h>
#include <fcntl.h>

IMPLEMENT_DYNAMIC(CIUCommonComandDlg, CDialog)

#define TEMP_FILE_PREFIX (_T("IUC"))

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CIUCommonComandDlg::CIUCommonComandDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIUCommonComandDlg::IDD, pParent)
{

}

/*!
 @brief ダイアログ データの交換と有効性チェックのためにフレームワークが呼び出します。

 @param [in]    pDX     CDataExchange オブジェクトへのポインタ
*/
void CIUCommonComandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CIUCommonComandDlg, CDialog)
	ON_BN_CLICKED(IDD_IU_COM_COMMAND_EXEC_BUTTON, &CIUCommonComandDlg::OnBnClickedIuComCommandExecButton)
	ON_WM_SIZE()
END_MESSAGE_MAP()

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CIUCommonComandDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CIUCommonComandDlg::PreTranslateMessage(MSG* pMsg)
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
void CIUCommonComandDlg::OnBnClickedIuComCommandExecButton()
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
void CIUCommonComandDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	RECT stRect;

	int nButtonWidth = 0;
	CButton *pUpdateBtn = ((CButton *) this->GetDlgItem(IDD_IU_COM_COMMAND_EXEC_BUTTON));
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

	CStatic *pTitleLabel = ((CStatic *) this->GetDlgItem(IDD_IU_COM_COMMAND_TITLE_LABEL));
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

 @param [in]    strCommandLine 実行コマンド名
*/
void CIUCommonComandDlg::SetCommandLine(CString strCommandLine)
{
	m_strCommandLine = strCommandLine;
}

/*!
 @brief タイトルラベルに文字列を設定

 @param [in]    strTitle 画面タイトル
*/
void CIUCommonComandDlg::SetTitle(CString &strTitle)
{
	CString strTempTitle = strTitle;
	strTempTitle += _T(":");

	GetDlgItem(IDD_IU_COM_COMMAND_TITLE_LABEL)->SetWindowText(strTempTitle);
}

/*!
 @brief 情報取得\n
 指定コマンドを実行して情報を取得します。
 情報がない場合は、空文字を返します。

 @param [in]    bInit 初期化時の呼び出し

 @return 情報　
*/
CString CIUCommonComandDlg::GetInformation(BOOL)
{
	CString strResult;
	if (m_strCommandLine.IsEmpty() == true) {
		return _T("");
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
						  (LPTSTR)((LPCTSTR)m_strCommandLine),
						  &sec_attr,
						  &sec_attr,
						  TRUE,
						  CREATE_NO_WINDOW,		// 非表示
						  NULL,
						  NULL,
						  &si,
						  &pi)){
		// プロセス起動に失敗
		// ファイルのクローズ
		if (hFile != NULL) {
			::CloseHandle(hFile);
			hFile = NULL;
		}
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