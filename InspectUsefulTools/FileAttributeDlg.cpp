/*! @file  FileAttributeDlg.cpp
    @brief ファイル属性変更画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "FileAttributeDlg.h"

// ビットフラグ判定関数
#define CHECK_BIT_FLAG(data, flag) (((data) & (flag)) == (flag))

IMPLEMENT_DYNAMIC(CFileAttributeDlg, CDialog)

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CFileAttributeDlg::CFileAttributeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileAttributeDlg::IDD, pParent)
	, m_strAttributeFilePath(_T(""))
{

}

/*!
 @brief ダイアログ データの交換と有効性チェックのためにフレームワークが呼び出します。

 @param [in]    pDX     CDataExchange オブジェクトへのポインタ
*/
void CFileAttributeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILE_ATTRIBUTE_PATH_EDIT, m_strAttributeFilePath);
}

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CFileAttributeDlg, CDialog)
	ON_BN_CLICKED(IDC_FILE_ATTRIBUTE_PATH_SELECT_BUTTON, &CFileAttributeDlg::OnBnClickedFileAttributePathSelectButton)
	ON_BN_CLICKED(IDC_FILE_ATTRIBUTE_OPEN_BUTTON, &CFileAttributeDlg::OnBnClickedFileAttributeOpenButton)
	ON_BN_CLICKED(IDC_FILE_ATTRIBUTE_SAVE_BUTTON, &CFileAttributeDlg::OnBnClickedFileAttributeSaveButton)
	ON_WM_DROPFILES()
	ON_EN_CHANGE(IDC_FILE_ATTRIBUTE_PATH_EDIT, &CFileAttributeDlg::OnEnChangeFileAttributePathEdit)
END_MESSAGE_MAP()

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CFileAttributeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// D&D設定
	::SHAutoComplete(::GetDlgItem(this->m_hWnd, IDC_FILE_ATTRIBUTE_PATH_EDIT), SHACF_FILESYSTEM);
	DragAcceptFiles();

	return TRUE;
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CFileAttributeDlg::PreTranslateMessage(MSG* pMsg)
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
void CFileAttributeDlg::OnBnClickedFileAttributePathSelectButton()
{
	// すべてのファイルを対象
	CFileDialog fileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY , CString((LPCTSTR) IDS_IU_FILETYPE_DESCRIPT));
	if (fileDlg.DoModal() == IDOK) {
		m_strAttributeFilePath = fileDlg.GetPathName();

		((CDialog *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_OPEN_BUTTON))->EnableWindow(TRUE);
		((CDialog *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_SAVE_BUTTON))->EnableWindow(TRUE);

		UpdateData(FALSE);
	}
}

/*!
 @brief D&Dすると呼び出されます。

 @param [in]    hDropInfo     D&D情報
*/
void CFileAttributeDlg::OnDropFiles(HDROP hDropInfo)
{
	UINT nDragFileCount = DragQueryFile(hDropInfo,  (UINT)-1, NULL, (UINT)0);

	// 複数のファイルがD&Dされたら中断
	if(nDragFileCount != 1) {
		return;
	}
	
	// ドラッグしたファイル名を取得し読み込みを行う
	TCHAR lpszFilePath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, lpszFilePath, MAX_PATH);

	m_strAttributeFilePath = lpszFilePath;
	((CDialog *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_OPEN_BUTTON))->EnableWindow(TRUE);
	((CDialog *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_SAVE_BUTTON))->EnableWindow(TRUE);
	UpdateData(FALSE);

	DragFinish(hDropInfo);
}

/*!
 @brief ファイルパスを変更すると呼び出されます。
*/
void CFileAttributeDlg::OnEnChangeFileAttributePathEdit()
{
	UpdateData(TRUE);

	bool bEnable = false;
	if (_taccess_s(m_strAttributeFilePath, 0) == 0) {
		// ファイルが存在する
		bEnable = true;
	}

	((CDialog *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_OPEN_BUTTON))->EnableWindow(bEnable);
	((CDialog *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_SAVE_BUTTON))->EnableWindow(bEnable);
}


/*!
 @brief 開くボタンをクリックすると呼び出されます。
*/
void CFileAttributeDlg::OnBnClickedFileAttributeOpenButton()
{
	UpdateData(TRUE);

	// ファイル属性の取得
	WIN32_FILE_ATTRIBUTE_DATA stFileInfo;
	if (GetFileAttributesEx(m_strAttributeFilePath, GetFileExMaxInfoLevel, &stFileInfo) == false) {
		if (GetFileAttributesEx(m_strAttributeFilePath, GetFileExInfoStandard, &stFileInfo) == false) {
			return;
		}
	}
	
	// 読み取り専用
	((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_READONLY_CHECK))->SetCheck(
		CHECK_BIT_FLAG(stFileInfo.dwFileAttributes, FILE_ATTRIBUTE_READONLY));

	// 隠しファイル
	((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_HIDDEN_CHECK))->SetCheck(
		CHECK_BIT_FLAG(stFileInfo.dwFileAttributes, FILE_ATTRIBUTE_HIDDEN));

	// システムファイル
	((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_SYSTEM_CHECK))->SetCheck(
		CHECK_BIT_FLAG(stFileInfo.dwFileAttributes, FILE_ATTRIBUTE_SYSTEM));

	// アーカイブファイル
	((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_ARCHIVE_CHECK))->SetCheck(
		CHECK_BIT_FLAG(stFileInfo.dwFileAttributes, FILE_ATTRIBUTE_ARCHIVE));

	// Index対象
	((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_INDEX_CHECK))->SetCheck(
		CHECK_BIT_FLAG(stFileInfo.dwFileAttributes, FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) == FALSE);

	// Offlineファイル
	((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_OFFLINE_CHECK))->SetCheck(
		CHECK_BIT_FLAG(stFileInfo.dwFileAttributes, FILE_ATTRIBUTE_OFFLINE));

	// 圧縮ファイル
	((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_COMPRESS_RADIO))->SetCheck(
		CHECK_BIT_FLAG(stFileInfo.dwFileAttributes, FILE_ATTRIBUTE_COMPRESSED));

	// 暗号化ファイル
	((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_ENCRYPET_RADIO))->SetCheck(
		CHECK_BIT_FLAG(stFileInfo.dwFileAttributes, FILE_ATTRIBUTE_ENCRYPTED));

	// リパースファイル
	((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_REPARSE_CHECK))->SetCheck(
		CHECK_BIT_FLAG(stFileInfo.dwFileAttributes, FILE_ATTRIBUTE_REPARSE_POINT));

	// スパースファイル
	((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_SPARSE_CHECK))->SetCheck(
		CHECK_BIT_FLAG(stFileInfo.dwFileAttributes, FILE_ATTRIBUTE_SPARSE_FILE));

	UpdateData(FALSE);

	// 処理完了ダイアログを表示
	AfxMessageBox(IDS_IU_OPERATION_COMPLETE);
}


/*!
 @brief 開くボタンをクリックすると呼び出されます。
*/
void CFileAttributeDlg::OnBnClickedFileAttributeSaveButton()
{
	UpdateData(TRUE);

	DWORD dwAttribute = 0;
	
	// 読み取り専用
	dwAttribute |= ((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_READONLY_CHECK))->GetCheck() ? FILE_ATTRIBUTE_READONLY : 0;
	// 隠しファイル
	dwAttribute |= ((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_HIDDEN_CHECK))->GetCheck() ? FILE_ATTRIBUTE_HIDDEN : 0;
	// システムファイル
	dwAttribute |= ((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_SYSTEM_CHECK))->GetCheck() ? FILE_ATTRIBUTE_SYSTEM : 0;
	// アーカイブファイル
	dwAttribute |= ((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_ARCHIVE_CHECK))->GetCheck() ? FILE_ATTRIBUTE_ARCHIVE : 0;
	// Index対象
	dwAttribute |= ((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_INDEX_CHECK))->GetCheck() ? 0 : FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
	// Offlineファイル
	dwAttribute |= ((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_OFFLINE_CHECK))->GetCheck() ? FILE_ATTRIBUTE_OFFLINE : 0;
	// 圧縮ファイル
	dwAttribute |= ((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_COMPRESS_RADIO))->GetCheck() ? FILE_ATTRIBUTE_COMPRESSED : 0;
	// 暗号化ファイル
	dwAttribute |= ((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_ENCRYPET_RADIO))->GetCheck() ? FILE_ATTRIBUTE_ENCRYPTED : 0;
	// リパースファイル
	dwAttribute |= ((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_REPARSE_CHECK))->GetCheck() ? FILE_ATTRIBUTE_REPARSE_POINT : 0;
	// スパースファイル
	dwAttribute |= ((CButton *) this->GetDlgItem(IDC_FILE_ATTRIBUTE_SPARSE_CHECK))->GetCheck() ? FILE_ATTRIBUTE_SPARSE_FILE : 0;

	// ファイルに属性を設定
	if (SetFileAttributes(m_strAttributeFilePath, dwAttribute) == false) {
		return;
	}
	UpdateData(FALSE);
}
