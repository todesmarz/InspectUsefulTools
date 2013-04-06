/*! @file  TimeStopWatchDlg.h
    @brief ストップウォッチ画面クラスヘッダ
*/
#pragma once

/*!
 @class CTimeStopWatchDlg
 @brief ストップウォッチ画面クラス
*/
class CTimeStopWatchDlg : public CDialog
{
private:
	CString m_strStopWatchTime;
	long long m_inStartTime;
	long long m_inCurrentTime;
	BOOL m_bStart;

	DECLARE_DYNAMIC(CTimeStopWatchDlg)

public:
	CTimeStopWatchDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_TIME_STOPWATCH_DIALOG };

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	afx_msg void OnBnClickedTimeStopwatchStartButton();
	afx_msg void OnBnClickedTimeStopwatchStopButton();
	afx_msg void OnBnClickedTimeStopwatchResetButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

private:
	void GetCurrentStopWatchTime();
};
