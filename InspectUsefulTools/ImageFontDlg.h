/*! @file  ImageFontDlg.h
    @brief フォントイメージ描画画面クラスヘッダ
*/
#pragma once
#include "resource.h"
#include "IUCommonImageDlg.h"

/*!
 @class CImageFontDlg
 @brief フォントイメージ描画画面クラス
*/
class CImageFontDlg :
	public CIUCommonImageDlg
{
private:
	static CArray<LOGFONT, LOGFONT&> m_aryLogfont;

public:
	CImageFontDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

protected:
	virtual BOOL InitializeCombobox(CComboBox *pComboBox);
	virtual BOOL GetBitmapImage(LPVOID pSelectItem, CImage &bitmap);

private:
	static int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, int FontType, LPARAM lParam);

};
