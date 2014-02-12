/*! @file  ImageMathGraphDlg.cpp
    @brief 数式グラフ描画画面クラス
*/
#include "StdAfx.h"
#include "ImageMathGraphDlg.h"

#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CImageMathGraphDlg::CImageMathGraphDlg(CWnd* pParent /*=NULL*/)
	: CIUCommonImageDlg(pParent)
{

}

/*!
 @brief コンボボックス初期化処理

 @param [in]    pComboBox     初期化対象のコンボボックス
 */
BOOL CImageMathGraphDlg::InitializeCombobox(CComboBox *pComboBox)
{
	// フォントファミリー設定
    CClientDC dc(this);

	int nIndex = 0;
	nIndex = pComboBox->AddString(_T("Sin"));
	pComboBox->SetItemData(nIndex, nIndex);

	nIndex = pComboBox->AddString(_T("Cos"));
	pComboBox->SetItemData(nIndex, nIndex);

	nIndex = pComboBox->AddString(_T("Tan"));
	pComboBox->SetItemData(nIndex, nIndex);

	nIndex = pComboBox->AddString(_T("y=x"));
	pComboBox->SetItemData(nIndex, nIndex);

	nIndex = pComboBox->AddString(_T("y=x^2"));
	pComboBox->SetItemData(nIndex, nIndex);

	nIndex = pComboBox->AddString(_T("コッホ曲線"));
	pComboBox->SetItemData(nIndex, nIndex);

	pComboBox->SetCurSel(0);

	return TRUE;
}

/*!
 @brief イメージの取得

 @param [in]    pSelectItem     選択データ
 @param [out]   bitmap          イメージ
 */
BOOL CImageMathGraphDlg::GetBitmapImage(LPVOID pSelectItem, CImage &bitmap)
{
	CRect rect;
	GetClientRect(&rect);

	bitmap.Create(rect.Width(), rect.Height(), 32);

	HDC hDC = bitmap.GetDC();
	Gdiplus::Graphics graphics(hDC);
	graphics.Clear((Gdiplus::ARGB)Gdiplus::Color::White);

	time_t nowTime = time(NULL);
	Gdiplus::Pen pen((COLORREF) nowTime, 2.0F);

	Gdiplus::Point centerPoint;
	centerPoint.X = rect.Width() /2;
	centerPoint.Y = rect.Height() /2;

	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	int nType = (int) pSelectItem;
	switch (nType)
	{
		case 0:
			{
				// Sin
				double nPreY = sin((double) ((rect.left -1)  % 360 / 180.0 * M_PI));
				for (int nIndex = rect.left; nIndex < rect.right; nIndex++) {
					double nY = sin((double) (nIndex % 360) / 180.0 * M_PI);
					graphics.DrawLine(&pen, (Gdiplus::REAL)nIndex - 1, (Gdiplus::REAL)nPreY * 100 + centerPoint.Y, (Gdiplus::REAL)nIndex, (Gdiplus::REAL)nY  * 100 + centerPoint.Y);
					nPreY = nY;
				}
			}
			break;
		case 1:
			{
				// cos
				double nPreY = cos((double) ((rect.left -1)  % 360 / 180.0 * M_PI));
				for (int nIndex = rect.left; nIndex < rect.right; nIndex++) {
					double nY = cos((double) (nIndex % 360) / 180.0 * M_PI);
					graphics.DrawLine(&pen, (Gdiplus::REAL)nIndex - 1,(Gdiplus::REAL) nPreY * 100 + centerPoint.Y, (Gdiplus::REAL)nIndex, (Gdiplus::REAL)nY  * 100 + centerPoint.Y);
					nPreY = nY;
				}
			}
			break;
		case 2:
			{
				// tan
				double nPreY = tan((double) ((rect.left -1)  % 360 / 180.0 * M_PI));
				for (int nIndex = rect.left; nIndex < rect.right; nIndex++) {
					double nY = tan((double) (nIndex % 360) / 180.0 * M_PI);
					graphics.DrawLine(&pen, (Gdiplus::REAL)nIndex - 1, (Gdiplus::REAL)nPreY * 100 + centerPoint.Y, (Gdiplus::REAL)nIndex, (Gdiplus::REAL)nY  * 100 + centerPoint.Y);
					nPreY = nY;
				}
			}
			break;
		case 3:
			{
				// y=x
				for (int nIndex = rect.left; nIndex < rect.right; nIndex++) {
					graphics.DrawLine(&pen, nIndex - 1, nIndex - 1, nIndex, nIndex);
				}
			}
			break;
		case 4:
			{
				// y=x^2
				double nPreY = pow((double)rect.left - 1 - centerPoint.X, 2);
				for (int nIndex = rect.left; nIndex < rect.right; nIndex++) {
					double nY = pow((double)nIndex - centerPoint.X, 2);
					graphics.DrawLine(&pen, (Gdiplus::REAL)nIndex - 1, (Gdiplus::REAL)(rect.Height() - nPreY), (Gdiplus::REAL)nIndex, (Gdiplus::REAL)(rect.Height() - nY));
					nPreY = nY;
				}
			}
			break;
		case 5:
			{
				// y=x^2
				Gdiplus::Rect kochRect(rect.left, rect.top, rect.Width(), rect.Height());
				Gdiplus::Point point1 = centerPoint;
				Gdiplus::Point point2(centerPoint.X + 1, centerPoint.Y);
				DrawKochLine(graphics, pen, 6, rect.Width(), 0, rect.Height(), 0);
			}
			break;
	}

	bitmap.ReleaseDC();

	return TRUE;
}

void CImageMathGraphDlg::DrawKochLine(Gdiplus::Graphics &graphic, const Gdiplus::Pen &pen, int n, double d, int x, int y, int w)
{
	if( n == 0 ) {
		double v = w / 180.0 * M_PI;
		int x1 = x + (int)(d * cos(v));
		int y1 = y + (int)(d * sin(v));

		graphic.DrawLine(&pen, x , y, x1, y1); // 角度 w で長さ d の直線を描画
          
		return;
	}

	d = d/3; // 基準の長さを 1/3にする

	DrawKochLine(graphic, pen, n-1, d, x, y, w); // 1つ下のレベルを描画

	double v = w / 180.0 * M_PI;
	x = x + (int)(d * cos(v));
	y = y + (int)(d * sin(v)); // 前進

	w = w -60; // -60度向きを変えて描画
	DrawKochLine(graphic, pen, n-1, d, x, y, w);
        
	v = w / 180.0 * M_PI;
	x = x + (int)(d * cos(v));
	y = y + (int)(d * sin(v)); // 前進

	w = w +120; // +120度向きを変えて描画
	DrawKochLine(graphic, pen, n-1, d, x, y, w);

	v = w / 180.0 * M_PI;
	x = x + (int)(d * cos(v));
	y = y + (int)(d * sin(v)); // 前進

	w = w -60; // -60度向きを変えて描画
	DrawKochLine(graphic, pen, n-1, d, x, y, w);
}
