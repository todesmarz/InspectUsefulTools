/*! @file  DnpService.cpp
    @brief サービス操作クラス
*/
#pragma once

#include "stdafx.h"
#include "Resource.h"
#include "DnpService.h"

/*!
 @brief コンストラクタ

 @param [in]    stServiceInfo     サービス情報
*/
CDnpService::CDnpService(SServiceInfo &stServiceInfo){
	m_bCancel = false;
	m_strServiceName = stServiceInfo.strServiceName;
	m_stServiceInfo = stServiceInfo;
	m_hService = NULL;

	OpenDnpService();
}

/*!
 @brief デストラクタ
*/
CDnpService::~CDnpService(){
	CloseDnpService();
}

/*!
 @brief サービスを開く
*/
bool CDnpService::OpenDnpService()
{
	m_hManager = 	::OpenSCManager(NULL, NULL, GENERIC_EXECUTE);
	if(m_hManager == NULL) {
		return false;
	}

	m_hService = ::OpenService(m_hManager, m_strServiceName, SERVICE_START | SERVICE_STOP | SERVICE_QUERY_STATUS | SERVICE_QUERY_CONFIG);
	ATLASSERT(m_hService);
	if(m_hService != NULL) {
		return false;
	}

	return true;
}

/*!
 @brief サービスを閉じる
*/
bool CDnpService::CloseDnpService()
{
	if(m_hService != NULL) {
		::CloseServiceHandle(m_hService);
		m_hService = NULL;
	}

	if(m_hManager != NULL) {
		::CloseServiceHandle(m_hManager);
		m_hManager = NULL;
	}

	return true;
}

/*!
 @brief サービスの状態変更\n
　中断したい場合はIsStateChangeCancel(true,true)を呼び出す

 @param [in]    bSave      
 @param [in]    bNewValue     
*/
bool CDnpService::IsStateChangeCancel(bool bSave, bool bNewValue)
{
	bool ret;

	m_secbCancel.Lock();

	if (bSave == true) {
		m_bCancel = bNewValue;
		ret = true;
	} else {
		ret = m_bCancel;
	}

	m_secbCancel.Unlock();

	return	ret;
}

/*!
 @brief サービスステータスの取得
*/
DWORD CDnpService::GetServiceStatus()
{
	DWORD dwStatus = (DWORD) -1;
	if(m_hService == NULL) {
		return dwStatus;
	}

	m_secServiceProcess.Lock();
	SERVICE_STATUS	sStatus;
	::ZeroMemory(&sStatus,sizeof(SERVICE_STATUS));
	BOOL bRet = ::QueryServiceStatus(m_hService,&sStatus);
	ATLASSERT(bRet);
	if(bRet == TRUE) {
		dwStatus = sStatus.dwCurrentState;
		m_dwWaitHint = sStatus.dwWaitHint;
	}
	m_secServiceProcess.Unlock();

	return dwStatus;
}

/*!
 @brief サービスが動作中か判断
*/
bool CDnpService::IsRunning()
{
	return GetServiceStatus() == SERVICE_RUNNING;
}

/*!
 @brief サービスが停止か判断
*/
bool CDnpService::IsStopped()
{
	return GetServiceStatus() == SERVICE_STOPPED;
}

/*!
 @brief サービスが一時停止か判断
*/
bool CDnpService::IsPaused()
{
	return GetServiceStatus() == SERVICE_PAUSED;
}

/*!
 @brief サービスのステータスが変更中か判断
*/
bool CDnpService::IsStatusChanging()
{
	DWORD dwStatus = GetServiceStatus();
	return dwStatus == SERVICE_STOP_PENDING || dwStatus == SERVICE_START_PENDING || dwStatus == SERVICE_PAUSE_PENDING || dwStatus == SERVICE_CONTINUE_PENDING;
}


/*!
 @struct SStatusChangeThreadParam_
 @brief  サービスステータス変更スレッド引数構造体
*/
typedef struct SStatusChangeThreadParam_ {
	CDnpService *pService;			/// 対象サービス
	EChangeServiceStatus eStatus;	/// 変更ステータス
} SStatusChangeThreadParam;


/*!
 @brief サービスのステータス変更

 @param [in]    eStatus      変更するステータス
 */
bool CDnpService::ChangeStatus(EChangeServiceStatus eStatus)
{
	if(m_hService == NULL) {
		return false;
	}

	if (m_bChaningProcess == true) {
		return false;
	}

	SStatusChangeThreadParam *pParam = new SStatusChangeThreadParam();
	pParam->pService = this;
	pParam->eStatus = eStatus;

	m_bChaningProcess = true;
	AfxBeginThread(&CDnpService::ChangeStatusThread, pParam);

	return true;
}

/*
/*!
 @brief サービスのステータス変更 (スレッド版)

 @param [in]    eStatus      変更するステータス
 */
UINT CDnpService::ChangeStatusThread(LPVOID pParam)
{
	SStatusChangeThreadParam *pChangeStateParam = ((SStatusChangeThreadParam *) pParam);
	CDnpService *pService = pChangeStateParam->pService;
	switch(pChangeStateParam->eStatus) {
		case Start:
			pService->StartDnpService();
			break;
		case Stop:
			pService->StopDnpService();
			break;

		case Restart:
			pService->StopDnpService();
			pService->StartDnpService();
			break;
	}

	pService->m_bChaningProcess = false;
	delete pChangeStateParam;

	return 0;
}

/*!
 @brief サービスの開始処理
 */
bool CDnpService::StartDnpService()
{
	if (IsRunning() == true) {
		return true;
	}

	if (IsStopped() == false) {
		return false;
	}

	m_secServiceProcess.Lock();
	//サービス開始要求
	BOOL bRet = ::StartService(m_hService,NULL,NULL);
	if(bRet == FALSE) {
		return false;
	}

	bool bChanged = true;
	//開始まで無限ループで待機
	//IsCancelを利用すれば無限ループからの脱出が可能
	while(IsStatusChanging() == true)
	{
		if(IsStateChangeCancel()) {
			bChanged = false;
			break;
		}

		//::Sleep(sStatus.dwWaitHint);
		//本来ならばdwWaitHitだけSleepをかけるが、中断処理を入れるため
		//500msecを上限にSleepする
		::Sleep((m_dwWaitHint > 500) ? 500 : m_dwWaitHint);
		continue;
	}

	bChanged = IsRunning();
	m_secServiceProcess.Unlock();

	return bChanged;
}

/*!
 @brief サービスの停止処理
 */
bool CDnpService::StopDnpService()
{
	if (IsStopped() == true) {
		return true;
	}

	if (IsRunning() == false) {
		return false;
	}

	m_secServiceProcess.Lock();
	SERVICE_STATUS	sStatus;
	::ZeroMemory(&sStatus,sizeof(SERVICE_STATUS));
	//サービス開始要求
	BOOL bRet = ::ControlService(m_hService,SERVICE_CONTROL_STOP,&sStatus);
	if(bRet == FALSE) {
		return false;
	}

	bool bChanged = true;
	//開始まで無限ループで待機
	//IsCancelを利用すれば無限ループからの脱出が可能
	while(IsStatusChanging() == true)
	{
		if(IsStateChangeCancel()) {
			bChanged = false;
			break;
		}

		//::Sleep(sStatus.dwWaitHint);
		//本来ならばdwWaitHitだけSleepをかけるが、中断処理を入れるため
		//500msecを上限にSleepする
		::Sleep((m_dwWaitHint > 500) ? 500 : m_dwWaitHint);
		continue;
	}

	bChanged = IsStopped();
	m_secServiceProcess.Unlock();

	return bChanged;
}

/*!
 @brief サービス一覧取得

 @param [out]    aryServiceInfoList     サービス一覧格納領域
 */
bool CDnpService::GetServiceList(CArray<SServiceInfo> &aryServiceInfoList)
{
	SC_HANDLE hManager = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	ATLASSERT(hManager);
	if(hManager == NULL) {
		return false;
	}

	DWORD dwBytesNeeded = 0;
	DWORD dwServices = 0;
	DWORD dwResumeHandle = 0;

	// 必要なメモリーサイズを取得
	EnumServicesStatus(hManager, SERVICE_WIN32, SERVICE_STATE_ALL
		, NULL, 0, &dwBytesNeeded, &dwServices, &dwResumeHandle);

	// メモリーの確保
	byte *pServiceStatus = new byte[dwBytesNeeded];

	if (EnumServicesStatus(hManager, SERVICE_WIN32, SERVICE_STATE_ALL
		, (LPENUM_SERVICE_STATUS)pServiceStatus, dwBytesNeeded, &dwBytesNeeded, &dwServices, &dwResumeHandle) == FALSE) {
		delete[] pServiceStatus;
		pServiceStatus = NULL;
		return false;
	}

	for (DWORD nIndex = 0; nIndex < dwServices; nIndex++) {
		SServiceInfo stServiceInfo;

		LPENUM_SERVICE_STATUS lpStatus = ((LPENUM_SERVICE_STATUS) pServiceStatus) + nIndex;
		stServiceInfo.strDispName = lpStatus->lpDisplayName;
		stServiceInfo.strServiceName = lpStatus->lpServiceName;

		aryServiceInfoList.Add(stServiceInfo);
	}

	if(hManager != NULL) {
		::CloseServiceHandle(hManager);
		hManager = NULL;
	}

	delete[] pServiceStatus;
	pServiceStatus = NULL;

	return true;
}

/*!
 @brief サービス情報取得

 @param [out]    stServiceInfo     サービス情報格納領域
 */
bool CDnpService::GetServiceInfo(SServiceInfo &stServiceInfo)
{
	if(m_hService == NULL) {
		return false;
	}
	
	//バッファサイズを取得してメモリ確保
	DWORD dwByteNeeded;
	if (m_stServiceInfo.strPath.IsEmpty() == true) {
		::QueryServiceConfig(m_hService, NULL, 0, &dwByteNeeded);
		byte *pQueryServiceConfig = new byte[dwByteNeeded];
		//PSvcConfigに情報を取得してサービスのフルパス取得
		if (QueryServiceConfig(m_hService, (LPQUERY_SERVICE_CONFIG) pQueryServiceConfig, dwByteNeeded, &dwByteNeeded)) {
			m_stServiceInfo.strPath = ((LPQUERY_SERVICE_CONFIG)pQueryServiceConfig)->lpBinaryPathName;
		}

		delete[] pQueryServiceConfig;
		pQueryServiceConfig = NULL;
	}

	if (m_stServiceInfo.strDescript.IsEmpty() == true) {
		::QueryServiceConfig2(m_hService, SERVICE_CONFIG_DESCRIPTION, NULL, 0, &dwByteNeeded);
		byte *pQueryServiceConfig = new byte[dwByteNeeded];
		//PSvcConfigに情報を取得してサービスのフルパス取得
		if (QueryServiceConfig2(m_hService, SERVICE_CONFIG_DESCRIPTION, pQueryServiceConfig, dwByteNeeded, &dwByteNeeded)) {
			m_stServiceInfo.strDescript = ((LPSERVICE_DESCRIPTION)pQueryServiceConfig)->lpDescription;
		}

		delete[] pQueryServiceConfig;
		pQueryServiceConfig = NULL;
	}

	DWORD dwStatus = GetServiceStatus();
	switch (dwStatus) {
		case SERVICE_STOPPED: m_stServiceInfo.strStatus = CString((LPCTSTR) IDS_SERVICE_STATUS_SERVICE_STOPPED); break;
		case SERVICE_START_PENDING: m_stServiceInfo.strStatus = CString((LPCTSTR) IDS_SERVICE_STATUS_SERVICE_START_PENDING); break;
		case SERVICE_STOP_PENDING: m_stServiceInfo.strStatus = CString((LPCTSTR) IDS_SERVICE_STATUS_SERVICE_STOP_PENDING); break;
		case SERVICE_RUNNING: m_stServiceInfo.strStatus = CString((LPCTSTR) IDS_SERVICE_STATUS_SERVICE_RUNNING); break;
		case SERVICE_CONTINUE_PENDING: m_stServiceInfo.strStatus =CString((LPCTSTR) IDS_SERVICE_STATUS_SERVICE_CONTINUE_PENDING); break;
		case SERVICE_PAUSE_PENDING: m_stServiceInfo.strStatus = CString((LPCTSTR) IDS_SERVICE_STATUS_SERVICE_PAUSE_PENDING); break;
		case SERVICE_PAUSED: m_stServiceInfo.strStatus = CString((LPCTSTR) IDS_SERVICE_STATUS_SERVICE_PAUSED); break;
	}

	stServiceInfo = m_stServiceInfo;

	return true;
}