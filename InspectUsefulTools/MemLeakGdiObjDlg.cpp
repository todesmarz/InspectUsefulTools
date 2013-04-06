/*! @file  MemLeakMemoryDlg.cpp
    @brief メモリーリーク(GDI オブジェクト)画面クラス
*/
#include "StdAfx.h"
#include "MemLeakGdiObjDlg.h"

#include <math.h>

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CMemLeakGdiObjDlg::CMemLeakGdiObjDlg(CWnd* pParent /*=NULL*/)
	: CIUCommonMemoryLeakDlg(pParent)
{

}

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CMemLeakGdiObjDlg::OnInitDialog()
{
	CIUCommonMemoryLeakDlg::OnInitDialog();

	SetAllocLabel(CString((LPCTSTR) IDS_MEM_LEAK_GDI_ALLOC_TITLE));
	SetReleaseLabel(CString((LPCTSTR) IDS_MEM_LEAK_GDI_RELEACE_TITLE));

	return TRUE;
}

/*!
 @brief 領域を確保します。

 @param [in]    nSize     サイズ
 @param [in]    nUnit     単位

*/
long long CMemLeakGdiObjDlg::AllocMemory(long long nSize, int nUnit)
{
	// 単位分計算
	long long nReqAllocSize = nSize * (long long) max(pow(1024.0, (double) nUnit), 1);
	long long nResultAllocSize = 0;

	CRect rect;
	try {
		for (int nIndex = 0; nIndex < nReqAllocSize; nIndex++) {
			CDC *pMemLeak = GetDC();

			m_aryMemLeak.Add(pMemLeak);
			nResultAllocSize++;
		}
	}
	catch(CException *pE)
	{
		TCHAR* pszErrorMsg = new TCHAR[1024];
		pE->GetErrorMessage(pszErrorMsg, 1024);

		MEMORYSTATUS stat;
		::GlobalMemoryStatus (&stat);

		CString strErrorMessage;
		strErrorMessage.Format(_T("%s"), pszErrorMsg);

		AfxMessageBox(strErrorMessage, MB_TASKMODAL);
		pE->Delete();
	}

	return nResultAllocSize;
}

/*!
 @brief 確保した領域を解放します。
*/
void CMemLeakGdiObjDlg::ReleaseMemory()
{
	for (int nAryIndex = 0; nAryIndex < m_aryMemLeak.GetCount(); nAryIndex++) {
		ReleaseDC(((CDC *) m_aryMemLeak.GetAt(nAryIndex)));
	}

	m_aryMemLeak.RemoveAll();
}