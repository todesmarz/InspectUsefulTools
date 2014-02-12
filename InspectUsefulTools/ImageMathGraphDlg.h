/*! @file  ImageMathGraphDlg.h
    @brief 数式グラフ描画画面クラスヘッダ
*/
#pragma once
#include "resource.h"
#include "IUCommonImageDlg.h"

/*!
 @class CImageMathGraphDlg
 @brief 数式グラフ描画画面クラス
*/
class CImageMathGraphDlg:
	public CIUCommonImageDlg
{
public:
	CImageMathGraphDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

protected:
	virtual BOOL InitializeCombobox(CComboBox *pComboBox);
	virtual BOOL GetBitmapImage(LPVOID pSelectItem, CImage &bitmap);

private:
	void DrawKochLine(Gdiplus::Graphics &graphic, const Gdiplus::Pen &pen, int n, double d, int x, int y, int w);
};
