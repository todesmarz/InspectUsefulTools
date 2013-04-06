/*! @file  MemLeakUserObjDlg.cpp
    @brief メモリーリーク(User オブジェクト)画面クラス
*/
#include "StdAfx.h"
#include "MemLeakUserObjDlg.h"

#include <math.h>

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CMemLeakUserObjDlg::CMemLeakUserObjDlg(CWnd* pParent /*=NULL*/)
	: CIUCommonMemoryLeakDlg(pParent)
{

}

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CMemLeakUserObjDlg::OnInitDialog()
{
	CIUCommonMemoryLeakDlg::OnInitDialog();

	SetAllocLabel(CString((LPCTSTR) IDS_MEM_LEAK_USER_ALLOC_TITLE));
	SetReleaseLabel(CString((LPCTSTR) IDS_MEM_LEAK_USER_RELEACE_TITLE));

	return TRUE;
}

/*!
 @brief 領域を確保します。

 @param [in]    nSize     サイズ
 @param [in]    nUnit     単位

*/
long long CMemLeakUserObjDlg::AllocMemory(long long nSize, int nUnit)
{
	// 単位分計算
	long long nReqAllocSize = nSize * (long long) max(pow(1024.0, (double) nUnit), 1);
	long long nResultAllocSize = 0;

	CRect rect;
	try {
		for (int nIndex = 0; nIndex < nReqAllocSize; nIndex++) {
			CEdit *pWnd = new CEdit();
			pWnd->Create(0, rect, this, (UINT) IDC_STATIC);

			m_aryMemLeak.Add(pWnd);
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
void CMemLeakUserObjDlg::ReleaseMemory()
{
	for (int nAryIndex = 0; nAryIndex < m_aryMemLeak.GetCount(); nAryIndex++) {
		::DestroyWindow(((CWnd *) m_aryMemLeak.GetAt(nAryIndex))->GetSafeHwnd());
	}

	m_aryMemLeak.RemoveAll();
}