/*! @file  MemLockMutexDlg.h
    @brief Mutexロック画面クラス ヘッダ
*/
#pragma once
#include "resource.h"
#include "IUCommonLockDlg.h"

/*!
 @class CMemLockMutexDlg
 @brief Mutexロック画面クラス
*/
class CMemLockMutexDlg : public CIUCommonLockDlg
{

public:
	CMemLockMutexDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

protected:
	virtual BOOL LockObject(CString &strLockName, HANDLE &hLockHandle);
	virtual BOOL UnlockObject(CString &strLockName, HANDLE hLockHandle);
};
