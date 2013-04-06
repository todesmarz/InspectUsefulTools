/*! @file  TimeStopWatchDlg.cpp
    @brief ストップウォッチ画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "TimeStopWatchDlg.h"

IMPLEMENT_DYNAMIC(CTimeStopWatchDlg, CDialog)

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CTimeStopWatchDlg::CTimeStopWatchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeStopWatchDlg::IDD, pParent)
	, m_strStopWatchTime(_T(""))
{

}

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
void CTimeStopWatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TIME_STOPWATCH_TIME_EDIT, m_strStopWatchTime);
}

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CTimeStopWatchDlg, CDialog)
	ON_BN_CLICKED(IDC_TIME_STOPWATCH_START_BUTTON, &CTimeStopWatchDlg::OnBnClickedTimeStopwatchStartButton)
	ON_BN_CLICKED(IDC_TIME_STOPWATCH_STOP_BUTTON, &CTimeStopWatchDlg::OnBnClickedTimeStopwatchStopButton)
	ON_BN_CLICKED(IDC_TIME_STOPWATCH_RESET_BUTTON, &CTimeStopWatchDlg::OnBnClickedTimeStopwatchResetButton)
	ON_WM_TIMER()
END_MESSAGE_MAP()

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CTimeStopWatchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bStart = FALSE;
	m_inStartTime = 0;
	m_inCurrentTime = 0;
	GetCurrentStopWatchTime();
	UpdateData(FALSE);

	return TRUE;
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CTimeStopWatchDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
        return 0;
        //  必ず０で戻ること!!
        //  0以外で戻ると各コントロールへ次の処理が回らなくなる
    }
    return CDialog::PreTranslateMessage(pMsg);
}

/*!
 @brief 開始ボタンをクリックすると呼び出されます。
*/
void CTimeStopWatchDlg::OnBnClickedTimeStopwatchStartButton()
{
	m_inStartTime = clock();
	SetTimer(1, 10, NULL);
	m_bStart = TRUE;

	GetDlgItem(IDC_TIME_STOPWATCH_START_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_TIME_STOPWATCH_STOP_BUTTON)->EnableWindow(TRUE);
}

/*!
 @brief 停止ボタンをクリックすると呼び出されます。
*/
void CTimeStopWatchDlg::OnBnClickedTimeStopwatchStopButton()
{
	KillTimer(1);
	m_bStart = FALSE;

	GetDlgItem(IDC_TIME_STOPWATCH_START_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_TIME_STOPWATCH_STOP_BUTTON)->EnableWindow(FALSE);
}

/*!
 @brief リセットボタンをクリックすると呼び出されます。
*/
void CTimeStopWatchDlg::OnBnClickedTimeStopwatchResetButton()
{
	if (m_bStart == TRUE) {
		m_inStartTime = clock();
	} else {
		m_inStartTime = 0;
	}
	m_inCurrentTime = 0;

	GetCurrentStopWatchTime();
}

/*!
 @brief 現在の計測時間を表示します。
*/
void CTimeStopWatchDlg::GetCurrentStopWatchTime()
{
	long long nDiff = m_inCurrentTime - m_inStartTime;

	// ミリ秒
	int nMilliSecond = nDiff % 1000;

	// 秒
	int nSecond = nDiff / 1000 % 60;

	// 分
	int nMinute = nDiff / (1000 * 60) % 60;

	// 時
	int nHour = nDiff / (1000 * 60 * 60) % 24;

	m_strStopWatchTime.Format(_T("%02d:%02d:%02d.%03d"), nHour, nMinute, nSecond, nMilliSecond);
	UpdateData(False);
}


/*!
 @brief タイマーイベントで呼び出されます。
*/
void CTimeStopWatchDlg::OnTimer(UINT_PTR nIDEvent)
{
	m_inCurrentTime = clock();
	GetCurrentStopWatchTime();

	CDialog::OnTimer(nIDEvent);
}
