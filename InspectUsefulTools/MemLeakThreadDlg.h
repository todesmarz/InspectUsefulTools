/*! @file  MemLeakThreadDlg.h
    @brief �������[���[�N(�X���b�h)��ʃN���X�w�b�_
*/
#pragma once
#include "resource.h"
#include "IUCommonMemoryLeakDlg.h"

/*!
 @class CMemLeakThreadDlg
 @brief �������[���[�N(�X���b�h)��ʃN���X
*/
class CMemLeakThreadDlg :
	public CIUCommonMemoryLeakDlg
{
public:
	CMemLeakThreadDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^

protected:
	virtual BOOL OnInitDialog();

	virtual long long AllocMemory(long long nSize, int nUnit);
	virtual void ReleaseMemory();

private:
	static UINT LeakThread(LPVOID pParam);

};
