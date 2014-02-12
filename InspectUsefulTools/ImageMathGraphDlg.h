/*! @file  ImageMathGraphDlg.h
    @brief �����O���t�`���ʃN���X�w�b�_
*/
#pragma once
#include "resource.h"
#include "IUCommonImageDlg.h"

/*!
 @class CImageMathGraphDlg
 @brief �����O���t�`���ʃN���X
*/
class CImageMathGraphDlg:
	public CIUCommonImageDlg
{
public:
	CImageMathGraphDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^

protected:
	virtual BOOL InitializeCombobox(CComboBox *pComboBox);
	virtual BOOL GetBitmapImage(LPVOID pSelectItem, CImage &bitmap);

private:
	void DrawKochLine(Gdiplus::Graphics &graphic, const Gdiplus::Pen &pen, int n, double d, int x, int y, int w);
};
