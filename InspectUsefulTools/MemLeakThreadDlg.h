/*! @file  MemLeakThreadDlg.h
    @brief メモリーリーク(スレッド)画面クラスヘッダ
*/
#pragma once
#include "resource.h"
#include "IUCommonMemoryLeakDlg.h"

/*!
 @class CMemLeakThreadDlg
 @brief メモリーリーク(スレッド)画面クラス
*/
class CMemLeakThreadDlg :
	public CIUCommonMemoryLeakDlg
{
public:
	CMemLeakThreadDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

protected:
	virtual BOOL OnInitDialog();

	virtual long long AllocMemory(long long nSize, int nUnit);
	virtual void ReleaseMemory();

private:
	static UINT LeakThread(LPVOID pParam);

};
