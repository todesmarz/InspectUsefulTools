/*! @file  MemLockFileMappingDlg.h
    @brief ロックファイル画面クラス ヘッダ
*/
#pragma once
#include "resource.h"
#include "IUCommonLockDlg.h"

/*!
 @class CMemLockFileMappingDlg
 @brief ロックファイル画面クラス
*/
class CMemLockFileMappingDlg : public CIUCommonLockDlg
{

public:
	CMemLockFileMappingDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

protected:
	virtual BOOL LockObject(CString &strLockName, HANDLE &hLockHandle);
	virtual BOOL UnlockObject(CString &strLockName, HANDLE hLockHandle);

};
