/*! @file  SysErrorDescriptDlg.cpp
    @brief エラー詳細表示画面クラス
*/
#include "StdAfx.h"
#include "SysErrorDescriptDlg.h"

#include <limits.h>
#include <afxinet.h>
/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CSysErrorDescriptDlg::CSysErrorDescriptDlg(CWnd* pParent /*=NULL*/)
	: CIUCommonNumExecDlg(pParent)
{

}


/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CSysErrorDescriptDlg::OnInitDialog()
{
	CIUCommonNumExecDlg::OnInitDialog();

	CSpinButtonCtrl *pSpinCtrl = (CSpinButtonCtrl *) GetDlgItem(IDD_IU_COM_NUM_EXEC_TIMES_SPIN);

	pSpinCtrl->SetRange32(INT_MIN, INT_MAX);
	pSpinCtrl->SetPos32(0);


	return TRUE;
}

/*!
 @brief 情報取得\n
 指定コマンドを実行して情報を取得します。

 @return 情報
*/
CString CSysErrorDescriptDlg::GetInformation(BOOL bInit)
{
	if (bInit == TRUE) {
		return _T("");
	}

	CSpinButtonCtrl *pSpinCtrl = (CSpinButtonCtrl *) GetDlgItem(IDD_IU_COM_NUM_EXEC_TIMES_SPIN);
	int nErrorNumber = pSpinCtrl->GetPos32();

	// strerror
	CString strErrorMessage;
	strErrorMessage += _T("strerror()-----------------------\r\n");

	TCHAR szErrorMessage[1024];
	ZeroMemory(szErrorMessage, sizeof(szErrorMessage));
	_tcserror_s(szErrorMessage, sizeof(szErrorMessage) / sizeof(TCHAR), nErrorNumber);
	strErrorMessage += szErrorMessage;
	strErrorMessage += _T("\r\n");

	// FormatMessage
	strErrorMessage += _T("\r\n");
	ZeroMemory(szErrorMessage, sizeof(szErrorMessage));
	strErrorMessage += _T("FormatMessage()-----------------------\r\n");
	FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			nErrorNumber,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			(LPTSTR) &szErrorMessage,
			sizeof(szErrorMessage) / sizeof(TCHAR),
			NULL);
	strErrorMessage += szErrorMessage;

	// Exception
	strErrorMessage += _T("\r\n");
	CInternetException ex(nErrorNumber);
	strErrorMessage += _T("CException()-----------------------\r\n");
	ex.GetErrorMessage(szErrorMessage, sizeof(szErrorMessage) / sizeof(TCHAR));
	strErrorMessage += szErrorMessage;

	// ErrorTable
	strErrorMessage += _T("\r\n");
	strErrorMessage += _T("Error Table()-----------------------\r\n");
	for (int nTblIndex = 0; nTblIndex < sizeof(SErrorInfoDataTbl) / sizeof(SErrorInfoData); nTblIndex++) {
		SErrorInfoData *pData = &SErrorInfoDataTbl[nTblIndex];
		if (pData->nErrorNo != nErrorNumber
			|| pData->szDetail == NULL) {
			continue;
		}

		strErrorMessage += pData->szDetail;
	}

	strErrorMessage += _T("\r\n");

	return strErrorMessage;
}
