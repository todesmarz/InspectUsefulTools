/*! @file  MemLeakMemoryDlg.h
    @brief メモリーリーク(メモリー)画面クラスヘッダ
*/
#pragma once
#include "resource.h"
#include "IUCommonMemoryLeakDlg.h"

/*!
 @class CMemLeakMemoryDlg
 @brief メモリーリーク(メモリー)画面クラス
*/
class CMemLeakMemoryDlg :
	public CIUCommonMemoryLeakDlg
{
public:
	CMemLeakMemoryDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

protected:
	virtual BOOL OnInitDialog();

	virtual long long AllocMemory(long long nSize, int nUnit);
	virtual void ReleaseMemory();
};
