/*! @file  FileReplaceDlg.h
    @brief ファイル置換画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "FileReplaceDlg.h"
#include "ReplaceFile.h"
#include "IUSettingData.h"

IMPLEMENT_DYNAMIC(CFileReplaceDlg, CDialog)

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CFileReplaceDlg::CFileReplaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileReplaceDlg::IDD, pParent)
	, m_strReplaceFilePath(_T(""))
	, m_strExtName(_T("CSV"))
	, m_strAfterReplace(_T(""))
	, m_strBeforeReplace(_T(""))
{

}

/*!
 @brief ダイアログ データの交換と有効性チェックのためにフレームワークが呼び出します。

 @param [in]    pDX     CDataExchange オブジェクトへのポインタ
*/
void CFileReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILE_REPLACE_PATH_EDIT, m_strReplaceFilePath);
	DDX_Text(pDX, IDC_FILE_REPLACE_EXT_LIST_EDIT, m_strExtName);
	DDX_Text(pDX, IDC_FILE_REPLACE_AFTER_WORD_EDIT, m_strAfterReplace);
	DDX_Text(pDX, IDC_FILE_REPLACE_BEFORE_WORD_EDIT, m_strBeforeReplace);
}


/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CFileReplaceDlg, CDialog)
	ON_BN_CLICKED(IDC_FILE_REPLACE_PATH_SELECT_BUTTON, &CFileReplaceDlg::OnBnClickedFileReplacePathSelectButton)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_FILE_REPLACE_EXEC_BUTTON, &CFileReplaceDlg::OnBnClickedFileReplaceExecButton)
	ON_EN_CHANGE(IDC_FILE_REPLACE_PATH_EDIT, &CFileReplaceDlg::OnEnChangeFileReplacePathEdit)
END_MESSAGE_MAP()


/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CFileReplaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// D&D設定
	::SHAutoComplete(::GetDlgItem(this->m_hWnd, IDC_FILE_REPLACE_PATH_EDIT), SHACF_FILESYSTEM);
	DragAcceptFiles();
	
	m_strReplaceFilePath = CIUSettingData::GetInstance().m_strReplacePath;
	m_strBeforeReplace = CIUSettingData::GetInstance().m_strReplaceSourceWord;
	m_strAfterReplace = CIUSettingData::GetInstance().m_strReplaceDestWord;
	UpdateData(FALSE);
	
	if (m_strReplaceFilePath.IsEmpty() == false) {
		((CDialog *) this->GetDlgItem(IDC_FILE_REPLACE_EXEC_BUTTON))->EnableWindow(TRUE);
	}

	return TRUE;
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CFileReplaceDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
        return 0;
        //  必ず０で戻ること!!
        //  0以外で戻ると各コントロールへ次の処理が回らなくなる
    }
    return CDialog::PreTranslateMessage(pMsg);
}

/*!
 @brief ファイル選択ボタンをクリックすると呼び出されます。
*/
void CFileReplaceDlg::OnBnClickedFileReplacePathSelectButton()
{
	// すべてのフォルダを対象
	BROWSEINFO sBrowseInfo;
	TCHAR szDisplayName[MAX_PATH];

	UpdateData(TRUE);
	_tcscpy_s(szDisplayName, m_strReplaceFilePath);

	sBrowseInfo.hwndOwner = this->m_hWnd;
	sBrowseInfo.pidlRoot = NULL;
	sBrowseInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	sBrowseInfo.lParam = (LPARAM)szDisplayName;
	sBrowseInfo.pszDisplayName = szDisplayName;
	sBrowseInfo.lpszTitle = NULL;
	sBrowseInfo.lpfn = &CFileReplaceDlg::BrowseCallbackProc;

	LPITEMIDLIST pidlBrowse =::SHBrowseForFolder(&sBrowseInfo);

	if (pidlBrowse != NULL) {
		TCHAR szSelectFolderPath[MAX_PATH];
		if (SHGetPathFromIDList(pidlBrowse, szSelectFolderPath) == TRUE) {
			m_strReplaceFilePath = szSelectFolderPath;

			((CDialog *) this->GetDlgItem(IDC_FILE_REPLACE_EXEC_BUTTON))->EnableWindow(TRUE);

			UpdateData(FALSE);
		}
		CoTaskMemFree(pidlBrowse);
	}
}

/*!
 @brief フォルダ選択コールバック関数

 @param [in]    hwnd     ウィンドウハンドル
 @param [in]    uMsg     メッセージ
 @param [in]    lParam   パラメータ
 @param [in]    lpData   パラメータ
*/
int CALLBACK CFileReplaceDlg::BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM /*lParam*/,LPARAM lpData)
{
    if(uMsg==BFFM_INITIALIZED && lpData != NULL){
		::SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
    }
    return 0;
}

/*!
 @brief D&Dすると呼び出されます。

 @param [in]    hDropInfo     D&D情報
*/
void CFileReplaceDlg::OnDropFiles(HDROP hDropInfo)
{
	UINT nDragFileCount = DragQueryFile(hDropInfo,  (UINT)-1, NULL, (UINT)0);

	// 複数のファイルがD&Dされたら中断
	if(nDragFileCount != 1) {
		return;
	}
	
	// ドラッグしたファイル名を取得し読み込みを行う
	TCHAR lpszFilePath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, lpszFilePath, MAX_PATH);

	m_strReplaceFilePath = lpszFilePath;
	((CDialog *) this->GetDlgItem(IDC_FILE_REPLACE_EXEC_BUTTON))->EnableWindow(TRUE);
	UpdateData(FALSE);

	DragFinish(hDropInfo);
}


/*!
 @brief 実行ボタンをクリックすると呼び出されます。
*/
void CFileReplaceDlg::OnBnClickedFileReplaceExecButton()
{
	UpdateData(TRUE);
	
	// 設定に反映
	CIUSettingData::GetInstance().m_strReplacePath = m_strReplaceFilePath;
	CIUSettingData::GetInstance().m_strReplaceSourceWord = m_strBeforeReplace;
	CIUSettingData::GetInstance().m_strReplaceDestWord = m_strAfterReplace;

	CReplaceFile inReplaceFile(m_strBeforeReplace, m_strAfterReplace);

	int nReplaceCount = inReplaceFile.Execute(m_strReplaceFilePath, m_strExtName);

	CString strMsg;
	CString strFormat((LPCTSTR) IDC_FILE_REPLACE_RESULT_FORMAT);
	strMsg.Format(strFormat, nReplaceCount);
	

	// 終了メッセージの出力
	AfxMessageBox(strMsg, MB_OK | MB_ICONINFORMATION);
}

/*!
 @brief ファイルパスを変更すると呼び出されます。
*/
void CFileReplaceDlg::OnEnChangeFileReplacePathEdit()
{
	UpdateData(TRUE);

	bool bEnable = false;
	if (_taccess_s(m_strReplaceFilePath, 0) == 0) {
		// ファイルが存在する
		bEnable = true;
	}

	((CDialog *) this->GetDlgItem(IDC_FILE_REPLACE_EXEC_BUTTON))->EnableWindow(bEnable);
}
