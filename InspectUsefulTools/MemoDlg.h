/*! @file  MemoDlg.h
    @brief メモ画面クラスヘッダ
*/
#pragma once
#include "afxcmn.h"

/*!
 @class CMemoDlg
 @brief メモ画面クラス
*/
class CMemoDlg : public CDialog
{
	DECLARE_DYNAMIC(CMemoDlg)

private:
	CRichEditCtrl m_inMemoRichEdit;

public:
	CMemoDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_DISPLAY_MEMO_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};
