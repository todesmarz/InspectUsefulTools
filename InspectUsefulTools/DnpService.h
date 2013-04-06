/*! @file  DnpService.h
    @brief サービス操作クラスヘッダ
*/
#pragma once

#include <windows.h>
#include <winsvc.h>


/*!
 @enum  EChangeServiceStatus_
 @brief サービスステータス列挙体
*/
typedef enum EChangeServiceStatus_ {
	Start,
	Stop,
	Restart,
	
} EChangeServiceStatus;

/*!
 @struct SServiceInfo_
 @brief  サービス情報構造体
*/
typedef struct SServiceInfo_ {
	CString strDispName;		/// 表示名称
	CString strServiceName;		/// サービス名称
	CString strDescript;		/// サービス説明
	CString strPath;			/// サービス実行ファイルパス
	CString strStatus;			/// サービスのステータス
} SServiceInfo;

/*!
 @class CDnpService
 @brief サービス操作クラス
*/
class CDnpService
{
private:
	bool m_bCancel;									/// サービスの起動／停止処理中断用変数、trueなら中断開始
	CComAutoCriticalSection	m_secbCancel;			/// サービスの起動／停止処理中断用クリティカルセクション
	CComAutoCriticalSection	m_secServiceProcess;	/// サービス処理中クリティカルセクション
	CString m_strServiceName;
	SC_HANDLE m_hManager;
	SC_HANDLE m_hService;
	DWORD m_dwWaitHint;
	SServiceInfo m_stServiceInfo;
	bool m_bChaningProcess;

public:
	CDnpService(SServiceInfo &stServiceInfo);
	~CDnpService(void);

public:
	bool IsStateChangeCancel(bool bSave=false,bool bNewValue=false);

	DWORD GetServiceStatus();
	bool IsRunning();
	bool IsStopped();
	bool IsPaused();
	bool IsStatusChanging();

	bool ChangeStatus(EChangeServiceStatus eStatus);
	bool OpenDnpService();
	bool CloseDnpService();

	bool GetServiceInfo(SServiceInfo &stServiceInfo);

	static bool GetServiceList(CArray<SServiceInfo> &aryServiceInfoList);

private:
	bool StartDnpService();
	bool StopDnpService();

	static UINT ChangeStatusThread(LPVOID pParam);
};
