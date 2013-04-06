/*! @file  IUCommonComandDlg.h
    @brief 共通コマンド実行画面クラスヘッダ
*/
#pragma once

/*!
 @class CIUCommonComandDlg
 @brief 共通コマンド実行画面クラス
*/
class CIUCommonComandDlg : public CDialog
{
	DECLARE_DYNAMIC(CIUCommonComandDlg)

private:
	CString m_strCommandLine;					/// 対象コマンド

public:
	CIUCommonComandDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

	void SetCommandLine(CString strCommandLine);
	void SetTitle(CString &strTitle);
	virtual CString GetInformation(BOOL bInit);

// ダイアログ データ
	enum { IDD = IDD_IU_COM_COMMAND_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedIuComCommandExecButton();

	DECLARE_MESSAGE_MAP()
};
