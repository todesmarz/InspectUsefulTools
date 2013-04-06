/*! @file  UserInfoDlg.cpp
    @brief ユーザ情報表示画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "UserInfoDlg.h"

#include <lm.h>

IMPLEMENT_DYNAMIC(CUserInfoDlg, CDialog)

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CUserInfoDlg::CUserInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserInfoDlg::IDD, pParent)
	, m_strUserName(_T(""))
	, m_strFullUserName(_T(""))
	, m_strDomainName(_T(""))
	, m_bAdminUser(FALSE)
	, m_strGroupName(_T(""))
{

}

/*!
 @brief ダイアログ データの交換と有効性チェックのためにフレームワークが呼び出します。

 @param [in]    pDX     CDataExchange オブジェクトへのポインタ
*/
void CUserInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDD_SYS_USER_INFO_USER_NAME_EDIT, m_strUserName);
	DDX_Text(pDX, IDD_SYS_USER_INFO_FULL_NAME_EDIT, m_strFullUserName);
	DDX_Text(pDX, IDD_SYS_USER_INFO_DOMAIN_NAME_EDIT, m_strDomainName);
	DDX_Check(pDX, IDD_SYS_USER_INFO_ADMIN_CHECK, m_bAdminUser);
	DDX_Text(pDX, IDD_SYS_USER_INFO_GROUP_NAME_EDIT, m_strGroupName);
}

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CUserInfoDlg, CDialog)
END_MESSAGE_MAP()

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CUserInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ExecUserInfo();

	return TRUE;
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CUserInfoDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
        return 0;
        //  必ず０で戻ること!!
        //  0以外で戻ると各コントロールへ次の処理が回らなくなる
    }
    return CDialog::PreTranslateMessage(pMsg);
}

/*!
 @brief ユーザ情報の表示
*/
void CUserInfoDlg::ExecUserInfo()
{
	TCHAR szUserName[UNLEN + 1];
	DWORD nUserNameSize = sizeof(szUserName) / sizeof(TCHAR);
	GetUserName(szUserName, &nUserNameSize);
	m_strUserName = szUserName;

	TCHAR szDomainName[DNLEN + 1];
	DWORD nDomainNameSize = sizeof(szDomainName) / sizeof(TCHAR);
	GetComputerNameEx(ComputerNameDnsDomain, szDomainName, & nDomainNameSize);
	m_strDomainName = szDomainName;

	TCHAR szFullUserName[UNLEN + 1];
	ZeroMemory(szFullUserName, sizeof(szFullUserName));
	GetFullName(szUserName, szDomainName, szFullUserName);
	m_strFullUserName = szFullUserName;

	m_bAdminUser = IsUserAnAdmin();

	TCHAR szGroupName[(GNLEN + 1) * 5];
	ZeroMemory(szGroupName, sizeof(szGroupName));
	GetGroupName(szUserName, szGroupName);
	m_strGroupName = szGroupName;	
	m_strGroupName.TrimLeft(_T(','));

	UpdateData(FALSE);
}

/*!
 @brief ユーザフルネームの取得 (MBCS)
*/
BOOL CUserInfoDlg::GetFullNameA(char *UserName, char *Domain, char *dest)
{
	WCHAR  wszUserName[256];           // Unicode user name
	WCHAR  wszDomain[256];
	LPBYTE ComputerName = 0;
  
	struct _USER_INFO_2 *ui;          // User structure

	// Convert ASCII user name and domain to Unicode.
	MultiByteToWideChar(CP_ACP, 0, UserName, strlen(UserName)+1, wszUserName, sizeof(wszUserName)  / sizeof(WCHAR));
	MultiByteToWideChar(CP_ACP, 0, Domain, strlen(Domain)+1, wszDomain, sizeof(wszDomain) / sizeof(WCHAR) );

	BOOL bFoundDC = TRUE;
	DWORD nRet = NetGetDCName(NULL, NULL, &ComputerName );
	// Get the computer name of a DC for the specified domain.
	if (nRet != NERR_Success) {
		printf("Error getting user information.\n" );
		bFoundDC = FALSE;
	}

	// Look up the user on the DC.
	nRet = NetUserGetInfo((LPWSTR) ComputerName,
		(LPWSTR) wszUserName, 2, (LPBYTE *) &ui);
	if (nRet != NERR_Success) {
		if (bFoundDC == TRUE) {
			NetApiBufferFree(ComputerName);
		}
		printf("Error getting user information.\n" );
		return(FALSE );
	}
	if (bFoundDC == TRUE) {
		NetApiBufferFree(ComputerName);
	}

	// Convert the Unicode full name to ASCII.
	WideCharToMultiByte(CP_ACP, 0, ui->usri2_full_name, -1, dest, 256, NULL, NULL );
	
	return(TRUE );
}

/*!
 @brief ユーザフルネームの取得 (Unicode)
*/
BOOL CUserInfoDlg::GetFullNameW(wchar_t *UserName, wchar_t *, wchar_t *dest)
{
	LPBYTE ComputerName = 0;
  
	struct _USER_INFO_2 *ui;          // User structure

	BOOL bFoundDC = TRUE;
	DWORD nRet = NetGetDCName(NULL, NULL, &ComputerName );
	// Get the computer name of a DC for the specified domain.
	if (nRet != NERR_Success) {
		printf("Error getting user information.\n" );
		bFoundDC = FALSE;
	}

	// Look up the user on the DC.
	nRet = NetUserGetInfo((LPWSTR) ComputerName,
		(LPWSTR) UserName, 2, (LPBYTE *) &ui);
	if (nRet != NERR_Success) {

		if (bFoundDC == TRUE) {
			NetApiBufferFree(ComputerName);
		}
		printf("Error getting user information.\n" );
		return(FALSE );
	}
	if (bFoundDC == TRUE) {
		NetApiBufferFree(ComputerName);
	}

	wcsncpy_s(dest, 256, ui->usri2_full_name, _TRUNCATE);
	
	return(TRUE );
}

/*!
 @brief ユーザグループの取得 (MBCS)
*/
BOOL CUserInfoDlg::GetGroupNameA(char *UserName, char *dest)
{
	WCHAR  wszUserName[256];           // Unicode user name
	LPBYTE ComputerName = 0;
  
	// Convert ASCII user name and domain to Unicode.
	MultiByteToWideChar(CP_ACP, 0, UserName, strlen(UserName) + 1, wszUserName, sizeof(wszUserName) / sizeof(WCHAR));

	// Get the computer name of a DC for the specified domain.
	BOOL bFoundDC = TRUE;
	DWORD nRet = NetGetDCName(NULL, NULL, &ComputerName );
	if (nRet != NERR_Success) {
		printf("Error getting group information.\n" );
		bFoundDC = FALSE;
	}

	LPLOCALGROUP_USERS_INFO_0 pBuf = NULL;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;	// Look up the user on the DC.
	nRet = NetUserGetLocalGroups((LPWSTR) ComputerName,
		(LPWSTR) wszUserName, 0, LG_INCLUDE_INDIRECT, (LPBYTE *) &pBuf, MAX_PREFERRED_LENGTH, &dwEntriesRead, &dwTotalEntries);
	if (nRet != NERR_Success) {
		if (bFoundDC == TRUE) {
			NetApiBufferFree(ComputerName);
		}
		printf("Error getting group information.\n" );
		return(FALSE );
	}
	if (bFoundDC == TRUE) {
		NetApiBufferFree(ComputerName);
	}

	LPLOCALGROUP_USERS_INFO_0 pTmpBuf = pBuf;
	for (unsigned int i = 0; i < dwEntriesRead; i++) {
		if (pTmpBuf == NULL) {
		   fprintf(stderr, "An access violation has occurred\n");
		   break;
		}
		strcat_s(dest, (GNLEN + 1) * 5, ",");

		char szGroupName[GNLEN + 1];
		// Convert the Unicode full name to ASCII.
		WideCharToMultiByte(CP_ACP, 0, pTmpBuf->lgrui0_name, -1, szGroupName, GNLEN, NULL, NULL );

		strcat_s(dest, (GNLEN + 1) * 5, szGroupName);
		pTmpBuf++;
	}
	NetApiBufferFree(pBuf);

	
	return (TRUE);
}

/*!
 @brief ユーザグループの取得 (Unicode)
*/
BOOL CUserInfoDlg::GetGroupNameW(TCHAR *UserName, TCHAR *dest)
{
	LPBYTE ComputerName = 0;
  
	// Get the computer name of a DC for the specified domain.
	BOOL bFoundDC = TRUE;
	DWORD nRet = NetGetDCName(NULL, NULL, &ComputerName );
	if (nRet != NERR_Success) {
		printf("Error getting group information.\n" );
		bFoundDC = FALSE;
	}

	LPLOCALGROUP_USERS_INFO_0 pBuf = NULL;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;	// Look up the user on the DC.
	nRet = NetUserGetLocalGroups((LPWSTR) ComputerName,
		(LPWSTR) UserName, 0, LG_INCLUDE_INDIRECT, (LPBYTE *) &pBuf, MAX_PREFERRED_LENGTH, &dwEntriesRead, &dwTotalEntries);
	if (nRet != NERR_Success) {
		if (bFoundDC == TRUE) {
			NetApiBufferFree(ComputerName);
		}
		printf("Error getting group information.\n" );
		return (FALSE);
	}
	if (bFoundDC == TRUE) {
		NetApiBufferFree(ComputerName);
	}

	LPLOCALGROUP_USERS_INFO_0 pTmpBuf = pBuf;
	for (unsigned int i = 0; i <dwEntriesRead; i++) {
		if (pTmpBuf == NULL) {
		   fprintf(stderr, "An access violation has occurred\n");
		   break;
		}
		wcscat_s(dest, (GNLEN + 1) * 5, L",");
		wcscat_s(dest, (GNLEN + 1) * 5, pTmpBuf->lgrui0_name);
		pTmpBuf++;
	}
	NetApiBufferFree(pBuf);

	return (TRUE);
}