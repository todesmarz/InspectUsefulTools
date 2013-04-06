/*! @file  FileTimeStampDlg.h
    @brief ファイルタイムスタンプ変更画面クラスヘッダ
*/
#pragma once

/*!
 @class CFileTimeStampDlg
 @brief ファイルタイムスタンプ変更画面クラス
*/
class CFileTimeStampDlg : public CDialog
{
	DECLARE_DYNAMIC(CFileTimeStampDlg)

private:
	CString m_strFileLockPath;				/// ファイルパス

public:
	CFileTimeStampDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_FILE_TIMESTAMP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedFileTimestampPathSelectButton();
	afx_msg void OnEnChangeFileTimestampPathEdit();
	afx_msg void OnBnClickedFileTimestampSetButton();
	afx_msg void OnBnClickedFileTimestampResetButton();
	afx_msg void OnDropFiles(HDROP hDropInfo);

	DECLARE_MESSAGE_MAP()
};
