/*! @file  ImageFontDlg.h
    @brief フォントイメージ描画画面クラスヘッダ
*/
#pragma once
#include "resource.h"
#include "IUCommonImageDlg.h"

typedef struct {
	int x;
	int y;
	COLORREF color;
} SCursorInfo;

/*!
 @class CImageCursorInfoDlg
 @brief カーソル情報イメージ描画画面クラス
*/
class CImageCursorInfoDlg : public CIUCommonImageDlg
{
private:
	CArray<SCursorInfo, SCursorInfo> m_aryCurosrInfo;

public:
	CImageCursorInfoDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

protected:
	virtual BOOL InitializeCombobox(CComboBox *pComboBox);
	virtual BOOL GetBitmapImage(LPVOID pSelectItem, CImage &bitmap);
};
