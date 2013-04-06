/*! @file  IUCommonMemoryLeakDlg.cpp
    @brief 共通メモリーリーク画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "IUCommonMemoryLeakDlg.h"

IMPLEMENT_DYNAMIC(CIUCommonMemoryLeakDlg, CDialog)

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CIUCommonMemoryLeakDlg::CIUCommonMemoryLeakDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIUCommonMemoryLeakDlg::IDD, pParent)
{
	m_nCurrentAllocMemory = 0;
	m_bSetUnitCombo = FALSE;
}

/*!
 @brief ダイアログ データの交換と有効性チェックのためにフレームワークが呼び出します。

 @param [in]    pDX     CDataExchange オブジェクトへのポインタ
*/
void CIUCommonMemoryLeakDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CIUCommonMemoryLeakDlg, CDialog)
	ON_BN_CLICKED(IDD_IU_COM_MEM_LEAK_BUTTON, &CIUCommonMemoryLeakDlg::OnBnClickedIuComMemLeakButton)
	ON_BN_CLICKED(IDD_IU_COM_MEM_LEAK_RELEASE_BUTTON, &CIUCommonMemoryLeakDlg::OnBnClickedIuComMemLeakReleaseButton)
	ON_WM_SIZE()
END_MESSAGE_MAP()


/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CIUCommonMemoryLeakDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CEdit *pAllocEdit = ((CEdit *) this->GetDlgItem(IDD_IU_COM_MEM_LEAK_SIZE_EDIT));
	pAllocEdit->SetWindowText(_T("0"));

	CEdit *pAllocStatEdit = ((CEdit *) this->GetDlgItem(IDD_IU_COM_MEM_LEAK_STAT_EDIT));
	pAllocStatEdit->SetWindowText(_T("0"));

	return TRUE;
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CIUCommonMemoryLeakDlg::PreTranslateMessage(MSG* pMsg)
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
void CIUCommonMemoryLeakDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	RECT stRect;

	int nButtonWidth = 0;
	CButton *pAllocBtn = ((CButton *) this->GetDlgItem(IDD_IU_COM_MEM_LEAK_BUTTON));
	// 確保ボタンをウィンドウサイズに合わせる
	if (pAllocBtn != NULL && IsWindow(pAllocBtn->GetSafeHwnd()) == TRUE) {
		pAllocBtn->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		int nWidth = stRect.right - stRect.left;
		stRect.right = cx;
		stRect.left = cx - nWidth;
		pAllocBtn->MoveWindow(&stRect);
		nButtonWidth = nWidth;
	}
	nButtonWidth += 5;

	CComboBox *pUnitCombo = ((CComboBox *) this->GetDlgItem(IDD_IU_COM_MEM_LEAK_SIZE_UNIT_COMBO));
	// 単位をウィンドウサイズに合わせる
	if (pUnitCombo != NULL && IsWindow(pUnitCombo->GetSafeHwnd()) == TRUE
		&& m_bSetUnitCombo == TRUE) {
		pUnitCombo->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		int nWidth = stRect.right - stRect.left;
		stRect.right = cx - nButtonWidth;
		stRect.left = cx - nButtonWidth - nWidth;
		pUnitCombo->MoveWindow(&stRect);
		nButtonWidth += nWidth + 5;
	}

	CEdit *pSizeEdit = ((CEdit *) this->GetDlgItem(IDD_IU_COM_MEM_LEAK_SIZE_EDIT));
	// サイズエリアをウィンドウサイズに合わせる
	if (pSizeEdit != NULL && IsWindow(pSizeEdit->GetSafeHwnd()) == TRUE) {
		pSizeEdit->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		stRect.right = cx - nButtonWidth;
		pSizeEdit->MoveWindow(&stRect);
	}


	CButton *pReleaseBtn = ((CButton *) this->GetDlgItem(IDD_IU_COM_MEM_LEAK_RELEASE_BUTTON));
	// 解放ボタンをウィンドウサイズに合わせる
	if (pReleaseBtn != NULL && IsWindow(pReleaseBtn->GetSafeHwnd()) == TRUE) {
		pReleaseBtn->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		int nWidth = stRect.right - stRect.left;
		stRect.right = cx;
		stRect.left = cx - nWidth;
		pReleaseBtn->MoveWindow(&stRect);
		nButtonWidth = nWidth;
	}
	nButtonWidth += 5;

	CEdit *pAllocStatEdit = ((CEdit *) this->GetDlgItem(IDD_IU_COM_MEM_LEAK_STAT_EDIT));
	// 確保済みサイズエリアをウィンドウサイズに合わせる
	if (pAllocStatEdit != NULL && IsWindow(pAllocStatEdit->GetSafeHwnd()) == TRUE) {
		pAllocStatEdit->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		stRect.right = cx - nButtonWidth;
		pAllocStatEdit->MoveWindow(&stRect);
	}
}

/*!
 @brief 確保ボタンをクリックすると呼び出されます。
*/
void CIUCommonMemoryLeakDlg::OnBnClickedIuComMemLeakButton()
{
	// サイズの取得
	long long nAllockSize = 0;
	CString strAllocSize;
	CEdit *pAllocSizeEdit = ((CEdit *) this->GetDlgItem(IDD_IU_COM_MEM_LEAK_SIZE_EDIT));
	pAllocSizeEdit->GetWindowText(strAllocSize);
	TCHAR *pEnd = NULL;
	nAllockSize = _tcstol(strAllocSize, &pEnd, 10);
	if (*pEnd != NULL) {
		// 変換エラー
		return;
	}

	// 単位の取得
	int nSelectUnit = 0;
	CComboBox *pUnitCombo = ((CComboBox *) this->GetDlgItem(IDD_IU_COM_MEM_LEAK_SIZE_UNIT_COMBO));
	nSelectUnit = pUnitCombo->GetCurSel();

	// 領域の確保
	long long nAllocStat = AllocMemory(nAllockSize, nSelectUnit);
	m_nCurrentAllocMemory += nAllocStat;

	CEdit *pAllocStatEdit = ((CEdit *) this->GetDlgItem(IDD_IU_COM_MEM_LEAK_STAT_EDIT));
	CString strAllocStat;
	strAllocStat.Format(_T("%lld"), m_nCurrentAllocMemory);
	pAllocStatEdit->SetWindowText(strAllocStat);

	if (0 < m_nCurrentAllocMemory) {
		// 解放ボタン有効化
		this->GetDlgItem(IDD_IU_COM_MEM_LEAK_RELEASE_BUTTON)->EnableWindow(TRUE);
	}
}

/*!
 @brief 解放ボタンをクリックすると呼び出されます。
*/
void CIUCommonMemoryLeakDlg::OnBnClickedIuComMemLeakReleaseButton()
{
	// 領域の解放
	ReleaseMemory();

	m_nCurrentAllocMemory = 0;
	CEdit *pAllocStatEdit = ((CEdit *) this->GetDlgItem(IDD_IU_COM_MEM_LEAK_STAT_EDIT));
	CString strAllocStat;
	strAllocStat.Format(_T("%lld"), m_nCurrentAllocMemory);
	pAllocStatEdit->SetWindowText(strAllocStat);

	// 解放ボタン無効化
	this->GetDlgItem(IDD_IU_COM_MEM_LEAK_RELEASE_BUTTON)->EnableWindow(FALSE);
}

/*!
 @brief 確保タイトル設定

 @param [in]    pszTitle  タイトル
*/
void CIUCommonMemoryLeakDlg::SetAllocLabel(LPCTSTR pszTitle)
{
	if (pszTitle == NULL) {
		return;
	}

	CEdit *pAllocStatEdit = ((CEdit *) this->GetDlgItem(IDD_IU_COM_MEM_LEAK_SIZE_LABEL));
	CString strAllocStat(pszTitle);
	strAllocStat += _T(":");
	pAllocStatEdit->SetWindowText(strAllocStat);
}

/*!
 @brief 解放タイトル設定

 @param [in]    pszTitle  タイトル
*/
void CIUCommonMemoryLeakDlg::SetReleaseLabel(LPCTSTR pszTitle)
{
	if (pszTitle == NULL) {
		return;
	}

	CEdit *pAllocStatEdit = ((CEdit *) this->GetDlgItem(IDD_IU_COM_MEM_LEAK_STAT_LABEL));
	CString strAllocStat(pszTitle);
	strAllocStat += _T(":");
	pAllocStatEdit->SetWindowText(strAllocStat);
}

/*!
 @brief 単位の設定

 @param [in]    aryUnitString  単位一覧
*/
void CIUCommonMemoryLeakDlg::SetUnitLabel(CStringArray &aryUnitString)
{
	CComboBox *pUnitCombo = ((CComboBox *) this->GetDlgItem(IDD_IU_COM_MEM_LEAK_SIZE_UNIT_COMBO));

	BOOL bSetItem = FALSE;
	for (int nAryIndex = 0; nAryIndex < aryUnitString.GetCount(); nAryIndex++) {
		pUnitCombo->AddString(aryUnitString[nAryIndex]);
		bSetItem = TRUE;
	}

	if (bSetItem == TRUE) {
		// 項目が設定された場合　表示
		pUnitCombo->ShowWindow(SW_SHOW);
		pUnitCombo->SetCurSel(0);
		m_bSetUnitCombo = TRUE;
	}
}
