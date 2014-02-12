/*! @file  InspectUsefulToolsApp.cpp
    @brief 当ツールアプリケーションクラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "InspectUsefulToolsDlg.h"
#include "IUSettingData.h"

#include <locale>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CInspectUsefulToolsApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

/*!
 @brief コンストラクション
*/
CInspectUsefulToolsApp::CInspectUsefulToolsApp()
{
}

/*!
 @brief アプリケーションクラスインスタンス
*/
CInspectUsefulToolsApp theApp;


/*!
 @brief 初期化処理
*/
BOOL CInspectUsefulToolsApp::InitInstance()
{
	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();
	AfxInitRichEdit();
	AfxInitRichEdit2();

	Gdiplus::GdiplusStartupInput gdiSI;
	ULONG_PTR gdiToken;
	Gdiplus::GdiplusStartup(&gdiToken, &gdiSI, NULL);

	::CoInitialize(NULL);
	setlocale(LC_ALL, ".ACP");

	{
		CInspectUsefulToolsDlg dlg;
		m_pMainWnd = &dlg;
		dlg.DoModal();
	}

	// 設定の保存
	CIUSettingData::GetInstance().Save();

	Gdiplus::GdiplusShutdown(gdiToken);
	::CoUninitialize();

	return FALSE;
}
