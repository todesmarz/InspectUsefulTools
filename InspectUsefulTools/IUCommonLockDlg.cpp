/*! @file  IUCommonLockDlg.cpp
    @brief 共通ロック画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "IUCommonLockDlg.h"

IMPLEMENT_DYNAMIC(CIUCommonLockDlg, CDialog)

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CIUCommonLockDlg::CIUCommonLockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIUCommonLockDlg::IDD, pParent)
{

}

/*!
 @brief デストラクタ
*/
CIUCommonLockDlg::~CIUCommonLockDlg()
{
	POSITION pos = this->m_mapLockInfo.GetStartPosition();
	///マップの中身のメモリを解放
	while( pos != NULL ) {
		CString strKey;
		void *pHandle;
		m_mapLockInfo.GetNextAssoc( pos, strKey, pHandle);

		// Mutexの所有権を解放する。
		ReleaseMutex(pHandle);
		CloseHandle(pHandle);
	}
}

/*!
 @brief ダイアログ データの交換と有効性チェックのためにフレームワークが呼び出します。

 @param [in]    pDX     CDataExchange オブジェクトへのポインタ
*/
void CIUCommonLockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CIUCommonLockDlg, CDialog)
	ON_BN_CLICKED(IDD_IU_COM_LOCK_LOCK_BUTTON, &CIUCommonLockDlg::OnBnClickedIuComLockLockButton)
	ON_BN_CLICKED(IDD_IU_COM_LOCK_UNLOCK_BUTTON, &CIUCommonLockDlg::OnBnClickedIuComLockUnlockButton)
	ON_EN_CHANGE(IDD_IU_COM_LOCK_NAME_EDIT, &CIUCommonLockDlg::OnEnChangeIuComLockNameEdit)
	ON_WM_SIZE()
END_MESSAGE_MAP()


/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CIUCommonLockDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CIUCommonLockDlg::PreTranslateMessage(MSG* pMsg)
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
void CIUCommonLockDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	RECT stRect;

	int nButtonWidth = 0;
	CButton *pUnlockBtn = ((CButton *) this->GetDlgItem(IDD_IU_COM_LOCK_UNLOCK_BUTTON));
	// 確保ボタンをウィンドウサイズに合わせる
	if (pUnlockBtn != NULL && IsWindow(pUnlockBtn->GetSafeHwnd()) == TRUE) {
		pUnlockBtn->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		int nWidth = stRect.right - stRect.left;
		stRect.right = cx;
		stRect.left = cx - nWidth;
		pUnlockBtn->MoveWindow(&stRect);
		nButtonWidth = nWidth;
	}
	nButtonWidth += 5;

	CButton *pLockBtn = ((CButton *) this->GetDlgItem(IDD_IU_COM_LOCK_LOCK_BUTTON));
	// 単位をウィンドウサイズに合わせる
	if (pLockBtn != NULL && IsWindow(pLockBtn->GetSafeHwnd()) == TRUE) {
		pLockBtn->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		int nWidth = stRect.right - stRect.left;
		stRect.right = cx - nButtonWidth;
		stRect.left = cx - nButtonWidth - nWidth;
		pLockBtn->MoveWindow(&stRect);
		nButtonWidth += nWidth + 5;
	}

	CEdit *pSizeEdit = ((CEdit *) this->GetDlgItem(IDD_IU_COM_LOCK_NAME_EDIT));
	// サイズエリアをウィンドウサイズに合わせる
	if (pSizeEdit != NULL && IsWindow(pSizeEdit->GetSafeHwnd()) == TRUE) {
		pSizeEdit->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		stRect.right = cx;
		pSizeEdit->MoveWindow(&stRect);
	}
}

/*!
 @brief ロックボタンをクリックすると呼び出されます。
*/
void CIUCommonLockDlg::OnBnClickedIuComLockLockButton()
{
	CString strLockName;
	CEdit *pLockNameEdit = ((CEdit *) this->GetDlgItem(IDD_IU_COM_LOCK_NAME_EDIT));
	pLockNameEdit->GetWindowText(strLockName);

	HANDLE hLockHandle;
	BOOL bRet = LockObject(strLockName, hLockHandle);
	if (bRet == TRUE) {
		m_mapLockInfo.SetAt(strLockName, hLockHandle);
		AfxMessageBox(_T("ロックしました"));
		ModifyButtonEnable();
	} else {
		AfxMessageBox(_T("すでにロック済みのためロックできませんでした"));
	}
}

/*!
 @brief 解除ボタンをクリックすると呼び出されます。
*/
void CIUCommonLockDlg::OnBnClickedIuComLockUnlockButton()
{
	CString strLockName;
	CEdit *pLockNameEdit = ((CEdit *) this->GetDlgItem(IDD_IU_COM_LOCK_NAME_EDIT));
	pLockNameEdit->GetWindowText(strLockName);

	void *pHandle;
	if (m_mapLockInfo.Lookup(strLockName, pHandle) == FALSE) {
		return;
	}


	BOOL bRet = UnlockObject(strLockName, (HANDLE) pHandle);
	if (bRet == TRUE) {
		// 登録情報を削除
		m_mapLockInfo.RemoveKey(strLockName);

		AfxMessageBox(_T("ロック解除しました"));
		ModifyButtonEnable();
	} else {
		AfxMessageBox(_T("ロックされていないため解除できませんでした"));
	}
}

/*!
 @brief ロック名登録Editの変更イベント
*/
void CIUCommonLockDlg::OnEnChangeIuComLockNameEdit()
{
	ModifyButtonEnable();
}

/*!
 @brief ボタンの状態変更
*/
void CIUCommonLockDlg::ModifyButtonEnable()
{
	CString strLockName;
	CEdit *pLockNameEdit = ((CEdit *) this->GetDlgItem(IDD_IU_COM_LOCK_NAME_EDIT));
	pLockNameEdit->GetWindowText(strLockName);

	// 登録済みか確認
	void *pHandle;
	BOOL bFound = m_mapLockInfo.Lookup(strLockName, pHandle);

	CButton *pUnlockBtn = ((CButton *) this->GetDlgItem(IDD_IU_COM_LOCK_UNLOCK_BUTTON));
	CButton *pLockBtn = ((CButton *) this->GetDlgItem(IDD_IU_COM_LOCK_LOCK_BUTTON));

	// ボタンの状態を変更
	pUnlockBtn->EnableWindow(bFound == TRUE);
	pLockBtn->EnableWindow(bFound == FALSE);	
}