/*! @file  SysServiceInfoDlg.h
    @brief サービス情報表示画面クラスヘッダ
*/
#pragma once

#include "afxwin.h"
#include "DnpService.h"

/*!
 @class CSysServiceInfoDlg
 @brief サービス情報表示画面クラス
*/
class CSysServiceInfoDlg : public CDialog
{
private:
	CComboBox m_inServiceNameCombo;
	CArray<SServiceInfo> m_aryServiceInfoList;
	CDnpService *m_pDnpService;
	
	CString m_strServiceDescription;
	CString m_strServiceExePath;
	CString m_strServiceStatus;

	DECLARE_DYNAMIC(CSysServiceInfoDlg)

public:
	CSysServiceInfoDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSysServiceInfoDlg();

// ダイアログ データ
	enum { IDD = IDD_SYS_SERVICE_INFO_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	afx_msg void OnCbnSelchangeSysServiceInfoNameCombo();
	afx_msg void OnBnClickedSysServiceInfoStopButton();
	afx_msg void OnBnClickedSysServiceInfoRestartButton();
	afx_msg void OnBnClickedSysServiceInfoStartButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};
