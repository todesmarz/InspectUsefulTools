// MemLockMutexDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "MemLockFileMappingDlg.h"

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CMemLockFileMappingDlg::CMemLockFileMappingDlg(CWnd* pParent /*=NULL*/)
	: CIUCommonLockDlg(pParent)
{

}

/*!
 @brief ロック処理

 @param [in]    strLockName     ロック名称
 @param [out]   hLockHandle     ロックハンドル
*/
BOOL CMemLockFileMappingDlg::LockObject(CString &strLockName, HANDLE &hLockHandle)
{
	hLockHandle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 1024, strLockName);

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
BOOL CMemLockFileMappingDlg::UnlockObject(CString &, HANDLE hLockHandle)
{
	if (hLockHandle == NULL) {
		return TRUE;
	}

    // 所有権を解放する。
    UnmapViewOfFile(hLockHandle);
    CloseHandle(hLockHandle);

	return TRUE;
}
