/*! @file  IUCommonNumExecDlg.cpp
    @brief 共通指定回数実行画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "IUCommonNumExecDlg.h"

IMPLEMENT_DYNAMIC(CIUCommonNumExecDlg, CDialog)

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CIUCommonNumExecDlg::CIUCommonNumExecDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIUCommonNumExecDlg::IDD, pParent)
{

}

/*!
 @brief ダイアログ データの交換と有効性チェックのためにフレームワークが呼び出します。

 @param [in]    pDX     CDataExchange オブジェクトへのポインタ
*/
void CIUCommonNumExecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CIUCommonNumExecDlg, CDialog)
	ON_BN_CLICKED(IDD_IU_COM_NUM_EXEC_BUTTON, &CIUCommonNumExecDlg::OnBnClickedIuComNumExecButton)
	ON_WM_SIZE()
END_MESSAGE_MAP()

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CIUCommonNumExecDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CIUCommonNumExecDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
        return 0;
        //  必ず０で戻ること!!
        //  0以外で戻ると各コントロールへ次の処理が回らなくなる
    }
    return CDialog::PreTranslateMessage(pMsg);
}

/*!
 @brief 実行ボタンをクリックすると呼び出されます。
*/
void CIUCommonNumExecDlg::OnBnClickedIuComNumExecButton()
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
void CIUCommonNumExecDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	RECT stRect;

	int nButtonWidth = 0;
	CButton *pUpdateBtn = ((CButton *) this->GetDlgItem(IDD_IU_COM_NUM_EXEC_BUTTON));
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

	// マージン
	nButtonWidth += 5;

	CSpinButtonCtrl *pNumSpin = ((CSpinButtonCtrl *) this->GetDlgItem(IDD_IU_COM_NUM_EXEC_TIMES_SPIN));
	// UpDownコントロールをウィンドウサイズに合わせる
	if (pNumSpin != NULL && IsWindow(pNumSpin->GetSafeHwnd()) == TRUE) {
		pNumSpin->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		int nWidth = stRect.right - stRect.left;
		stRect.right = cx - nButtonWidth;
		stRect.left = stRect.right - nWidth;
		pNumSpin->MoveWindow(&stRect);

		nButtonWidth += nWidth;
	}
	CEdit *pNumEdit = ((CEdit *) this->GetDlgItem(IDD_IU_COM_NUM_EXEC_TIMES_EDIT));
	// 値入力エリアをウィンドウサイズに合わせる
	if (pNumEdit != NULL && IsWindow(pNumEdit->GetSafeHwnd()) == TRUE) {
		pNumEdit->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		int nWidth = stRect.right - stRect.left;
		stRect.right = cx - nButtonWidth;
		stRect.left = stRect.right - nWidth;
		pNumEdit->MoveWindow(&stRect);

		nButtonWidth += nWidth;
	}

	CStatic *pTitleLabel = ((CStatic *) this->GetDlgItem(IDD_IU_COM_NUM_EXEC_TITLE_LABEL));
	// タイトルエリアをウィンドウサイズに合わせる
	if (pTitleLabel != NULL && IsWindow(pTitleLabel->GetSafeHwnd()) == TRUE) {
		pTitleLabel->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		stRect.right = cx - nButtonWidth;
		pTitleLabel->MoveWindow(&stRect);
	}
}

/*!
 @brief タイトルラベルに文字列を設定
*/
void CIUCommonNumExecDlg::SetTitle(CString &strTitle)
{
	CString strTempTitle = strTitle;
	strTempTitle += _T(":");

	GetDlgItem(IDD_IU_COM_NUM_EXEC_TITLE_LABEL)->SetWindowText(strTempTitle);
}


/*!
 @brief 情報取得\n
 指定コマンドを実行して情報を取得します。

 @return 情報
*/
CString CIUCommonNumExecDlg::GetInformation(BOOL)
{
	return _T("");
}