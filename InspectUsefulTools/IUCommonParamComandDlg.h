/*! @file  IUCommonParamComandDlg.h
    @brief 共通引数ありコマンド実行画面クラスヘッダ
*/
#pragma once

/*!
 @class CIUCommonParamComandDlg
 @brief 共通引数ありコマンド実行画面クラス
*/
class CIUCommonParamComandDlg : public CDialog
{
	DECLARE_DYNAMIC(CIUCommonParamComandDlg)

private:
	CString m_strCommandLine;					/// 対象コマンド

	CString m_strParam1Format;					/// パラメータ1コマンドフォーマット
	CString m_strParam2Format;					/// パラメータ2コマンドフォーマット
	CString m_strParam3Format;					/// パラメータ3コマンドフォーマット
	CString m_strParam4Format;					/// パラメータ4コマンドフォーマット

	int m_nParamLineNumber;						/// 表示行数

public:
	CIUCommonParamComandDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

	void SetCommandLine(CString strCommandLine);
	void SetTitle(CString &strTitle);
	void SetCmdParam(int nParam, LPCTSTR pszParamTitle = NULL, LPCTSTR pszParamDefault = NULL, LPCTSTR pszParamFormat = NULL);  
	virtual CString GetInformation(BOOL bInit);

	void PackWindowRect();

// ダイアログ データ
	enum { IDD = IDD_IU_COM_PARAM_CMD_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedIuComParamCmdExecButton();

	DECLARE_MESSAGE_MAP()
};
