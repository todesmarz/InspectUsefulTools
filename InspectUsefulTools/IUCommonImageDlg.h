/*! @file  IUCommonImageDlg.h
    @brief ���ʃC���[�W�`���ʃN���X�w�b�_
*/
#pragma once
#include "afxwin.h"

/*!
 @class CIUCommonImageDlg
 @brief ���ʃC���[�W�`���ʃN���X
*/
class CIUCommonImageDlg : public CDialog
{
	DECLARE_DYNAMIC(CIUCommonImageDlg)

private:
	CImage *m_pBitmap;

protected:
	CComboBox m_ctrTargetCombo;
	int m_nSpaceCount;							/// Space key down count

public:
	CIUCommonImageDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	~CIUCommonImageDlg();   // �W���R���X�g���N�^


// �_�C�A���O �f�[�^
	enum { IDD = IDD_IU_COM_IMAGE_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeIuComImageTargetCombo();

	DECLARE_MESSAGE_MAP()

	virtual BOOL InitializeCombobox(CComboBox *pComboBox) = 0;
	virtual BOOL GetBitmapImage(LPVOID pSelectItem, CImage &bitmap) = 0;

	int GetSpaceKeyDownCount();
};
