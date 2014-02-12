/*! @file  IUCommonImageDlg.cpp
    @brief 共通イメージ描画画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "IUCommonImageDlg.h"

IMPLEMENT_DYNAMIC(CIUCommonImageDlg, CDialog)

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CIUCommonImageDlg::CIUCommonImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIUCommonImageDlg::IDD, pParent)
{
	m_pBitmap = NULL;
	m_nSpaceCount = 0;
}

/*!
 @brief デストラクタ
*/
CIUCommonImageDlg::~CIUCommonImageDlg()
{
	if (m_pBitmap != NULL) {
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
}

/*!
 @brief ダイアログ データの交換と有効性チェックのためにフレームワークが呼び出します。

 @param [in]    pDX     CDataExchange オブジェクトへのポインタ
*/
void CIUCommonImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IU_COM_IMAGE_TARGET_COMBO, m_ctrTargetCombo);
}

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CIUCommonImageDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_IU_COM_IMAGE_TARGET_COMBO, &CIUCommonImageDlg::OnCbnSelchangeIuComImageTargetCombo)
END_MESSAGE_MAP()

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CIUCommonImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitializeCombobox(&m_ctrTargetCombo);

	return TRUE;
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CIUCommonImageDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
        return 0;
        //  必ず０で戻ること!!
        //  0以外で戻ると各コントロールへ次の処理が回らなくなる
    }
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE) {
		m_nSpaceCount++;
	
		int nSel = m_ctrTargetCombo.GetCurSel();
		if (0 < m_ctrTargetCombo.GetCount() && nSel < 0) {
			return 0;
		}

		CImage inBitmap;
		void *pSelItem = m_ctrTargetCombo.GetItemDataPtr(nSel);
		if (GetBitmapImage(pSelItem, inBitmap) == TRUE) {
			if (m_pBitmap != NULL) {
				delete m_pBitmap;
				m_pBitmap = NULL;
			}

			// イメージのコピー
			m_pBitmap = new CImage();
			m_pBitmap->Create(inBitmap.GetWidth(), inBitmap.GetHeight(), inBitmap.GetBPP());
			HDC hDC = m_pBitmap->GetDC();
			inBitmap.BitBlt(hDC, 0, 0);
			m_pBitmap->ReleaseDC();

			this->Invalidate(TRUE);
		}
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
void CIUCommonImageDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	RECT stRect;

	CComboBox *pCombo = ((CComboBox *) this->GetDlgItem(IDC_IU_COM_IMAGE_TARGET_COMBO));
	// タイトルエリアをウィンドウサイズに合わせる
	if (pCombo != NULL && IsWindow(pCombo->GetSafeHwnd()) == TRUE) {
		pCombo->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		stRect.right = cx;
		pCombo->MoveWindow(&stRect);
	}
	
	int nSel = m_ctrTargetCombo.GetCurSel();
	if (0 < m_ctrTargetCombo.GetCount() && nSel < 0) {
		return;
	}
	
	if (this->IsWindowVisible() == false) {
		// 非表示時には、なにもしない
		return;
	}

	CImage inBitmap;
	void *pSelItem = m_ctrTargetCombo.GetItemDataPtr(nSel);
	if (GetBitmapImage(pSelItem, inBitmap) == TRUE) {
		if (m_pBitmap != NULL) {
			delete m_pBitmap;
			m_pBitmap = NULL;
		}

		// イメージのコピー
		m_pBitmap = new CImage();
		m_pBitmap->Create(inBitmap.GetWidth(), inBitmap.GetHeight(), inBitmap.GetBPP());
		HDC hDC = m_pBitmap->GetDC();
		inBitmap.BitBlt(hDC, 0, 0);
		m_pBitmap->ReleaseDC();

		this->Invalidate(TRUE);
	}
}

/*!
 @brief描画処理
*/
void CIUCommonImageDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (m_pBitmap == NULL) {
		return;
	}

	// 描画
	m_pBitmap->BitBlt(dc.GetSafeHdc(), 0, 0);
}


/*!
 @brief ターゲット変更イベント
*/
void CIUCommonImageDlg::OnCbnSelchangeIuComImageTargetCombo()
{
	int nSel = m_ctrTargetCombo.GetCurSel();
	if (nSel < 0) {
		return;
	}

	CImage inBitmap;
	void *pSelItem = m_ctrTargetCombo.GetItemDataPtr(nSel);
	if (GetBitmapImage(pSelItem, inBitmap) == TRUE) {
		if (m_pBitmap != NULL) {
			delete m_pBitmap;
			m_pBitmap = NULL;
		}

		// イメージのコピー
		m_pBitmap = new CImage();
		m_pBitmap->Create(inBitmap.GetWidth(), inBitmap.GetHeight(), inBitmap.GetBPP());
		HDC hDC = m_pBitmap->GetDC();
		inBitmap.BitBlt(hDC, 0, 0);
		m_pBitmap->ReleaseDC();

		this->Invalidate(TRUE);
	}
}


/*!
 @brief タイマーイベントで呼び出されます。
*/
void CIUCommonImageDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (this->IsWindowVisible() == false) {
		// 非表示時には、なにもしない
		return;
	}

	int nSel = m_ctrTargetCombo.GetCurSel();
	if (0 < m_ctrTargetCombo.GetCount() && nSel < 0) {
		return;
	}

	CImage inBitmap;
	void *pSelItem = m_ctrTargetCombo.GetItemDataPtr(nSel);
	if (GetBitmapImage(pSelItem, inBitmap) == TRUE) {
		if (m_pBitmap != NULL) {
			delete m_pBitmap;
			m_pBitmap = NULL;
		}

		// イメージのコピー
		m_pBitmap = new CImage();
		m_pBitmap->Create(inBitmap.GetWidth(), inBitmap.GetHeight(), inBitmap.GetBPP());
		HDC hDC = m_pBitmap->GetDC();
		inBitmap.BitBlt(hDC, 0, 0);
		m_pBitmap->ReleaseDC();

		this->Invalidate(TRUE);
	}

	CDialog::OnTimer(nIDEvent);
}

/*!
 @brief スペースキー押下回数の取得\n

 @return スペースキー押下回数　
*/
int CIUCommonImageDlg::GetSpaceKeyDownCount()
{
	return m_nSpaceCount;
}