/*! @file  MemLeakMemoryDlg.cpp
    @brief �������[���[�N(GDI �I�u�W�F�N�g)��ʃN���X
*/
#include "StdAfx.h"
#include "MemLeakThreadDlg.h"

#include <math.h>

/*!
 @brief �R���X�g���N�^

 @param [in]    pParent     �e�E�B���h�E�C���X�^���X
*/
CMemLeakThreadDlg::CMemLeakThreadDlg(CWnd* pParent /*=NULL*/)
	: CIUCommonMemoryLeakDlg(pParent)
{

}

/*!
 @brief ���̃��\�b�h�� WM_INITDIALOG �̃��b�Z�[�W�ɉ������ČĂяo����܂��B
*/
BOOL CMemLeakThreadDlg::OnInitDialog()
{
	CIUCommonMemoryLeakDlg::OnInitDialog();

	SetAllocLabel(CString((LPCTSTR) IDS_MEM_LEAK_THREAD_ALLOC_TITLE));
	SetReleaseLabel(CString((LPCTSTR) IDS_MEM_LEAK_THREAD_RELEACE_TITLE));

	return TRUE;
}

/*!
 @brief �̈���m�ۂ��܂��B

 @param [in]    nSize     �T�C�Y
 @param [in]    nUnit     �P��

*/
long long CMemLeakThreadDlg::AllocMemory(long long nSize, int nUnit)
{
	// �P�ʕ��v�Z
	long long nReqAllocSize = nSize * (long long) max(pow(1024.0, (double) nUnit), 1);
	long long nResultAllocSize = 0;

	CRect rect;
	try {
		for (int nIndex = 0; nIndex < nReqAllocSize; nIndex++) {
			CWinThread *pThread = AfxBeginThread(&CMemLeakThreadDlg::LeakThread, NULL);

			m_aryMemLeak.Add(pThread);
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
 @brief �m�ۂ����̈��������܂��B
*/
void CMemLeakThreadDlg::ReleaseMemory()
{
	for (int nAryIndex = 0; nAryIndex < m_aryMemLeak.GetCount(); nAryIndex++) {
		((CWinThread *) m_aryMemLeak.GetAt(nAryIndex))->SuspendThread();
	}

	m_aryMemLeak.RemoveAll();
}

/*!
 @brief ���[�N�p�X���b�h
*/
UINT CMemLeakThreadDlg::LeakThread(LPVOID)
{
	// �����ɑ҂�������
	while (true) {
		Sleep(INFINITE);
	}
}
