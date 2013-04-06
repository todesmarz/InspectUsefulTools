/*! @file  SysServiceInfoDlg.cpp
    @brief サービス情報表示画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "SysServiceInfoDlg.h"

IMPLEMENT_DYNAMIC(CSysServiceInfoDlg, CDialog)

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CSysServiceInfoDlg::CSysServiceInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSysServiceInfoDlg::IDD, pParent)
	, m_strServiceDescription(_T(""))
	, m_strServiceExePath(_T(""))
	, m_strServiceStatus(_T(""))
{
	m_pDnpService = NULL;
}

/*!
 @brief デストラクタ
*/
CSysServiceInfoDlg::~CSysServiceInfoDlg()
{
	if (m_pDnpService != NULL) {
		delete m_pDnpService;
		m_pDnpService = NULL;
	}
}

/*!
 @brief ダイアログ データの交換と有効性チェックのためにフレームワークが呼び出します。

 @param [in]    pDX     CDataExchange オブジェクトへのポインタ
*/
void CSysServiceInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SYS_SERVICE_INFO_NAME_COMBO, m_inServiceNameCombo);
	DDX_Text(pDX, IDC_SYS_SERVICE_INFO_DESCRIPT_EDIT, m_strServiceDescription);
	DDX_Text(pDX, IDC_SYS_SERVICE_INFO_EXEC_PATH_EDIT, m_strServiceExePath);
	DDX_Text(pDX, IDC_SYS_INF_INFO_STATUS_LABEL, m_strServiceStatus);
}

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CSysServiceInfoDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_SYS_SERVICE_INFO_NAME_COMBO, &CSysServiceInfoDlg::OnCbnSelchangeSysServiceInfoNameCombo)
	ON_BN_CLICKED(IDC_SYS_SERVICE_INFO_STOP_BUTTON, &CSysServiceInfoDlg::OnBnClickedSysServiceInfoStopButton)
	ON_BN_CLICKED(IDC_SYS_SERVICE_INFO_RESTART_BUTTON, &CSysServiceInfoDlg::OnBnClickedSysServiceInfoRestartButton)
	ON_BN_CLICKED(IDC_SYS_SERVICE_INFO_START_BUTTON, &CSysServiceInfoDlg::OnBnClickedSysServiceInfoStartButton)
	ON_WM_TIMER()
END_MESSAGE_MAP()

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CSysServiceInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_aryServiceInfoList.RemoveAll();
	CDnpService::GetServiceList(m_aryServiceInfoList);

	// サービス名を登録
	for (int nIndex = 0; nIndex < m_aryServiceInfoList.GetCount(); nIndex++) {
		m_inServiceNameCombo.AddString(m_aryServiceInfoList[nIndex].strDispName);
	}
	m_inServiceNameCombo.SetCurSel(0);

	OnCbnSelchangeSysServiceInfoNameCombo();

	return TRUE;
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CSysServiceInfoDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
        return 0;
        //  必ず０で戻ること!!
        //  0以外で戻ると各コントロールへ次の処理が回らなくなる
    }
    return CDialog::PreTranslateMessage(pMsg);
}

/*!
 @brief コンボボックスの選択が変更された時に呼び出されます。
*/
void CSysServiceInfoDlg::OnCbnSelchangeSysServiceInfoNameCombo()
{
	if (m_pDnpService != NULL) {
		delete m_pDnpService;
	}

	int nIndex = m_inServiceNameCombo.GetCurSel();

	if (nIndex < m_aryServiceInfoList.GetCount()) {
		SServiceInfo &stServiceInfo = m_aryServiceInfoList[nIndex];

		// 選択したサービスの情報を取得
		m_pDnpService = new CDnpService(stServiceInfo);
		if (m_pDnpService->GetServiceInfo(stServiceInfo) == true) {
			m_strServiceDescription = stServiceInfo.strDescript;
			m_strServiceExePath = stServiceInfo.strPath;
			m_strServiceStatus = stServiceInfo.strStatus;

			UpdateData(FALSE);
		}
	}
}

/*!
 @brief サービス開始ボタンをクリックすると呼び出されます。
*/
void CSysServiceInfoDlg::OnBnClickedSysServiceInfoStartButton()
{
	if (m_pDnpService == NULL) {
		return;
	}

	// 開始
	if (m_pDnpService->ChangeStatus(Start) == true) {
		SetTimer(1, 500, NULL);
	}
}


/*!
 @brief サービス停止ボタンをクリックすると呼び出されます。
*/
void CSysServiceInfoDlg::OnBnClickedSysServiceInfoStopButton()
{
	if (m_pDnpService == NULL) {
		return;
	}

	// 停止
	if (m_pDnpService->ChangeStatus(Stop) == true) {
		SetTimer(1, 500, NULL);
	}
}

/*!
 @brief サービス再起動ボタンをクリックすると呼び出されます。
*/
void CSysServiceInfoDlg::OnBnClickedSysServiceInfoRestartButton()
{
	if (m_pDnpService == NULL) {
		return;
	}

	// 再起動
	if (m_pDnpService->ChangeStatus(Restart) == true) {
		SetTimer(1, 500, NULL);
	}
}

/*!
 @brief タイーマーで呼び出されます。
*/
void CSysServiceInfoDlg::OnTimer(UINT_PTR nIDEvent)
{
	int nIndex = m_inServiceNameCombo.GetCurSel();
	if (nIndex < m_aryServiceInfoList.GetCount()) {
		SServiceInfo &stServiceInfo = m_aryServiceInfoList[nIndex];

		// 現在表示しているサービスの情報を取得
		if (m_pDnpService->GetServiceInfo(stServiceInfo) == true) {
			m_strServiceDescription = stServiceInfo.strDescript;
			m_strServiceExePath = stServiceInfo.strPath;
			m_strServiceStatus = stServiceInfo.strStatus;

			UpdateData(FALSE);
		}
	}

	if (m_pDnpService->IsStatusChanging() == false) {
		// 存在しないサービスを表示している場合
		KillTimer(1);
	}

	CDialog::OnTimer(nIDEvent);
}
