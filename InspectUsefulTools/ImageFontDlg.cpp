/*! @file  ImageFontDlg.cpp
    @brief フォントイメージ描画画面クラス
*/
#include "StdAfx.h"
#include "ImageFontDlg.h"

CArray<LOGFONT, LOGFONT&> CImageFontDlg::m_aryLogfont;

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CImageFontDlg::CImageFontDlg(CWnd* pParent /*=NULL*/)
	: CIUCommonImageDlg(pParent)
{

}

/*!
 @brief コンボボックス初期化処理

 @param [in]    pComboBox     初期化対象のコンボボックス
 */
BOOL CImageFontDlg::InitializeCombobox(CComboBox *pComboBox)
{
	// フォントファミリー設定
    CClientDC dc(this);
    LOGFONT logfont; 
    ZeroMemory(&logfont, sizeof(LOGFONT)); 

	m_aryLogfont.RemoveAll();
	logfont.lfCharSet = DEFAULT_CHARSET;
	//コールバック関数の引数lParamにpComboBoxのポインターを代入する
    ::EnumFontFamiliesEx(dc.m_hDC, &logfont, (FONTENUMPROC)EnumFontFamExProc, (long)pComboBox, 0);

	for (int nIndex = 0; nIndex < m_aryLogfont.GetCount(); nIndex++) {
		LOGFONT *pLogfont = &m_aryLogfont[nIndex];
		pComboBox->SetItemDataPtr(nIndex, pLogfont);
	}

	pComboBox->SetCurSel(0);

	return TRUE;
}

int CALLBACK CImageFontDlg::EnumFontFamExProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX * /*lpntme*/, int FontType, LPARAM lParam)
{
	//CFontTestDlgのm_cmbFont(コンボ ボックス)のポインターを代入する
    CComboBox* pComboBox = (CComboBox*)lParam;

	if (FontType != TRUETYPE_FONTTYPE) {
		return TRUE;
	}

	//列挙されたフォント名をコンボ ボックスのリストに追加する
	CString strFontName;
	strFontName.Format(_T("%s (%s)"), lpelfe->elfFullName, lpelfe->elfScript);

	int nIndex = pComboBox->AddString(strFontName);
	int nLogFontIndex = m_aryLogfont.Add(lpelfe->elfLogFont);
	LOGFONT *pLogfont = &m_aryLogfont[nLogFontIndex];
	pComboBox->SetItemDataPtr(nIndex, pLogfont);

    return TRUE;
}

/*!
 @brief イメージの取得

 @param [in]    pSelectItem     選択データ
 @param [out]   bitmap          イメージ
 */
BOOL CImageFontDlg::GetBitmapImage(LPVOID pSelectItem, CImage &bitmap)
{
	CRect rect;
	GetClientRect(&rect);

	bitmap.Create(rect.Width(), rect.Height(), 32);

	HDC hDC = bitmap.GetDC();
	Gdiplus::Graphics graphics(hDC);
	graphics.Clear((Gdiplus::ARGB)Gdiplus::Color::White);

	CString strMessage;
	strMessage = _T("1234567890\n");
	strMessage += _T("abcdefghijklmnopqrstuvwxyz\n");
	strMessage += _T("ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
	strMessage += _T("あいおえおかきくけこさしすせそたちつてとなにぬねの\n");
	strMessage += _T("はひふへほまみむめもやゆよらりるれろわをん\n");

	LOGFONT *pLogfont = (LOGFONT *) pSelectItem;
	Gdiplus::Font font(hDC, pLogfont);

	Gdiplus::RectF drawLayout(0, 0, (Gdiplus::REAL)rect.Width(), (Gdiplus::REAL)rect.Height());

	Gdiplus::StringFormat stringFormat;
	stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
	stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	stringFormat.SetTrimming(Gdiplus::StringTrimmingNone);

	Gdiplus::SolidBrush brush((Gdiplus::ARGB)Gdiplus::Color::Black);

	graphics.SetTextRenderingHint((Gdiplus::TextRenderingHint) (GetSpaceKeyDownCount() % (int)Gdiplus::TextRenderingHintClearTypeGridFit));
	graphics.DrawString(strMessage, -1, &font, drawLayout, &stringFormat,&brush);


	bitmap.ReleaseDC();

	return TRUE;
}
