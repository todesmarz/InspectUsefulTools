/*! @file  MemLeakUserObjDlg.h
    @brief メモリーリーク(User オブジェクト)画面クラスヘッダ
*/
#pragma once
#include "resource.h"
#include "IUCommonMemoryLeakDlg.h"

/*!
 @class CMemLeakUserObjDlg
 @brief メモリーリーク(User オブジェクト)画面クラス
*/
class CMemLeakUserObjDlg :
	public CIUCommonMemoryLeakDlg
{
public:
	CMemLeakUserObjDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

protected:
	virtual BOOL OnInitDialog();

	virtual long long AllocMemory(long long nSize, int nUnit);
	virtual void ReleaseMemory();
};
