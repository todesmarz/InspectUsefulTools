/*! @file  IUCommonFileDlg.h
    @brief 共通ファイル情報表示画面クラスヘッダ
*/
#pragma once

/*!
 @class CIUCommonFileDlg
 @brief 共通ファイル情報表示画面クラス
*/
class CIUCommonFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CIUCommonFileDlg)

public:
	CIUCommonFileDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

	virtual CString GetInformation(BOOL bInit) = 0;
	virtual void ExecuteOperation() = 0;
	void SetButtonText(LPCTSTR pszTitle);
	void SetTitle(LPCTSTR pszTitle);

// ダイアログ データ
	enum { IDD = IDD_IU_COM_FILE_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnEnChangeIuComFileFilepathEdit();
	afx_msg void OnBnClickedIuComFileSelectFileButton();
	afx_msg void OnBnClickedIuComFileExecButton();

	DECLARE_MESSAGE_MAP()
};
