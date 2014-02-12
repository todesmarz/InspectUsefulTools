/*! @file  IUCommonImageDlg.cpp
    @brief ���ʃC���[�W�`���ʃN���X
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "IUCommonImageDlg.h"

IMPLEMENT_DYNAMIC(CIUCommonImageDlg, CDialog)

/*!
 @brief �R���X�g���N�^

 @param [in]    pParent     �e�E�B���h�E�C���X�^���X
*/
CIUCommonImageDlg::CIUCommonImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIUCommonImageDlg::IDD, pParent)
{
	m_pBitmap = NULL;
	m_nSpaceCount = 0;
}

/*!
 @brief �f�X�g���N�^
*/
CIUCommonImageDlg::~CIUCommonImageDlg()
{
	if (m_pBitmap != NULL) {
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
}

/*!
 @brief �_�C�A���O �f�[�^�̌����ƗL�����`�F�b�N�̂��߂Ƀt���[�����[�N���Ăяo���܂��B

 @param [in]    pDX     CDataExchange �I�u�W�F�N�g�ւ̃|�C���^
*/
void CIUCommonImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IU_COM_IMAGE_TARGET_COMBO, m_ctrTargetCombo);
}

/*!
 @brief ���b�Z�[�W�}�b�v��`
*/
BEGIN_MESSAGE_MAP(CIUCommonImageDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_IU_COM_IMAGE_TARGET_COMBO, &CIUCommonImageDlg::OnCbnSelchangeIuComImageTargetCombo)
END_MESSAGE_MAP()

/*!
 @brief ���̃��\�b�h�� WM_INITDIALOG �̃��b�Z�[�W�ɉ������ČĂяo����܂��B
*/
BOOL CIUCommonImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitializeCombobox(&m_ctrTargetCombo);

	return TRUE;
}

/*!
 @brief Windows �֐� TranslateMessage �� DispatchMessage �Ƀf�B�X�p�b�`�����O�ɁA�E�B���h�E ���b�Z�[�W���t�B���^��������ɂ́A���̊֐����I�[�o�[���C�h���܂��B\n
 ����̎����ł́A�A�N�Z�����[�^ �L�[�̕ϊ����s���܂��B\n
 ���̂��߁A�I�[�o�[���C�h���� CWinApp::PreTranslateMessage �����o�֐����Ăяo���K�v������܂��B
*/
BOOL CIUCommonImageDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
        return 0;
        //  �K���O�Ŗ߂邱��!!
        //  0�ȊO�Ŗ߂�Ɗe�R���g���[���֎��̏��������Ȃ��Ȃ�
    }
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE) {
		m_nSpaceCount++;
	
		int nSel = m_ctrTargetCombo.GetCurSel();
		if (0 < m_ctrTargetCombo.GetCount() && nSel < 0) {
			return 0;
		}

		CImage inBitmap;
		void *pSelItem = m_ctrTargetCombo.GetItemDataPtr(nSel);
		if (GetBitmapImage(pSelItem, inBitmap) == TRUE) {
			if (m_pBitmap != NULL) {
				delete m_pBitmap;
				m_pBitmap = NULL;
			}

			// �C���[�W�̃R�s�[
			m_pBitmap = new CImage();
			m_pBitmap->Create(inBitmap.GetWidth(), inBitmap.GetHeight(), inBitmap.GetBPP());
			HDC hDC = m_pBitmap->GetDC();
			inBitmap.BitBlt(hDC, 0, 0);
			m_pBitmap->ReleaseDC();

			this->Invalidate(TRUE);
		}
	}
    return CDialog::PreTranslateMessage(pMsg);
}

/*!
 @brief ���T�C�Y�C�x���g\n
 WM_SIZE�ŌĂяo����܂��B

 @param [in]    nType  ���T�C�Y�^�C�v
 @param [in]    cx     ��
 @param [in]    cy     ����
 */
void CIUCommonImageDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	RECT stRect;

	CComboBox *pCombo = ((CComboBox *) this->GetDlgItem(IDC_IU_COM_IMAGE_TARGET_COMBO));
	// �^�C�g���G���A���E�B���h�E�T�C�Y�ɍ��킹��
	if (pCombo != NULL && IsWindow(pCombo->GetSafeHwnd()) == TRUE) {
		pCombo->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		stRect.right = cx;
		pCombo->MoveWindow(&stRect);
	}
	
	int nSel = m_ctrTargetCombo.GetCurSel();
	if (0 < m_ctrTargetCombo.GetCount() && nSel < 0) {
		return;
	}
	
	if (this->IsWindowVisible() == false) {
		// ��\�����ɂ́A�Ȃɂ����Ȃ�
		return;
	}

	CImage inBitmap;
	void *pSelItem = m_ctrTargetCombo.GetItemDataPtr(nSel);
	if (GetBitmapImage(pSelItem, inBitmap) == TRUE) {
		if (m_pBitmap != NULL) {
			delete m_pBitmap;
			m_pBitmap = NULL;
		}

		// �C���[�W�̃R�s�[
		m_pBitmap = new CImage();
		m_pBitmap->Create(inBitmap.GetWidth(), inBitmap.GetHeight(), inBitmap.GetBPP());
		HDC hDC = m_pBitmap->GetDC();
		inBitmap.BitBlt(hDC, 0, 0);
		m_pBitmap->ReleaseDC();

		this->Invalidate(TRUE);
	}
}

/*!
 @brief�`�揈��
*/
void CIUCommonImageDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (m_pBitmap == NULL) {
		return;
	}

	// �`��
	m_pBitmap->BitBlt(dc.GetSafeHdc(), 0, 0);
}


/*!
 @brief �^�[�Q�b�g�ύX�C�x���g
*/
void CIUCommonImageDlg::OnCbnSelchangeIuComImageTargetCombo()
{
	int nSel = m_ctrTargetCombo.GetCurSel();
	if (nSel < 0) {
		return;
	}

	CImage inBitmap;
	void *pSelItem = m_ctrTargetCombo.GetItemDataPtr(nSel);
	if (GetBitmapImage(pSelItem, inBitmap) == TRUE) {
		if (m_pBitmap != NULL) {
			delete m_pBitmap;
			m_pBitmap = NULL;
		}

		// �C���[�W�̃R�s�[
		m_pBitmap = new CImage();
		m_pBitmap->Create(inBitmap.GetWidth(), inBitmap.GetHeight(), inBitmap.GetBPP());
		HDC hDC = m_pBitmap->GetDC();
		inBitmap.BitBlt(hDC, 0, 0);
		m_pBitmap->ReleaseDC();

		this->Invalidate(TRUE);
	}
}


/*!
 @brief �^�C�}�[�C�x���g�ŌĂяo����܂��B
*/
void CIUCommonImageDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (this->IsWindowVisible() == false) {
		// ��\�����ɂ́A�Ȃɂ����Ȃ�
		return;
	}

	int nSel = m_ctrTargetCombo.GetCurSel();
	if (0 < m_ctrTargetCombo.GetCount() && nSel < 0) {
		return;
	}

	CImage inBitmap;
	void *pSelItem = m_ctrTargetCombo.GetItemDataPtr(nSel);
	if (GetBitmapImage(pSelItem, inBitmap) == TRUE) {
		if (m_pBitmap != NULL) {
			delete m_pBitmap;
			m_pBitmap = NULL;
		}

		// �C���[�W�̃R�s�[
		m_pBitmap = new CImage();
		m_pBitmap->Create(inBitmap.GetWidth(), inBitmap.GetHeight(), inBitmap.GetBPP());
		HDC hDC = m_pBitmap->GetDC();
		inBitmap.BitBlt(hDC, 0, 0);
		m_pBitmap->ReleaseDC();

		this->Invalidate(TRUE);
	}

	CDialog::OnTimer(nIDEvent);
}

/*!
 @brief �X�y�[�X�L�[�����񐔂̎擾\n

 @return �X�y�[�X�L�[�����񐔁@
*/
int CIUCommonImageDlg::GetSpaceKeyDownCount()
{
	return m_nSpaceCount;
}