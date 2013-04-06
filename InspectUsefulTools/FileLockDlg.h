/*! @file  FileLockDlg.h
    @brief ファイルロック実行画面クラスヘッダ
*/
#pragma once

/*!
 @class CFileLockDlg
 @brief ファイルロック実行画面クラス
*/
class CFileLockDlg : public CDialog
{
private:
	CString m_strFileLockPath;				/// ファイルパス

	CMapStringToPtr m_mapFileHandle;
	int m_bShareDenyWrite;
	int m_bShareDenyRead;
	int m_bShareDenyDelete;

	DECLARE_DYNAMIC(CFileLockDlg)

public:
	CFileLockDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_FILE_LOCK_DIALOG };

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	afx_msg void OnBnClickedFileLockPathSelectButton();
	afx_msg void OnBnClickedFileLockRegistButton();
	afx_msg void OnBnClickedFileLockCancelButton();
	afx_msg void OnEnChangeFileLockPathEdit();
	afx_msg void OnDropFiles(HDROP hDropInfo);

	DECLARE_MESSAGE_MAP()
};
