/*! @file  IUCommonNumExecDlg.h
    @brief 共通指定回数実行画面クラスヘッダ
*/
#pragma once

/*!
 @class CIUCommonNumExecDlg
 @brief 共通指定回数実行画面クラス
*/
class CIUCommonNumExecDlg : public CDialog
{
	DECLARE_DYNAMIC(CIUCommonNumExecDlg)

public:
	CIUCommonNumExecDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

	void SetTitle(CString &strTitle);
	virtual CString GetInformation(BOOL bInit);

// ダイアログ データ
	enum { IDD = IDD_IU_COM_NUM_EXEC_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedIuComNumExecButton();

	DECLARE_MESSAGE_MAP()
};
