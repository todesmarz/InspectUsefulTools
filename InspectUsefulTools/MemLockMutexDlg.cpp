/*! @file  MemLockMutexDlg.cpp
    @brief 共通ロック画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "MemLockMutexDlg.h"

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CMemLockMutexDlg::CMemLockMutexDlg(CWnd* pParent /*=NULL*/)
	: CIUCommonLockDlg(pParent)
{

}

/*!
 @brief ロック処理

 @param [in]    strLockName     ロック名称
 @param [out]   hLockHandle     ロックハンドル
*/
BOOL CMemLockMutexDlg::LockObject(CString &strLockName, HANDLE &hLockHandle)
{
	hLockHandle = CreateMutex(NULL, NULL, strLockName);
	//m_mapLockInfo
    if(hLockHandle == NULL 
		|| ::GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return FALSE;
    }

	return TRUE;
}

/*!
 @brief アンロック処理

 @param [in]    strLockName     ロック名称
 @param [in]   hLockHandle     ロックハンドル
*/
BOOL CMemLockMutexDlg::UnlockObject(CString &, HANDLE hLockHandle)
{
 	if (hLockHandle == NULL) {
		return TRUE;
	}
	
	// Mutexの所有権を解放する。
    ReleaseMutex(hLockHandle);
    CloseHandle(hLockHandle);

	return TRUE;
}
