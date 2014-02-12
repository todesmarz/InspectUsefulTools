// FileTailDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "FileTailDlg.h"
#include "IUSettingData.h"

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CFileTailDlg::CFileTailDlg(CWnd* pParent /*=NULL*/)
	: CIUCommonFileDlg(pParent)
{
	m_pFile = NULL;
	m_bExecuteMode = FALSE;
}

/*!
 @brief ダイアログ データの交換と有効性チェックのためにフレームワークが呼び出します。

 @param [in]    pDX     CDataExchange オブジェクトへのポインタ
*/
void CFileTailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDD_IU_COM_FILE_FILEPATH_EDIT, m_strTailFilePath);
}

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CFileTailDlg, CIUCommonFileDlg)
	ON_WM_TIMER()
END_MESSAGE_MAP()


/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CFileTailDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_strTailFilePath = CIUSettingData::GetInstance().m_strTailPath;
	UpdateData(FALSE);
	
	if (m_strTailFilePath.IsEmpty() == false) {
		((CDialog *) this->GetDlgItem(IDD_IU_COM_FILE_EXEC_BUTTON))->EnableWindow(TRUE);
	}
	

	return TRUE;
}


/*!
 @brief 処理の実行開始
*/
void CFileTailDlg::ExecuteOperation()
{
	m_bExecuteMode = !m_bExecuteMode;

	if (m_bExecuteMode == FALSE) {
		SetButtonText(CString((LPCTSTR) IDC_FILE_TAIL_RUN));
	} else {
		SetButtonText(CString((LPCTSTR) IDC_FILE_TAIL_STOP));
	}

	if (m_pFile != NULL) {
		m_pFile->Close();
		delete m_pFile;
		m_pFile = NULL;
	}

	if (m_bExecuteMode == FALSE) {
		KillTimer(1);

		return;
	}
	m_strFileData = _T("");
	m_nReadIndex = 0;

	UpdateData(TRUE);
	// 設定に反映
	CIUSettingData::GetInstance().m_strTailPath = m_strTailFilePath;

	m_pFile = new CFile();
	if (m_pFile->Open(m_strTailFilePath, CFile::modeRead | CFile::typeBinary |CFile::shareDenyNone) == false) {
		return;
	}

	SetTimer(1, 2000, NULL);

	UpdateData(FALSE);
}

/*!
 @brief タイマーイベント
*/
void CFileTailDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (this->IsWindowVisible() == false) {
		// 非表示時には、なにもしない
		return;
	}

	CFileStatus inFileStatus;
	if (m_pFile->GetStatus(inFileStatus) == false) {
		return;
	}

	if (inFileStatus.m_size == m_nReadIndex) {
		return;
	} 
	if (inFileStatus.m_size < m_nReadIndex) {
		m_nReadIndex = 0;
		m_strFileData.Empty();
	} 

	m_pFile->Seek(m_nReadIndex, 0);

	char szBuffer[1024 *4 + 1];
	UINT nBufferSize = sizeof(szBuffer) / sizeof(char);
	ZeroMemory(szBuffer, nBufferSize);
	while(m_pFile->Read(szBuffer, nBufferSize - 1) != 0) {
		m_strFileData += szBuffer;
		ZeroMemory(szBuffer, nBufferSize);
	}
	m_nReadIndex = m_pFile->GetPosition();

	// 親ダイアログにイベントを送信
	CWnd *pParent = this->GetParent();
	if (pParent != NULL) {
		pParent->SendMessage(WM_COMMAND, MAKEWPARAM(IDD_IU_COM_COMMAND_EXEC_BUTTON, BN_CLICKED));
	}

	CDialog::OnTimer(nIDEvent);
}

/*!
 @brief 情報の取得
*/
CString CFileTailDlg::GetInformation(BOOL bInit)
{
	if (bInit == TRUE) {
		return _T("");
	}

	return m_strFileData;
}
