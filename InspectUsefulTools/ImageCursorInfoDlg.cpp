/*! @file  ImageFontDlg.cpp
    @brief フォントイメージ描画画面クラス
*/
#include "StdAfx.h"
#include "ImageCursorInfoDlg.h"

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CImageCursorInfoDlg::CImageCursorInfoDlg(CWnd* pParent /*=NULL*/)
	: CIUCommonImageDlg(pParent)
{

}

/*!
 @brief コンボボックス初期化処理

 @param [in]    pComboBox     初期化対象のコンボボックス
 */
BOOL CImageCursorInfoDlg::InitializeCombobox(CComboBox *pComboBox)
{
	SetTimer(1, 2000, NULL);

	return TRUE;
}

/*!
 @brief イメージの取得

 @param [in]    pSelectItem     選択データ
 @param [out]   bitmap          イメージ
 */
BOOL CImageCursorInfoDlg::GetBitmapImage(LPVOID pSelectItem, CImage &bitmap)
{
	CRect rect;
	GetClientRect(&rect);

	bitmap.Create(rect.Width(), rect.Height(), 32);

	HDC hDC = bitmap.GetDC();
	Gdiplus::Graphics graphics(hDC);
	graphics.Clear((Gdiplus::ARGB)Gdiplus::Color::White);

	POINT pt;
	GetCursorPos(&pt);
	
	HDC hDesktopDC = ::GetDC(HWND_DESKTOP);
	::ScreenToClient(HWND_DESKTOP, &pt);
	COLORREF color = GetPixel( hDesktopDC, pt.x, pt.y);
	::ReleaseDC(HWND_DESKTOP, hDesktopDC);

	SCursorInfo newInfo;
	newInfo.x = pt.x;
	newInfo.y = pt.y;
	newInfo.color = color;

	BOOL bExistSetting = FALSE;
	// すでに登録済みかチェック
	for (int nAryIndex = 0; nAryIndex < m_aryCurosrInfo.GetCount(); nAryIndex++) {
		if (m_aryCurosrInfo[nAryIndex].color == newInfo.color) {
			bExistSetting = TRUE;
			break;
		}
	}
	if (bExistSetting == FALSE) {
		m_aryCurosrInfo.Add(newInfo);

		while (10 < m_aryCurosrInfo.GetCount()) {
			// 10以上超えた場合は、古いデータから削除する
			m_aryCurosrInfo.RemoveAt(0);
		}
	}

	CString strMessage;
	for (int nAryIndex = 0; nAryIndex < m_aryCurosrInfo.GetCount(); nAryIndex++) {
		SCursorInfo info = m_aryCurosrInfo[nAryIndex];

		strMessage.Format(_T("%s%d: Point : (x=%4d y=%4d)  Color : #%06X\n"), strMessage, (nAryIndex+1), info.x, info.y, info.color);
		Gdiplus::Color rectColor;
		rectColor.SetFromCOLORREF(info.color);
		
		Gdiplus::SolidBrush rectBrush(rectColor);
		graphics.FillRectangle(&rectBrush, 50.0f * nAryIndex, 0.0f, 50.0f, (Gdiplus::REAL)rect.Height());
	}

	Gdiplus::Font font(_T("メイリオ"), 12.0f);

	Gdiplus::RectF drawLayout(0, 0, (Gdiplus::REAL)rect.Width(), (Gdiplus::REAL)rect.Height());

	Gdiplus::StringFormat stringFormat;
	stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
	stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	stringFormat.SetTrimming(Gdiplus::StringTrimmingNone);

	Gdiplus::SolidBrush brush((Gdiplus::ARGB)Gdiplus::Color::Black);

	graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	graphics.DrawString(strMessage, -1, &font, drawLayout, &stringFormat,&brush);

	
	bitmap.ReleaseDC();

	return TRUE;
}

