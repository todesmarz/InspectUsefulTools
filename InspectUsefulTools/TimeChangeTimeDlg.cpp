/*! @file  CTimeChangeTimeDlg.cpp
    @brief システム時刻変更画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "TimeChangeTimeDlg.h"

IMPLEMENT_DYNAMIC(CTimeChangeTimeDlg, CDialog)

#define TMZ_NAME_SIZE (256)

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CTimeChangeTimeDlg::CTimeChangeTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeChangeTimeDlg::IDD, pParent)
	, m_inChangeDate(0)
	, m_inChangeTime(0)
{

}

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
void CTimeChangeTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_IME_CHANGE_TIME_DATE_DATETIMEPICKER1, m_inChangeDate);
	DDX_DateTimeCtrl(pDX, IDC_IME_CHANGE_TIME_TIME_DATETIMEPICKER2, m_inChangeTime);
	DDX_Control(pDX, IDC_IME_CHANGE_TIME_TIMEZONE_COMBO, m_inTimeZoneCombobox);
}

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CTimeChangeTimeDlg, CDialog)
	ON_BN_CLICKED(IDC_IME_CHANGE_TIME_TIME_SET_BUTTON, &CTimeChangeTimeDlg::OnBnClickedImeChangeTimeTimeSetButton)
	ON_BN_CLICKED(IDC_IME_CHANGE_TIME_TIMEZONE_SET_BUTTON, &CTimeChangeTimeDlg::OnBnClickedImeChangeTimeTimezoneSetButton)
END_MESSAGE_MAP()

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CTimeChangeTimeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_inChangeDate = CTime::GetCurrentTime();
	m_inChangeTime = CTime::GetCurrentTime();

	CString strTimeZoneRegPath = _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Time Zones");

	HKEY hKey;
	RegOpenKey(HKEY_LOCAL_MACHINE, strTimeZoneRegPath, &hKey);

	// タイムゾーンの取得
	CStringArray aryStdName;
	TCHAR szKeyName[TMZ_NAME_SIZE];
	DWORD dwDataIndex = 0;
	DWORD dwSize = sizeof(szKeyName) / sizeof(TCHAR);
	while (RegEnumKeyEx(hKey, dwDataIndex, szKeyName, &dwSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
		HKEY hSubKey;
		RegOpenKey(hKey, szKeyName, &hSubKey);

		TCHAR szDisplay[TMZ_NAME_SIZE];
		DWORD dwDisplaySize = sizeof(szDisplay) / sizeof(TCHAR);
		RegQueryValueEx(hSubKey, _T("Display"), NULL, NULL, (BYTE *) szDisplay, &dwDisplaySize);
		int nIndex = m_inTimeZoneCombobox.AddString(szDisplay);

		RegQueryValueEx(hSubKey, _T("Std"), NULL, NULL, (BYTE *) szDisplay, &dwDisplaySize);
		aryStdName.Add(szDisplay);

		TIME_ZONE_INFORMATION stInfo;
		DWORD dwIndexSize = sizeof(TIME_ZONE_INFORMATION);
		RegQueryValueEx(hSubKey, _T("TZI"), NULL, NULL, (BYTE *) &stInfo, &dwIndexSize);
		m_inTimeZoneCombobox.SetItemData(nIndex, stInfo.Bias);
		dwDataIndex++;
		dwSize = sizeof(szKeyName);

		RegCloseKey(hSubKey);
	}
	RegCloseKey(hKey);

	TIME_ZONE_INFORMATION stInfo;
	GetTimeZoneInformation(&stInfo);

	// 現在のタイムゾーンを判断
	int nIndex = 0;
	for (; nIndex < aryStdName.GetCount(); nIndex++) {
		if (aryStdName.GetAt(nIndex) == stInfo.StandardName) {
			break;
		}
	}

	m_inTimeZoneCombobox.SetCurSel(nIndex);

	UpdateData(FALSE);

	return TRUE;
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CTimeChangeTimeDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
        return 0;
        //  必ず０で戻ること!!
        //  0以外で戻ると各コントロールへ次の処理が回らなくなる
    }
    return CDialog::PreTranslateMessage(pMsg);
}

/*!
 @brief 時刻の設定ボタンをクリックすると呼び出されます。
*/
void CTimeChangeTimeDlg::OnBnClickedImeChangeTimeTimeSetButton()
{
	UpdateData(TRUE);
	SYSTEMTIME stSystemTime;
	m_inChangeDate.GetAsSystemTime(stSystemTime);
	
	stSystemTime.wHour = (WORD) m_inChangeTime.GetHour();
	stSystemTime.wMinute = (WORD) m_inChangeTime.GetMinute();
	stSystemTime.wSecond = (WORD) m_inChangeTime.GetSecond();

	SetLocalTime(&stSystemTime);
}

/*!
 @brief タイムゾーンの設定ボタンをクリックすると呼び出されます。
*/
void CTimeChangeTimeDlg::OnBnClickedImeChangeTimeTimezoneSetButton()
{
	TIME_ZONE_INFORMATION stInfo;
	ZeroMemory(&stInfo, sizeof(TIME_ZONE_INFORMATION));

	CString strTimeZoneRegPath = _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Time Zones");

	HKEY hKey;
	RegOpenKey(HKEY_LOCAL_MACHINE, strTimeZoneRegPath, &hKey);

	int nSelectTimezoneIndex = m_inTimeZoneCombobox.GetCurSel();
	CString strSelectTimezone;
	m_inTimeZoneCombobox.GetLBText(nSelectTimezoneIndex, strSelectTimezone);

	// タイムゾーンの取得
	CStringArray aryStdName;
	TCHAR szKeyName[TMZ_NAME_SIZE];
	DWORD dwDataIndex = 0;
	DWORD dwSize = sizeof(szKeyName) / sizeof(TCHAR);
	while (RegEnumKeyEx(hKey, dwDataIndex, szKeyName, &dwSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
		HKEY hSubKey;
		RegOpenKey(hKey, szKeyName, &hSubKey);

		// 対象のタイムゾーンか判定
		TCHAR szDisplay[TMZ_NAME_SIZE];
		DWORD dwDisplaySize = sizeof(szDisplay) / sizeof(TCHAR);
		RegQueryValueEx(hSubKey, _T("Display"), NULL, NULL, (BYTE *) szDisplay, &dwDisplaySize);		
		if (szDisplay != strSelectTimezone) {
			dwDataIndex++;
			dwSize = sizeof(szKeyName);
			continue;
		}

		// タイムゾーン値の取得
		DWORD dwIndexSize = sizeof(TIME_ZONE_INFORMATION);
		RegQueryValueEx(hSubKey, _T("TZI"), NULL, NULL, (BYTE *) &stInfo, &dwIndexSize);

		// 名称
		RegQueryValueEx(hSubKey, _T("Std"), NULL, NULL, (BYTE *) szDisplay, &dwDisplaySize);
		_tcscpy_s(stInfo.StandardName, sizeof(stInfo.StandardName) / sizeof(TCHAR), szDisplay);

		RegCloseKey(hSubKey);
		break;
	}
	RegCloseKey(hKey);

	SetTimeZoneInformation(&stInfo);
}
