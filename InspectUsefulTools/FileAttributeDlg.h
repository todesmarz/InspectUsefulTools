/*! @file  FileAttributeDlg.h
    @brief ファイル属性変更画面クラスヘッダ
*/
#pragma once

/*!
 @class CFileAttributeDlg
 @brief ファイル属性変更画面クラス
*/
class CFileAttributeDlg : public CDialog
{
	DECLARE_DYNAMIC(CFileAttributeDlg)

private:
	CString m_strAttributeFilePath;				/// ファイルパス

public:
	CFileAttributeDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_FILE_ATTRIBUTE_DIALOG };

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	afx_msg void OnBnClickedFileAttributePathSelectButton();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedFileAttributeOpenButton();
	afx_msg void OnBnClickedFileAttributeSaveButton();
	afx_msg void OnEnChangeFileAttributePathEdit();

	DECLARE_MESSAGE_MAP()
};
