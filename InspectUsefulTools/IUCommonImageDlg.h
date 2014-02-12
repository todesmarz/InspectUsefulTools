/*! @file  IUCommonImageDlg.h
    @brief 共通イメージ描画画面クラスヘッダ
*/
#pragma once
#include "afxwin.h"

/*!
 @class CIUCommonImageDlg
 @brief 共通イメージ描画画面クラス
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
	CIUCommonImageDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	~CIUCommonImageDlg();   // 標準コンストラクタ


// ダイアログ データ
	enum { IDD = IDD_IU_COM_IMAGE_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeIuComImageTargetCombo();

	DECLARE_MESSAGE_MAP()

	virtual BOOL InitializeCombobox(CComboBox *pComboBox) = 0;
	virtual BOOL GetBitmapImage(LPVOID pSelectItem, CImage &bitmap) = 0;

	int GetSpaceKeyDownCount();
};
