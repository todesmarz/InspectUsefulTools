/*! @file  IUCommonLockDlg.h
    @brief 共通ロック画面クラス ヘッダ
*/
#pragma once

/*!
 @class CIUCommonLockDlg
 @brief 共通ロック画面クラス
*/
class CIUCommonLockDlg : public CDialog
{
	DECLARE_DYNAMIC(CIUCommonLockDlg)

private:
	CMapStringToPtr m_mapLockInfo;			/// ロック済みデータ情報 

public:
	CIUCommonLockDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CIUCommonLockDlg();

// ダイアログ データ
	enum { IDD = IDD_IU_COM_LOCK_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual BOOL LockObject(CString &strLockName, HANDLE &hLockHandle) = 0;
	virtual BOOL UnlockObject(CString &strLockName, HANDLE hLockHandle) = 0;
	afx_msg void OnBnClickedIuComLockLockButton();
	afx_msg void OnBnClickedIuComLockUnlockButton();
	afx_msg void OnEnChangeIuComLockNameEdit();

	DECLARE_MESSAGE_MAP()

private:
	void ModifyButtonEnable();
};
