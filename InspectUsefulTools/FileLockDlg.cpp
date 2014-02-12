/*! @file  FileLockDlg.cpp
    @brief ファイルロック実行画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "FileLockDlg.h"
#include "IUSettingData.h"

#include <io.h>

IMPLEMENT_DYNAMIC(CFileLockDlg, CDialog)

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CFileLockDlg::CFileLockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileLockDlg::IDD, pParent)
	, m_strFileLockPath(_T(""))
	, m_bShareDenyWrite(false)
	, m_bShareDenyRead(false)
	, m_bShareDenyDelete(false)
{
}

/*!
 @brief ダイアログ データの交換と有効性チェックのためにフレームワークが呼び出します。

 @param [in]    pDX     CDataExchange オブジェクトへのポインタ
*/
void CFileLockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILE_LOCK_PATH_EDIT, m_strFileLockPath);
	DDX_Check(pDX, IDC_FILE_LOCK_DENY_WRITE_CHECK, m_bShareDenyWrite);
	DDX_Check(pDX, IDC_FILE_LOCK_DENY_READ_CHECK, m_bShareDenyRead);
	DDX_Check(pDX, IDC_FILE_LOCK_DENY_DELETE_CHECK, m_bShareDenyDelete);
}

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CFileLockDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_FILE_LOCK_PATH_SELECT_BUTTON, &CFileLockDlg::OnBnClickedFileLockPathSelectButton)
	ON_BN_CLICKED(IDC_FILE_LOCK_REGIST_BUTTON, &CFileLockDlg::OnBnClickedFileLockRegistButton)
	ON_BN_CLICKED(IDC_FILE_LOCK_CANCEL_BUTTON, &CFileLockDlg::OnBnClickedFileLockCancelButton)
	ON_EN_CHANGE(IDC_FILE_LOCK_PATH_EDIT, &CFileLockDlg::OnEnChangeFileLockPathEdit)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CFileLockDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// D&D設定
	::SHAutoComplete(::GetDlgItem(this->m_hWnd, IDC_FILE_LOCK_PATH_EDIT), SHACF_FILESYSTEM);
	DragAcceptFiles();

	m_strFileLockPath = CIUSettingData::GetInstance().m_strFileLockPath;
	UpdateData(FALSE);
	
	if (m_strFileLockPath.IsEmpty() == false) {
		((CDialog *) this->GetDlgItem(IDC_FILE_LOCK_REGIST_BUTTON))->EnableWindow(TRUE);
		((CDialog *) this->GetDlgItem(IDC_FILE_LOCK_CANCEL_BUTTON))->EnableWindow(FALSE);
	}
	return TRUE;
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CFileLockDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
        return 0;
        //  必ず０で戻ること!!
        //  0以外で戻ると各コントロールへ次の処理が回らなくなる
    }
    return CDialog::PreTranslateMessage(pMsg);
}

/*!
 @brief ファイルロックボタンをクリックすると呼び出されます。
*/
void CFileLockDlg::OnBnClickedFileLockRegistButton()
{
	UpdateData(TRUE);

	if (m_strFileLockPath.IsEmpty() == true) {
		return;
	}
	// 設定に反映
	CIUSettingData::GetInstance().m_strFileLockPath = m_strFileLockPath;

	DWORD dwShareMode = 0;
	if (m_bShareDenyWrite == 0) {
		// 書き込み禁止無効
		dwShareMode |= FILE_SHARE_WRITE;
	}
	if (m_bShareDenyRead == 0) {
		// 読み込み禁止無効
		dwShareMode |= FILE_SHARE_READ;
	}
	if (m_bShareDenyDelete == 0) {
		// 削除禁止無効
		dwShareMode |= FILE_SHARE_DELETE;
	}

	HANDLE hf = CreateFile( m_strFileLockPath, GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE,
                        dwShareMode,
                        NULL, OPEN_ALWAYS, 0, NULL);

	((CDialog *) this->GetDlgItem(IDC_FILE_LOCK_REGIST_BUTTON))->EnableWindow(FALSE);
	((CDialog *) this->GetDlgItem(IDC_FILE_LOCK_CANCEL_BUTTON))->EnableWindow(TRUE);


	HANDLE hTemp;
	if( hf != INVALID_HANDLE_VALUE
		&& m_mapFileHandle.Lookup(m_strFileLockPath, hTemp) == FALSE){
		m_mapFileHandle.SetAt(m_strFileLockPath, hf);
	}
}

/*!
 @brief ファイルロック解除ボタンをクリックすると呼び出されます。
*/
void CFileLockDlg::OnBnClickedFileLockCancelButton()
{
	UpdateData(TRUE);

	if (m_strFileLockPath.IsEmpty() == true) {
		// 対象が存在しない
		return;
	}
	// 設定に反映
	CIUSettingData::GetInstance().m_strFileLockPath = m_strFileLockPath;

	HANDLE hf;
	if (m_mapFileHandle.Lookup(m_strFileLockPath, hf) == TRUE) {
		CloseHandle(hf);
		m_mapFileHandle.RemoveKey(m_strFileLockPath);
	}

	((CDialog *) this->GetDlgItem(IDC_FILE_LOCK_REGIST_BUTTON))->EnableWindow(TRUE);
	((CDialog *) this->GetDlgItem(IDC_FILE_LOCK_CANCEL_BUTTON))->EnableWindow(FALSE);
}


/*!
 @brief ファイル選択ボタンをクリックすると呼び出されます。
*/
void CFileLockDlg::OnBnClickedFileLockPathSelectButton()
{
	// すべてのファイルを対象
	CFileDialog fileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY , CString((LPCTSTR) IDS_IU_FILETYPE_DESCRIPT));

	UpdateData(TRUE);
	CString strFilePath = m_strFileLockPath;
	if (strFilePath.IsEmpty() == false) {
		PathRemoveFileSpec((LPTSTR)(LPCTSTR)strFilePath);
		fileDlg.m_ofn.lpstrInitialDir = strFilePath;
	}
	// 設定に反映
	CIUSettingData::GetInstance().m_strFileLockPath = m_strFileLockPath;

	if (fileDlg.DoModal() == IDOK) {
		m_strFileLockPath = fileDlg.GetPathName();

		HANDLE hf;
		BOOL bFound = m_mapFileHandle.Lookup(m_strFileLockPath, hf);

		((CDialog *) this->GetDlgItem(IDC_FILE_LOCK_REGIST_BUTTON))->EnableWindow(!bFound);
		((CDialog *) this->GetDlgItem(IDC_FILE_LOCK_CANCEL_BUTTON))->EnableWindow(bFound);

		UpdateData(FALSE);
	}
}

/*!
 @brief ファイル選択ボタンをクリックすると呼び出されます。
*/
void CFileLockDlg::OnEnChangeFileLockPathEdit()
{
	UpdateData(TRUE);

	if (_taccess_s(m_strFileLockPath, 0) == 0) {
		// ファイルが存在する
		((CDialog *) this->GetDlgItem(IDC_FILE_LOCK_REGIST_BUTTON))->EnableWindow(FALSE);
		((CDialog *) this->GetDlgItem(IDC_FILE_LOCK_CANCEL_BUTTON))->EnableWindow(FALSE);
		return;
	}

	HANDLE hf;
	BOOL bFound = m_mapFileHandle.Lookup(m_strFileLockPath, hf);

	((CDialog *) this->GetDlgItem(IDC_FILE_LOCK_REGIST_BUTTON))->EnableWindow(!bFound);
	((CDialog *) this->GetDlgItem(IDC_FILE_LOCK_CANCEL_BUTTON))->EnableWindow(bFound);
}

/*!
 @brief D&Dすると呼び出されます。

 @param [in]    hDropInfo     D&D情報
*/
void CFileLockDlg::OnDropFiles(HDROP hDropInfo)
{
	UINT nDragFileCount = DragQueryFile(hDropInfo,  (UINT)-1, NULL, (UINT)0);

	// 複数のファイルがD&Dされたら中断
	if(nDragFileCount != 1) {
		return;
	}
	
	// ドラッグしたファイル名を取得し読み込みを行う
	TCHAR lpszFilePath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, lpszFilePath, MAX_PATH);

	m_strFileLockPath = lpszFilePath;

	HANDLE hf;
	BOOL bFound = m_mapFileHandle.Lookup(m_strFileLockPath, hf);

	((CDialog *) this->GetDlgItem(IDC_FILE_LOCK_REGIST_BUTTON))->EnableWindow(!bFound);
	((CDialog *) this->GetDlgItem(IDC_FILE_LOCK_CANCEL_BUTTON))->EnableWindow(bFound);
	UpdateData(FALSE);

	DragFinish(hDropInfo);
}
