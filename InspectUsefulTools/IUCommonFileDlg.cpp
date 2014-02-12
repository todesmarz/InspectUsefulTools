/*! @file  IUCommonFileDlg.cpp
    @brief 共通ファイル情報表示画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "IUCommonFileDlg.h"

IMPLEMENT_DYNAMIC(CIUCommonFileDlg, CDialog)

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CIUCommonFileDlg::CIUCommonFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIUCommonFileDlg::IDD, pParent)
{

}

/*!
 @brief ダイアログ データの交換と有効性チェックのためにフレームワークが呼び出します。

 @param [in]    pDX     CDataExchange オブジェクトへのポインタ
*/
void CIUCommonFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CIUCommonFileDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_DROPFILES()
	ON_EN_CHANGE(IDD_IU_COM_FILE_FILEPATH_EDIT, &CIUCommonFileDlg::OnEnChangeIuComFileFilepathEdit)
	ON_BN_CLICKED(IDD_IU_COM_FILE_SELECT_FILE_BUTTON, &CIUCommonFileDlg::OnBnClickedIuComFileSelectFileButton)
	ON_BN_CLICKED(IDD_IU_COM_FILE_EXEC_BUTTON, &CIUCommonFileDlg::OnBnClickedIuComFileExecButton)
END_MESSAGE_MAP()

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CIUCommonFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	::SHAutoComplete(::GetDlgItem(this->m_hWnd, IDD_IU_COM_FILE_FILEPATH_EDIT), SHACF_FILESYSTEM);
	DragAcceptFiles();

	return TRUE;
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CIUCommonFileDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
        return 0;
        //  必ず０で戻ること!!
        //  0以外で戻ると各コントロールへ次の処理が回らなくなる
    }
    return CDialog::PreTranslateMessage(pMsg);
}

/*!
 @brief リサイズイベント\n
 WM_SIZEで呼び出されます。

 @param [in]    nType  リサイズタイプ
 @param [in]    cx     幅
 @param [in]    cy     高さ
 */
void CIUCommonFileDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	RECT stRect;

	int nButtonWidth = 0;
	CButton *pExecBtn = ((CButton *) this->GetDlgItem(IDD_IU_COM_FILE_EXEC_BUTTON));
	// 変換ボタンをウィンドウサイズに合わせる
	if (pExecBtn != NULL && IsWindow(pExecBtn->GetSafeHwnd()) == TRUE) {
		pExecBtn->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		int nWidth = stRect.right - stRect.left;
		stRect.right = cx;
		stRect.left = cx - nWidth;
		pExecBtn->MoveWindow(&stRect);
		nButtonWidth = nWidth;
	}
	nButtonWidth += 5;

	CButton *pFileSelectBtn = ((CButton *) this->GetDlgItem(IDD_IU_COM_FILE_SELECT_FILE_BUTTON));
	// ファイル選択ボタンをウィンドウサイズに合わせる
	if (pFileSelectBtn != NULL && IsWindow(pFileSelectBtn->GetSafeHwnd()) == TRUE) {
		pFileSelectBtn->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		int nWidth = stRect.right - stRect.left;
		stRect.right = cx - nButtonWidth;
		stRect.left = stRect.right - nWidth;
		pFileSelectBtn->MoveWindow(&stRect);
		nButtonWidth += nWidth;
	}
	nButtonWidth += 5;

	CEdit *pFilePathEdit = ((CEdit *) this->GetDlgItem(IDD_IU_COM_FILE_FILEPATH_EDIT));
	// タイトルエリアをウィンドウサイズに合わせる
	if (pFilePathEdit != NULL && IsWindow(pFilePathEdit->GetSafeHwnd()) == TRUE) {
		pFilePathEdit->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		stRect.right = cx - nButtonWidth;
		pFilePathEdit->MoveWindow(&stRect);
	}
}

/*!
 @brief D&Dイベント\n
 WM_SIZEで呼び出されます。

 @param [in]    hDropInfo  ドロップ情報
 */
void CIUCommonFileDlg::OnDropFiles(HDROP hDropInfo)
{
	UINT nDragFileCount = DragQueryFile(hDropInfo,  (UINT)-1, NULL, (UINT)0);

	// 複数のファイルがD&Dされたら中断
	if(nDragFileCount != 1) {
		return;
	}
	
	// ドラッグしたファイル名を取得し読み込みを行う
	TCHAR lpszFilePath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, lpszFilePath, MAX_PATH);

	GetDlgItem(IDD_IU_COM_FILE_FILEPATH_EDIT)->SetWindowText(lpszFilePath);
	((CDialog *) this->GetDlgItem(IDD_IU_COM_FILE_EXEC_BUTTON))->EnableWindow(TRUE);

	DragFinish(hDropInfo);
}

/*!
 @brief ファイルパスを変更すると呼び出されます。
*/
void CIUCommonFileDlg::OnEnChangeIuComFileFilepathEdit()
{
	CString strFilePath;
	this->GetDlgItem(IDD_IU_COM_FILE_FILEPATH_EDIT)->GetWindowText(strFilePath);

	bool bEnable = false;
	if (_taccess_s(strFilePath, 0) == 0) {
		// ファイルが存在する
		bEnable = true;
	}

	((CDialog *) this->GetDlgItem(IDD_IU_COM_FILE_EXEC_BUTTON))->EnableWindow(TRUE);
}

/*!
 @brief ファイル選択ボタンをクリックすると呼び出されます。
*/
void CIUCommonFileDlg::OnBnClickedIuComFileSelectFileButton()
{
	// すべてのファイルを対象
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST , CString((LPCTSTR) IDS_IU_FILETYPE_DESCRIPT));
	
	CString strFilePath;
	this->GetDlgItem(IDD_IU_COM_FILE_FILEPATH_EDIT)->GetWindowText(strFilePath);
	if (strFilePath.IsEmpty() == false) {
		PathRemoveFileSpec((LPTSTR)(LPCTSTR)strFilePath);
		fileDlg.m_ofn.lpstrInitialDir = strFilePath;
	}

	if (fileDlg.DoModal() == IDOK) {
		CString strFilePath = fileDlg.GetPathName();
		this->GetDlgItem(IDD_IU_COM_FILE_FILEPATH_EDIT)->SetWindowText(strFilePath);

		((CDialog *) this->GetDlgItem(IDD_IU_COM_FILE_EXEC_BUTTON))->EnableWindow(TRUE);
	}
}

/*!
 @brief 実行ボタンをクリックすると呼び出されます。
*/
void CIUCommonFileDlg::OnBnClickedIuComFileExecButton()
{
	ExecuteOperation();
}

void CIUCommonFileDlg::SetButtonText(LPCTSTR pszTitle)
{
	if (pszTitle == NULL) {
		return;
	}
		
	CString strTempTitle = pszTitle;

	GetDlgItem(IDD_IU_COM_FILE_EXEC_BUTTON)->SetWindowText(strTempTitle);
}

void CIUCommonFileDlg::SetTitle(LPCTSTR pszTitle)
{
	if (pszTitle == NULL) {
		return;
	}

	CString strTempTitle = pszTitle;
	strTempTitle += _T(":");

	GetDlgItem(IDD_IU_COM_FILE_TITLE_LABEL)->SetWindowText(strTempTitle);
}
