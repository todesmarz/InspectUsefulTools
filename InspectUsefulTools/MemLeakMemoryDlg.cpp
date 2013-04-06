/*! @file  MemLeakMemoryDlg.cpp
    @brief メモリーリーク(メモリー)画面クラス
*/
#include "StdAfx.h"
#include "MemLeakMemoryDlg.h"

#include <math.h>

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CMemLeakMemoryDlg::CMemLeakMemoryDlg(CWnd* pParent /*=NULL*/)
	: CIUCommonMemoryLeakDlg(pParent)
{

}

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CMemLeakMemoryDlg::OnInitDialog()
{
	CIUCommonMemoryLeakDlg::OnInitDialog();

	SetAllocLabel(CString((LPCTSTR) IDS_MEM_LEAK_HEAP_ALLOC_TITLE));
	SetReleaseLabel(CString((LPCTSTR) IDS_MEM_LEAK_HEAP_RELEACE_TITLE));

	CStringArray aryUnitString;
	aryUnitString.Add(_T("Byte"));
	aryUnitString.Add(_T("KB"));
	aryUnitString.Add(_T("MB"));
	aryUnitString.Add(_T("GB"));
	SetUnitLabel(aryUnitString);

	return TRUE;
}

/*!
 @brief 領域を確保します。

 @param [in]    nSize     サイズ
 @param [in]    nUnit     単位

*/
long long CMemLeakMemoryDlg::AllocMemory(long long nSize, int nUnit)
{
	// 単位分計算
	long long nReqAllocSize = nSize * (long long) max(pow(1024.0, (double) nUnit), 1);
	long long nResultAllocSize = 0;

	try {
		while (0 < nReqAllocSize) {
			const int nAllocSize = (int) min(nReqAllocSize, 4096);
			short *pMemLeak = new short[nAllocSize];

			m_aryMemLeak.Add(pMemLeak);
			nReqAllocSize -= nAllocSize;
			nResultAllocSize += nAllocSize;
		}
	}
	catch(CException *pE)
	{
		TCHAR* pszErrorMsg = new TCHAR[1024];
		pE->GetErrorMessage(pszErrorMsg, 1024);

		MEMORYSTATUS stat;
		::GlobalMemoryStatus (&stat);

		CString strErrorMessage;
		strErrorMessage.Format(IDS_MEM_LEAK_HEAP_EXCEPTION_FORMAT, 
			pszErrorMsg, 
			stat.dwAvailPhys,
			stat.dwAvailVirtual,
			stat.dwAvailPageFile);

		AfxMessageBox(strErrorMessage, MB_TASKMODAL);
		pE->Delete();
	}

	return nResultAllocSize;
}

/*!
 @brief 確保した領域を解放します。
*/
void CMemLeakMemoryDlg::ReleaseMemory()
{
	for (int nAryIndex = 0; nAryIndex < m_aryMemLeak.GetCount(); nAryIndex++) {
		delete[] m_aryMemLeak.GetAt(nAryIndex);
	}

	m_aryMemLeak.RemoveAll();
}