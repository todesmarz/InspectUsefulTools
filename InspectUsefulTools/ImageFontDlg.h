/*! @file  ImageFontDlg.h
    @brief �t�H���g�C���[�W�`���ʃN���X�w�b�_
*/
#pragma once
#include "resource.h"
#include "IUCommonImageDlg.h"

/*!
 @class CImageFontDlg
 @brief �t�H���g�C���[�W�`���ʃN���X
*/
class CImageFontDlg :
	public CIUCommonImageDlg
{
private:
	static CArray<LOGFONT, LOGFONT&> m_aryLogfont;

public:
	CImageFontDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^

protected:
	virtual BOOL InitializeCombobox(CComboBox *pComboBox);
	virtual BOOL GetBitmapImage(LPVOID pSelectItem, CImage &bitmap);

private:
	static int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, int FontType, LPARAM lParam);

};
