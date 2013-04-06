/*! @file  MemoDlg.cpp
    @brief メモ画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "MemoDlg.h"

IMPLEMENT_DYNAMIC(CMemoDlg, CDialog)

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CMemoDlg::CMemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMemoDlg::IDD, pParent)
{

}

/*!
 @brief ダイアログ データの交換と有効性チェックのためにフレームワークが呼び出します。

 @param [in]    pDX     CDataExchange オブジェクトへのポインタ
*/
void CMemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DISPLAY_MEMO_RICHEDIT, m_inMemoRichEdit);
}

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CMemoDlg, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CMemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CMemoDlg::PreTranslateMessage(MSG* pMsg)
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
void CMemoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	RECT stRect;
	if (m_inMemoRichEdit.m_hWnd != NULL) {
		m_inMemoRichEdit.GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		stRect.bottom = cy;
		stRect.right = cx;
		m_inMemoRichEdit.MoveWindow(&stRect);
	}	
}
