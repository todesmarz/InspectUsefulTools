/*! @file  TimeChangeTimeDlg.h
    @brief システム時刻変更画面クラスヘッダ
*/
#pragma once
#include "afxwin.h"

/*!
 @class CTimeChangeTimeDlg
 @brief ダミーファイル作成実行画面クラス
*/
class CTimeChangeTimeDlg : public CDialog
{
	DECLARE_DYNAMIC(CTimeChangeTimeDlg)

private:
	CTime m_inChangeDate;
	CTime m_inChangeTime;
	CComboBox m_inTimeZoneCombobox;

public:
	CTimeChangeTimeDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_TIME_CHANGE_TIME_DIALOG };

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	afx_msg void OnBnClickedImeChangeTimeTimeSetButton();
	afx_msg void OnBnClickedImeChangeTimeTimezoneSetButton();

	DECLARE_MESSAGE_MAP()
};
