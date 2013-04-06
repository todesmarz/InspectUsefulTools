/*! @file  DummyFileSizeDlg.h
    @brief ダミーファイル作成実行画面クラスヘッダ
*/
#pragma once

/*!
 @class CDummyFileSizeDlg
 @brief ダミーファイル作成実行画面クラス
*/
class CDummyFileSizeDlg : public CDialog
{
	DECLARE_DYNAMIC(CDummyFileSizeDlg)
private:
	CString m_strFilePath;						/// ファイルパス
	CString m_strFileSize;						/// ファイルサイズ
	int m_nDataUnit;							/// 単位

public:
	CDummyFileSizeDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_DUMMY_FILE_SIZE_DIALOG };

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedDummyFileExecButton();
	afx_msg void OnBnClickedFileAttributePathSelectButton();
	afx_msg void OnEnChangeDummyFilePathEdit();

	DECLARE_MESSAGE_MAP()
};
