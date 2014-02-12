/*! @file  FileFolderOpenDlg.h
    @brief フォルダオープン画面クラスヘッダ
*/
#pragma once
#include "afxwin.h"

/*!
 @class CFileFolderOpenDlg
 @brief フォルダオープン画面クラス
*/
class CFileFolderOpenDlg : public CDialog
{
	DECLARE_DYNAMIC(CFileFolderOpenDlg)

private:
	CString m_strFolderPath;				/// ファイルパス
	CListBox m_ctrlFileListBox;				/// ファイル履歴コントロール

public:
	CFileFolderOpenDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_FILE_FOLDER_OPEN_DIALOG };

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	afx_msg void OnBnClickedFilePathSelectButton();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnEnChangeFilePathEdit();
	afx_msg void OnBnClickedFileFolderOpenButton();
	afx_msg void OnLbnSelchangeFileFolderOpenRecentlyFileList();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
private:
	static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData);
};
