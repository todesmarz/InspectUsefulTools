/*! @file  ImageFontDlg.h
    @brief �t�H���g�C���[�W�`���ʃN���X�w�b�_
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
 @brief �J�[�\�����C���[�W�`���ʃN���X
*/
class CImageCursorInfoDlg : public CIUCommonImageDlg
{
private:
	CArray<SCursorInfo, SCursorInfo> m_aryCurosrInfo;

public:
	CImageCursorInfoDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^

protected:
	virtual BOOL InitializeCombobox(CComboBox *pComboBox);
	virtual BOOL GetBitmapImage(LPVOID pSelectItem, CImage &bitmap);
};
