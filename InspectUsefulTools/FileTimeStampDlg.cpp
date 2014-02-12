/*! @file  FileTimeStampDlg.cpp
    @brief ファイルタイムスタンプ変更画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "FileTimeStampDlg.h"
#include "IUSettingData.h"

#include <io.h>

IMPLEMENT_DYNAMIC(CFileTimeStampDlg, CDialog)

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CFileTimeStampDlg::CFileTimeStampDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileTimeStampDlg::IDD, pParent)
	, m_strFileTimeStampPath(_T(""))
{

}

/*!
 @brief ダイアログ データの交換と有効性チェックのためにフレームワークが呼び出します。

 @param [in]    pDX     CDataExchange オブジェクトへのポインタ
*/
void CFileTimeStampDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILE_TIMESTAMP_PATH_EDIT, m_strFileTimeStampPath);
}

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CFileTimeStampDlg, CDialog)
	ON_BN_CLICKED(IDC_FILE_TIMESTAMP_PATH_SELECT_BUTTON, &CFileTimeStampDlg::OnBnClickedFileTimestampPathSelectButton)
	ON_BN_CLICKED(IDC_FILE_TIMESTAMP_SET_BUTTON, &CFileTimeStampDlg::OnBnClickedFileTimestampSetButton)
	ON_BN_CLICKED(IDC_FILE_TIMESTAMP_RESET_BUTTON, &CFileTimeStampDlg::OnBnClickedFileTimestampResetButton)
	ON_EN_CHANGE(IDC_FILE_TIMESTAMP_PATH_EDIT, &CFileTimeStampDlg::OnEnChangeFileTimestampPathEdit)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CFileTimeStampDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// D&D設定
	::SHAutoComplete(::GetDlgItem(this->m_hWnd, IDC_FILE_TIMESTAMP_PATH_EDIT), SHACF_FILESYSTEM);
	DragAcceptFiles();
	
	m_strFileTimeStampPath = CIUSettingData::GetInstance().m_strFileTimeStampPath;
	UpdateData(FALSE);
	
	if (m_strFileTimeStampPath.IsEmpty() == false) {
		((CDialog *) this->GetDlgItem(IDC_FILE_TIMESTAMP_SET_BUTTON))->EnableWindow(TRUE);
		((CDialog *) this->GetDlgItem(IDC_FILE_TIMESTAMP_RESET_BUTTON))->EnableWindow(TRUE);
	}
	return TRUE;
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CFileTimeStampDlg::PreTranslateMessage(MSG* pMsg)
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
void CFileTimeStampDlg::OnBnClickedFileTimestampPathSelectButton()
{
	// すべてのファイルを対象
	CFileDialog fileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY , CString((LPCTSTR) IDS_IU_FILETYPE_DESCRIPT));

	UpdateData(TRUE);
	CString strFilePath = m_strFileTimeStampPath;
	if (strFilePath.IsEmpty() == false) {
		PathRemoveFileSpec((LPTSTR)(LPCTSTR)strFilePath);
		fileDlg.m_ofn.lpstrInitialDir = strFilePath;
	}

	if (fileDlg.DoModal() == IDOK) {
		m_strFileTimeStampPath = fileDlg.GetPathName();

		((CDialog *) this->GetDlgItem(IDC_FILE_TIMESTAMP_SET_BUTTON))->EnableWindow(TRUE);
		((CDialog *) this->GetDlgItem(IDC_FILE_TIMESTAMP_RESET_BUTTON))->EnableWindow(TRUE);
		UpdateData(FALSE);
	}
}

/*!
 @brief ファイルパスを変更すると呼び出されます。
*/
void CFileTimeStampDlg::OnEnChangeFileTimestampPathEdit()
{
	UpdateData(TRUE);

	bool bEnable = false;
	if (_taccess_s(m_strFileTimeStampPath, 0) == 0) {
		bEnable = true;
	}

	((CDialog *) this->GetDlgItem(IDC_FILE_TIMESTAMP_SET_BUTTON))->EnableWindow(bEnable);
	((CDialog *) this->GetDlgItem(IDC_FILE_TIMESTAMP_RESET_BUTTON))->EnableWindow(bEnable);
}

/*!
 @brief 設定ボタンをクリックすると呼び出されます。
*/
void CFileTimeStampDlg::OnBnClickedFileTimestampSetButton()
{
	UpdateData(TRUE);

	CFileStatus inFileStatus;

	SYSTEMTIME stSystemDate;
	SYSTEMTIME stSystemTime;
	CDateTimeCtrl *pDateCtrl;
	CDateTimeCtrl *pTimeCtrl;

	// 作成日時
	pDateCtrl = ((CDateTimeCtrl *) this->GetDlgItem(IDC_FILE_TIMESTAMP_CREATE_DATE_DATETIMEPICKER));
	pTimeCtrl = ((CDateTimeCtrl *) this->GetDlgItem(IDC_FILE_TIMESTAMP_CREATE_TIME_DATETIMEPICKER));
	pDateCtrl->GetTime(&stSystemDate);
	pTimeCtrl->GetTime(&stSystemTime);
	inFileStatus.m_ctime = CTime(stSystemDate.wYear, stSystemDate.wMonth, stSystemDate.wDay
		, stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond);

	// 変更日時
	pDateCtrl = ((CDateTimeCtrl *) this->GetDlgItem(IDC_FILE_TIMESTAMP_MODIFY_DATE_DATETIMEPICKER));
	pTimeCtrl = ((CDateTimeCtrl *) this->GetDlgItem(IDC_FILE_TIMESTAMP_MODIFY_TIME_DATETIMEPICKER));
	inFileStatus.m_mtime.GetAsSystemTime(stSystemTime);
	pDateCtrl->GetTime(&stSystemDate);
	pTimeCtrl->GetTime(&stSystemTime);
	inFileStatus.m_mtime = CTime(stSystemDate.wYear, stSystemDate.wMonth, stSystemDate.wDay
		, stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond);

	// アクセス日時
	pDateCtrl = ((CDateTimeCtrl *) this->GetDlgItem(IDC_FILE_TIMESTAMP_ACCESS_DATE_DATETIMEPICKER));
	pTimeCtrl = ((CDateTimeCtrl *) this->GetDlgItem(IDC_FILE_TIMESTAMP_ACCESS_TIME_DATETIMEPICKER));
	inFileStatus.m_atime.GetAsSystemTime(stSystemTime);
	pDateCtrl->GetTime(&stSystemDate);
	pTimeCtrl->GetTime(&stSystemTime);
	inFileStatus.m_atime = CTime(stSystemDate.wYear, stSystemDate.wMonth, stSystemDate.wDay
		, stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond);

	// 設定に反映
	CIUSettingData::GetInstance().m_strFileTimeStampPath = m_strFileTimeStampPath;

	// タイムスタンプの設定
	CFile::SetStatus(m_strFileTimeStampPath, inFileStatus);	
}

/*!
 @brief リセットボタンをクリックすると呼び出されます。
*/
void CFileTimeStampDlg::OnBnClickedFileTimestampResetButton()
{
	UpdateData(TRUE);

	// タイムスタンプの取得
	CFileStatus inFileStatus;
	if (CFile::GetStatus(m_strFileTimeStampPath, inFileStatus) == false) {
		return;
	}

	SYSTEMTIME stSystemTime;
	CDateTimeCtrl *pDateCtrl;
	CDateTimeCtrl *pTimeCtrl;

	// 作成日時
	pDateCtrl = ((CDateTimeCtrl *) this->GetDlgItem(IDC_FILE_TIMESTAMP_CREATE_DATE_DATETIMEPICKER));
	pTimeCtrl = ((CDateTimeCtrl *) this->GetDlgItem(IDC_FILE_TIMESTAMP_CREATE_TIME_DATETIMEPICKER));
	inFileStatus.m_ctime.GetAsSystemTime(stSystemTime);
	pDateCtrl->SetTime(stSystemTime);
	pTimeCtrl->SetTime(stSystemTime);

	// 変更日時
	pDateCtrl = ((CDateTimeCtrl *) this->GetDlgItem(IDC_FILE_TIMESTAMP_MODIFY_DATE_DATETIMEPICKER));
	pTimeCtrl = ((CDateTimeCtrl *) this->GetDlgItem(IDC_FILE_TIMESTAMP_MODIFY_TIME_DATETIMEPICKER));
	inFileStatus.m_mtime.GetAsSystemTime(stSystemTime);
	pDateCtrl->SetTime(stSystemTime);
	pTimeCtrl->SetTime(stSystemTime);

	// アクセス日時
	pDateCtrl = ((CDateTimeCtrl *) this->GetDlgItem(IDC_FILE_TIMESTAMP_ACCESS_DATE_DATETIMEPICKER));
	pTimeCtrl = ((CDateTimeCtrl *) this->GetDlgItem(IDC_FILE_TIMESTAMP_ACCESS_TIME_DATETIMEPICKER));
	inFileStatus.m_atime.GetAsSystemTime(stSystemTime);
	pDateCtrl->SetTime(stSystemTime);
	pTimeCtrl->SetTime(stSystemTime);
	
	// 設定に反映
	CIUSettingData::GetInstance().m_strFileTimeStampPath = m_strFileTimeStampPath;

	UpdateData(FALSE);
}

/*!
 @brief D&Dすると呼び出されます。

 @param [in]    hDropInfo     D&D情報
*/
void CFileTimeStampDlg::OnDropFiles(HDROP hDropInfo)
{
	UINT nDragFileCount = DragQueryFile(hDropInfo,  (UINT)-1, NULL, (UINT)0);

	// 複数のファイルがD&Dされたら中断
	if(nDragFileCount != 1) {
		return;
	}
	
	// ドラッグしたファイル名を取得し読み込みを行う
	TCHAR lpszFilePath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, lpszFilePath, MAX_PATH);

	m_strFileTimeStampPath = lpszFilePath;
	((CDialog *) this->GetDlgItem(IDC_FILE_TIMESTAMP_SET_BUTTON))->EnableWindow(TRUE);
	((CDialog *) this->GetDlgItem(IDC_FILE_TIMESTAMP_RESET_BUTTON))->EnableWindow(TRUE);
	UpdateData(FALSE);

	DragFinish(hDropInfo);
}
