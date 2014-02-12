/*! @file  FileFolderOpenDlg.cpp
    @brief フォルダオープン画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "FileFolderOpenDlg.h"
#include "IUSettingData.h"

#include <locale.h>

IMPLEMENT_DYNAMIC(CFileFolderOpenDlg, CDialog)

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CFileFolderOpenDlg::CFileFolderOpenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileFolderOpenDlg::IDD, pParent)
{

}

/*!
 @brief ダイアログ データの交換と有効性チェックのためにフレームワークが呼び出します。

 @param [in]    pDX     CDataExchange オブジェクトへのポインタ
*/
void CFileFolderOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDD_FILE_FOLDER_OPEN_PATH_EDIT, m_strFolderPath);
	DDX_Control(pDX, IDD_FILE_FOLDER_OPEN_RECENTLY_FILE_LIST, m_ctrlFileListBox);
}

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CFileFolderOpenDlg, CDialog)
	ON_BN_CLICKED(IDD_FILE_FOLDER_OPEN_PATH_SELECT_BUTTON, &CFileFolderOpenDlg::OnBnClickedFilePathSelectButton)
	ON_WM_DROPFILES()
	ON_EN_CHANGE(IDD_FILE_FOLDER_OPEN_PATH_EDIT, &CFileFolderOpenDlg::OnEnChangeFilePathEdit)
	ON_BN_CLICKED(IDD_FILE_FOLDER_OPEN_BUTTON, &CFileFolderOpenDlg::OnBnClickedFileFolderOpenButton)
	ON_LBN_SELCHANGE(IDD_FILE_FOLDER_OPEN_RECENTLY_FILE_LIST, &CFileFolderOpenDlg::OnLbnSelchangeFileFolderOpenRecentlyFileList)
	ON_WM_SIZE()
END_MESSAGE_MAP()

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CFileFolderOpenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// D&D設定
	::SHAutoComplete(::GetDlgItem(this->m_hWnd, IDD_FILE_FOLDER_OPEN_PATH_EDIT), SHACF_FILESYSTEM);
	DragAcceptFiles();

	
	m_strFolderPath = CIUSettingData::GetInstance().m_strFolderOpenPath;

	CString strList = CIUSettingData::GetInstance().m_strFolderOpenPathList;
	int nIndex = 0;
	while (0 <= (nIndex = strList.Find(_T("::")))) {
		CString filePath =  strList.Left(nIndex);

		//環境変数を展開するのに必要なサイズを取得する
		DWORD bufferSize = ExpandEnvironmentStrings(m_strFolderPath, NULL, 0);
		//必要なサイズを確保
		PTSTR pBuffer = new TCHAR[bufferSize];
		//環境変数を展開
		bufferSize = ExpandEnvironmentStrings(m_strFolderPath, pBuffer, bufferSize);

		if (_taccess(pBuffer, 0) == 0) {
			m_ctrlFileListBox.InsertString(0, filePath);
		}
		strList = strList.Mid(nIndex + 2);
	}

	//環境変数を展開するのに必要なサイズを取得する
	DWORD bufferSize = ExpandEnvironmentStrings(m_strFolderPath, NULL, 0);
	//必要なサイズを確保
	PTSTR pBuffer = new TCHAR[bufferSize];
	//環境変数を展開
	bufferSize = ExpandEnvironmentStrings(m_strFolderPath, pBuffer, bufferSize);

	if (strList.IsEmpty() == false && _taccess(pBuffer, 0) == 0) {
		m_ctrlFileListBox.InsertString(0, strList);
	}
	UpdateData(FALSE);

	if (m_strFolderPath.IsEmpty() == false) {
			((CDialog *) this->GetDlgItem(IDD_FILE_FOLDER_OPEN_BUTTON))->EnableWindow(TRUE);
	}

	return TRUE;
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CFileFolderOpenDlg::PreTranslateMessage(MSG* pMsg)
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
void CFileFolderOpenDlg::OnBnClickedFilePathSelectButton()
{
	// すべてのフォルダを対象
	BROWSEINFO sBrowseInfo;
	TCHAR szDisplayName[MAX_PATH];

	UpdateData(TRUE);
	_tcscpy_s(szDisplayName, m_strFolderPath);

	sBrowseInfo.hwndOwner = this->m_hWnd;
	sBrowseInfo.pidlRoot = NULL;
	sBrowseInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	sBrowseInfo.lParam = (LPARAM)szDisplayName;
	sBrowseInfo.pszDisplayName = szDisplayName;
	sBrowseInfo.lpszTitle = NULL;
	sBrowseInfo.lpfn = &CFileFolderOpenDlg::BrowseCallbackProc;

	LPITEMIDLIST pidlBrowse =::SHBrowseForFolder(&sBrowseInfo);

	if (pidlBrowse != NULL) {
		TCHAR szSelectFolderPath[MAX_PATH];
		if (SHGetPathFromIDList(pidlBrowse, szSelectFolderPath) == TRUE) {
			m_strFolderPath = szSelectFolderPath;

			((CDialog *) this->GetDlgItem(IDD_FILE_FOLDER_OPEN_BUTTON))->EnableWindow(TRUE);

			UpdateData(FALSE);
		}
	}
}

/*!
 @brief フォルダ選択コールバック関数

 @param [in]    hwnd     ウィンドウハンドル
 @param [in]    uMsg     メッセージ
 @param [in]    lParam   パラメータ
 @param [in]    lpData   パラメータ
*/
int CALLBACK CFileFolderOpenDlg::BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM /*lParam*/,LPARAM lpData)
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
void CFileFolderOpenDlg::OnDropFiles(HDROP hDropInfo)
{
	UINT nDragFileCount = DragQueryFile(hDropInfo,  (UINT)-1, NULL, (UINT)0);

	// 複数のファイルがD&Dされたら中断
	if(nDragFileCount != 1) {
		return;
	}
	
	// ドラッグしたファイル名を取得し読み込みを行う
	TCHAR lpszFilePath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, lpszFilePath, MAX_PATH);

	m_strFolderPath = lpszFilePath;
	((CDialog *) this->GetDlgItem(IDD_FILE_FOLDER_OPEN_BUTTON))->EnableWindow(TRUE);
	UpdateData(FALSE);

	DragFinish(hDropInfo);
}

/*!
 @brief ファイルパスを変更すると呼び出されます。
*/
void CFileFolderOpenDlg::OnEnChangeFilePathEdit()
{
	UpdateData(TRUE);
	
	//環境変数を展開するのに必要なサイズを取得する
	DWORD bufferSize = ExpandEnvironmentStrings(m_strFolderPath, NULL, 0);
	//必要なサイズを確保
	PTSTR pBuffer = new TCHAR[bufferSize];
	//環境変数を展開
	bufferSize = ExpandEnvironmentStrings(m_strFolderPath, pBuffer, bufferSize);

	bool bEnable = false;
	if (_taccess_s(pBuffer, 0) == 0) {
		// ファイルが存在する
		bEnable = true;
	}

	((CDialog *) this->GetDlgItem(IDD_FILE_FOLDER_OPEN_BUTTON))->EnableWindow(bEnable);
}

/*!
 @brief フォルダを開く
*/
void CFileFolderOpenDlg::OnBnClickedFileFolderOpenButton()
{
	//環境変数を展開するのに必要なサイズを取得する
	DWORD bufferSize = ExpandEnvironmentStrings(m_strFolderPath, NULL, 0);
	//必要なサイズを確保
	PTSTR pBuffer = new TCHAR[bufferSize];
	//環境変数を展開
	bufferSize = ExpandEnvironmentStrings(m_strFolderPath, pBuffer, bufferSize);

	ShellExecute(NULL, _T("open"), pBuffer, NULL, NULL, SW_SHOWNORMAL);

	int nFindIndex = m_ctrlFileListBox.FindString(0, m_strFolderPath);
	if (0 <= nFindIndex) {
		// 履歴に存在する場合は削除し、後に追加する
		m_ctrlFileListBox.DeleteString(nFindIndex);
	}
	
	// 設定に反映
	CIUSettingData::GetInstance().m_strFolderOpenPath = m_strFolderPath;

	m_ctrlFileListBox.InsertString(0, m_strFolderPath);

	
	CString strList;
	int nItemCount = m_ctrlFileListBox.GetCount();
	for (int nAryIndex = nItemCount - 1;  max(nItemCount - 10, 0) <= nAryIndex; nAryIndex--) {
		CString strItem;
		m_ctrlFileListBox.GetText(nAryIndex, strItem);
		strList += strItem + _T("::");
	}
	CIUSettingData::GetInstance().m_strFolderOpenPathList = strList;
}

/*!
 @brief 選択変更く
*/
void CFileFolderOpenDlg::OnLbnSelchangeFileFolderOpenRecentlyFileList()
{
	CString strSelectItem;
	int nSelIndex = m_ctrlFileListBox.GetCurSel();
	if (nSelIndex < 0) {
		return;
	}

	m_ctrlFileListBox.GetText(nSelIndex, strSelectItem);

	m_strFolderPath = strSelectItem;
	UpdateData(FALSE);
	
	//環境変数を展開するのに必要なサイズを取得する
	DWORD bufferSize = ExpandEnvironmentStrings(m_strFolderPath, NULL, 0);
	//必要なサイズを確保
	PTSTR pBuffer = new TCHAR[bufferSize];
	//環境変数を展開
	bufferSize = ExpandEnvironmentStrings(m_strFolderPath, pBuffer, bufferSize);

	bool bEnable = false;
	if (_taccess_s(pBuffer, 0) == 0) {
		// ファイルが存在する
		bEnable = true;
	}

	((CDialog *) this->GetDlgItem(IDD_FILE_FOLDER_OPEN_BUTTON))->EnableWindow(bEnable);
}

/*!
 @brief リサイズイベント\n
 WM_SIZEで呼び出されます。

 @param [in]    nType  リサイズタイプ
 @param [in]    cx     幅
 @param [in]    cy     高さ
 */
void CFileFolderOpenDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	RECT stRect;

	int nWidth = 0;
	CButton *pSelectButton = ((CButton *) this->GetDlgItem(IDD_FILE_FOLDER_OPEN_PATH_SELECT_BUTTON));
	// 表示ラベルをウィンドウサイズに合わせる
	if (pSelectButton != NULL && IsWindow(pSelectButton->GetSafeHwnd()) == TRUE) {
		pSelectButton->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		nWidth = stRect.right - stRect.left;
		stRect.right = cx;
		stRect.left = stRect.right - nWidth;
		pSelectButton->MoveWindow(&stRect);
	}

	nWidth += 5;

	CEdit *pFilePathEdit = ((CEdit *) this->GetDlgItem(IDD_FILE_FOLDER_OPEN_PATH_EDIT));
	// 表示ラベルをウィンドウサイズに合わせる
	if (pFilePathEdit != NULL && IsWindow(pFilePathEdit->GetSafeHwnd()) == TRUE) {
		pFilePathEdit->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		stRect.right = cx- nWidth;
		pFilePathEdit->MoveWindow(&stRect);
	}

	CButton *pOpenButton = ((CButton *) this->GetDlgItem(IDD_FILE_FOLDER_OPEN_BUTTON));
	// 表示ラベルをウィンドウサイズに合わせる
	if (pOpenButton != NULL && IsWindow(pOpenButton->GetSafeHwnd()) == TRUE) {
		pOpenButton->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		nWidth = stRect.right - stRect.left;
		stRect.right = cx;
		stRect.left = stRect.right - nWidth;
		pOpenButton->MoveWindow(&stRect);
	}

	CListBox *pRecentlyList = ((CListBox *) this->GetDlgItem(IDD_FILE_FOLDER_OPEN_RECENTLY_FILE_LIST));
	// 表示ラベルをウィンドウサイズに合わせる
	if (pRecentlyList != NULL && IsWindow(pRecentlyList->GetSafeHwnd()) == TRUE) {
		pRecentlyList->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		stRect.bottom = cy;
		stRect.right = cx;
		pRecentlyList->MoveWindow(&stRect);
	}
}
