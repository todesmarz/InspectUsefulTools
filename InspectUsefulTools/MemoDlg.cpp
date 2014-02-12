/*! @file  MemoDlg.cpp
    @brief メモ画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "MemoDlg.h"
#include "IUSettingData.h"

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
	ON_EN_CHANGE(IDC_DISPLAY_MEMO_RICHEDIT, &CMemoDlg::OnEnChangeDisplayMemoRichedit)
END_MESSAGE_MAP()

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CMemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 変更イベントのマスク
	m_inMemoRichEdit.SetEventMask(ENM_CHANGE);

	// This is hard-coded for example purposes. It is likely this would
	// be read from file or another source.
	EDITSTREAM es;
	es.dwCookie = (DWORD)&CIUSettingData::GetInstance().m_strMemo; // Pass a pointer to the CString to the callback function
	es.pfnCallback = MEditStreamInCallback; // Specify the pointer to the callback function

	m_inMemoRichEdit.StreamIn(SF_RTF,es); // Perform the streaming

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


void CMemoDlg::OnEnChangeDisplayMemoRichedit()
{
	// TODO:  これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialog::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO:  ここにコントロール通知ハンドラー コードを追加してください。

	//m_inMemoRichEdit.GetWindowText(CIUSettingData::GetInstance().m_strMemo);
	//m_inMemoRichEdit.GetTextRange(0, m_inMemoRichEdit.GetTextLength(), CIUSettingData::GetInstance().m_strMemo);

	EDITSTREAM es;

	es.dwCookie = (DWORD)&CIUSettingData::GetInstance().m_strMemo; // Pass a pointer to the CString to the callback function 
	es.pfnCallback = MEditStreamOutCallback; // Specify the pointer to the callback function.

	m_inMemoRichEdit.StreamOut(SF_RTF, es); // Perform the streaming
}

DWORD __stdcall MEditStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CString sThisWrite;
	sThisWrite.GetBufferSetLength(cb);

	CString *psBuffer = (CString *)dwCookie;
	
	for (int i=0;i<cb;i++) {
		sThisWrite.SetAt(i,*(pbBuff+i));
	}

	*psBuffer += sThisWrite;

	*pcb = sThisWrite.GetLength();
	sThisWrite.ReleaseBuffer();
	return 0;
}

DWORD __stdcall MEditStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CString *psBuffer = (CString *)dwCookie;

	int nBufferLength = psBuffer->GetLength();
	if (nBufferLength < cb ) cb = nBufferLength;

	for (int i=0; i< cb; i++)
	{
		*(pbBuff+i) = (BYTE) psBuffer->GetAt(i);
	}

	*pcb = cb;
	if (0 < cb) {
		*psBuffer = psBuffer->Mid(cb);
	}

	return 0;
}