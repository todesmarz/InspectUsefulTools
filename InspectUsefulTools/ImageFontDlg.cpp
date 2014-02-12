/*! @file  ImageFontDlg.cpp
    @brief �t�H���g�C���[�W�`���ʃN���X
*/
#include "StdAfx.h"
#include "ImageFontDlg.h"

CArray<LOGFONT, LOGFONT&> CImageFontDlg::m_aryLogfont;

/*!
 @brief �R���X�g���N�^

 @param [in]    pParent     �e�E�B���h�E�C���X�^���X
*/
CImageFontDlg::CImageFontDlg(CWnd* pParent /*=NULL*/)
	: CIUCommonImageDlg(pParent)
{

}

/*!
 @brief �R���{�{�b�N�X����������

 @param [in]    pComboBox     �������Ώۂ̃R���{�{�b�N�X
 */
BOOL CImageFontDlg::InitializeCombobox(CComboBox *pComboBox)
{
	// �t�H���g�t�@�~���[�ݒ�
    CClientDC dc(this);
    LOGFONT logfont; 
    ZeroMemory(&logfont, sizeof(LOGFONT)); 

	m_aryLogfont.RemoveAll();
	logfont.lfCharSet = DEFAULT_CHARSET;
	//�R�[���o�b�N�֐��̈���lParam��pComboBox�̃|�C���^�[��������
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
	//CFontTestDlg��m_cmbFont(�R���{ �{�b�N�X)�̃|�C���^�[��������
    CComboBox* pComboBox = (CComboBox*)lParam;

	if (FontType != TRUETYPE_FONTTYPE) {
		return TRUE;
	}

	//�񋓂��ꂽ�t�H���g�����R���{ �{�b�N�X�̃��X�g�ɒǉ�����
	CString strFontName;
	strFontName.Format(_T("%s (%s)"), lpelfe->elfFullName, lpelfe->elfScript);

	int nIndex = pComboBox->AddString(strFontName);
	int nLogFontIndex = m_aryLogfont.Add(lpelfe->elfLogFont);
	LOGFONT *pLogfont = &m_aryLogfont[nLogFontIndex];
	pComboBox->SetItemDataPtr(nIndex, pLogfont);

    return TRUE;
}

/*!
 @brief �C���[�W�̎擾

 @param [in]    pSelectItem     �I���f�[�^
 @param [out]   bitmap          �C���[�W
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
	strMessage += _T("�����������������������������������ĂƂȂɂʂ˂�\n");
	strMessage += _T("�͂Ђӂւق܂݂ނ߂��������������\n");

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
