/*! @file  IUCommonInfoDlg.h
    @brief 共通情報表示画面クラスヘッダ
*/
#pragma once

/*!
 @class CIUCommonInfoDlg
 @brief 共通情報表示画面クラス
*/
class CIUCommonInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CIUCommonInfoDlg)

private:
	CDialog *m_pDialog;						/// 表示 子画面
	CString m_strResult;					/// 結果
	CString m_strType;						/// 画面-識別子
	CFindReplaceDialog *m_inFindDialog;		/// 検索ダイアログ

	static UINT WM_FINDREPLACE;

public:
	CIUCommonInfoDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CIUCommonInfoDlg();

	void SetInformationType(LPCTSTR szType);

// ダイアログ データ
	enum { IDD = IDD_IU_COM_INFO_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedIuComInfoUpdateButton();
	afx_msg LONG OnFindReplace(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	void PerformInformationType();
	void DisplayInformation(BOOL bInit);
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
