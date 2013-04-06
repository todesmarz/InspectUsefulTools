/*! @file  UserInfoDlg.h
    @brief ユーザ情報表示画面クラスヘッダ
*/
#pragma once


/*!
 @class CUserInfoDlg
 @brief ユーザ情報表示画面クラス
*/
class CUserInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserInfoDlg)

private:
	CString m_strUserName;
	CString m_strFullUserName;
	CString m_strDomainName;
	BOOL m_bAdminUser;
	CString m_strGroupName;

public:
	CUserInfoDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_SYS_USER_INFO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	void ExecUserInfo();

	BOOL GetFullNameA(char *UserName, char *Domain, char *dest);
	BOOL GetFullNameW(wchar_t *UserName, wchar_t *Domain, wchar_t *dest);

#ifdef UNICODE
#define GetFullName GetFullNameW
#else
#define GetFullName GetFullNameA
#endif

	BOOL GetGroupNameA(char *UserName, char *dest);
	BOOL GetGroupNameW(wchar_t *UserName, wchar_t *dest);

#ifdef UNICODE
#define GetGroupName GetGroupNameW
#else
#define GetGroupName GetGroupNameA
#endif
};
