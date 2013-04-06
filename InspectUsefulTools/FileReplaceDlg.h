/*! @file  FileReplaceDlg.h
    @brief ファイル置換画面クラスヘッダ
*/
#pragma once

/*!
 @class CFileReplaceDlg
 @brief ファイル置換画面クラス
*/
class CFileReplaceDlg : public CDialog
{
	DECLARE_DYNAMIC(CFileReplaceDlg)

private:
	CString m_strReplaceFilePath;				/// ファイルパス
	CString m_strExtName;
	CString m_strAfterReplace;
	CString m_strBeforeReplace;

public:
	CFileReplaceDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_FILE_REPLACE_DIALOG };

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedFileReplacePathSelectButton();
	afx_msg void OnBnClickedFileReplaceExecButton();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeFileReplacePathEdit();
};
