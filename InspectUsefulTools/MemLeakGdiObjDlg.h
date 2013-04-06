/*! @file  MemLeakGdiObjDlg.h
    @brief メモリーリーク(GDI オブジェクト)画面クラスヘッダ
*/
#pragma once
#include "resource.h"
#include "IUCommonMemoryLeakDlg.h"

/*!
 @class CMemLeakGdiObjDlg
 @brief メモリーリーク(GDI オブジェクト)画面クラス
*/
class CMemLeakGdiObjDlg :
	public CIUCommonMemoryLeakDlg
{
public:
	CMemLeakGdiObjDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

protected:
	virtual BOOL OnInitDialog();

	virtual long long AllocMemory(long long nSize, int nUnit);
	virtual void ReleaseMemory();
};
