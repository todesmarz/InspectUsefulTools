/*! @file  SysProcessSpeedDlg.cpp
    @brief 処理時間計測画面クラス
*/
#include "StdAfx.h"
#include "SysProcessSpeedDlg.h"

#include <time.h>

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CSysProcessSpeedDlg::CSysProcessSpeedDlg(CWnd* pParent /*=NULL*/)
	: CIUCommonNumExecDlg(pParent)
{

}


/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CSysProcessSpeedDlg::OnInitDialog()
{
	CIUCommonNumExecDlg::OnInitDialog();

	CSpinButtonCtrl *pSpinCtrl = (CSpinButtonCtrl *) GetDlgItem(IDD_IU_COM_NUM_EXEC_TIMES_SPIN);

	pSpinCtrl->SetRange32(0, 1000 * 1000 * 1000);
	pSpinCtrl->SetPos32(1000 * 1000 * 10);


	return TRUE;
}

/*!
 @struct SProcessSpeedThreadParam_
 @brief  サービス情報構造体
*/
typedef struct SProcessSpeedThreadParam_ {
	int nTimes;			/// 実行回数
	CString strResult;	/// 処理結果
	HANDLE hThread;		/// スレッドハンドル
} SProcessSpeedThreadParam;


/*!
 @brief 情報取得\n
 指定コマンドを実行して情報を取得します。

 @return 情報
*/
CString CSysProcessSpeedDlg::GetInformation(BOOL bInit)
{
	if (bInit == TRUE) {
		return _T("");
	}

	int nProcessTime;
	CSpinButtonCtrl *pSpinCtrl = (CSpinButtonCtrl *) GetDlgItem(IDD_IU_COM_NUM_EXEC_TIMES_SPIN);
	nProcessTime = pSpinCtrl->GetPos32();

	SYSTEM_INFO info;
	GetSystemInfo(&info);

	// スレッドの起動
	CPtrArray aryParam;
	for (unsigned int nCoreIndex = 0; nCoreIndex < info.dwNumberOfProcessors; nCoreIndex++) {
		SProcessSpeedThreadParam *pParam = new SProcessSpeedThreadParam();
		pParam->nTimes = nProcessTime;
		CWinThread *pThread = AfxBeginThread(&CSysProcessSpeedDlg::ProcessSpeedThread, (LPVOID) pParam);
		pParam->hThread = pThread->m_hThread;
		aryParam.Add(pParam);
	}
	
	HANDLE *pThreadList = new HANDLE[aryParam.GetCount()];
	for (int nAryIndex = 0; nAryIndex < aryParam.GetCount(); nAryIndex++) {
		if (aryParam[nAryIndex] != NULL) {
			pThreadList[nAryIndex] = ((SProcessSpeedThreadParam *)aryParam[nAryIndex])->hThread;
		}
	}

	// 待ち
	::WaitForMultipleObjects(aryParam.GetCount(), pThreadList, TRUE, INFINITE);

	// 解放処理
	CString strResult;
	for (int nAryIndex = 0; nAryIndex < aryParam.GetCount(); nAryIndex++) {
		if (aryParam[nAryIndex] != NULL) {
			CString strCpu;
			strCpu.Format(_T("------------------------- CPU%d -------------------------\r\n"), nAryIndex + 1);
			strResult += strCpu;
			strResult += ((SProcessSpeedThreadParam *)aryParam[nAryIndex])->strResult;
			delete aryParam[nAryIndex];
			aryParam[nAryIndex] = NULL;
		}
	}
	aryParam.RemoveAll();


	return strResult;
}

/*!
 @brief プロセス演算速度計測

 @return 情報
*/
UINT CSysProcessSpeedDlg::ProcessSpeedThread(LPVOID pParam)
{
	SProcessSpeedThreadParam *pProcessParam = (SProcessSpeedThreadParam *) pParam;

	try {
		int nProcessTime = pProcessParam->nTimes;

		LARGE_INTEGER   startTime, endTime, freq;
		double nAddTime;
		double nSubTime;
		double nMulTime;
		double nDivTime;
		double nModTime;
		double nShiftTime;
		double nLogicTime;
		double nIfAddTime;
		double nIfSubTime;
		double nIfMulTime;
		double nIfDivTime;
		double nStringTime;
		double nStringFormatTime;
		long n = 0;
		CString strTemp = _T("TEMP");
		QueryPerformanceFrequency(&freq);

		// 加算演算時間計測
		QueryPerformanceCounter(&startTime);
		for (int i = 0; i < nProcessTime; i++) {
			n++;
		}
		QueryPerformanceCounter(&endTime);
		nAddTime = (double) (endTime.QuadPart - startTime.QuadPart) / (double)freq.QuadPart * 1000;

		// 減算演算時間計測
		QueryPerformanceCounter(&startTime);
		for (int i = 0; i < nProcessTime; i++) {
			n--;
		}
		QueryPerformanceCounter(&endTime);
		nSubTime = (double) (endTime.QuadPart - startTime.QuadPart) / (double)freq.QuadPart * 1000;

		// 乗算演算時間計測
		n = 1;
		QueryPerformanceCounter(&startTime);
		for (int i = 0; i < nProcessTime; i++) {
			n *= i;
		}
		QueryPerformanceCounter(&endTime);
		nMulTime = (double) (endTime.QuadPart - startTime.QuadPart) / (double)freq.QuadPart * 1000;

		// 除算演算時間計測
		QueryPerformanceCounter(&startTime);
		for (int i = 0; i < nProcessTime; i++) {
			n /= (i + 1);
		}
		QueryPerformanceCounter(&endTime);
		nDivTime = (double) (endTime.QuadPart - startTime.QuadPart) / (double)freq.QuadPart * 1000;

		// 余算演算時間計測
		QueryPerformanceCounter(&startTime);
		for (int i = 0; i < nProcessTime; i++) {
			n %= (i + 1);
		}
		QueryPerformanceCounter(&endTime);
		nModTime = (double) (endTime.QuadPart - startTime.QuadPart) / (double)freq.QuadPart * 1000;
		// シフト演算時間計測
		QueryPerformanceCounter(&startTime);
		for (int i = 0; i < nProcessTime; i++) {
			n >>= (i + 1);
		}
		QueryPerformanceCounter(&endTime);
		nShiftTime = (double) (endTime.QuadPart - startTime.QuadPart) / (double)freq.QuadPart * 1000;

		// 論理演算時間計測
		QueryPerformanceCounter(&startTime);
		for (int i = 0; i < nProcessTime; i++) {
			n ^= (i + 1);
		}
		QueryPerformanceCounter(&endTime);
		nLogicTime = (double) (endTime.QuadPart - startTime.QuadPart) / (double)freq.QuadPart * 1000;

		// 分岐 加算時間計測
		QueryPerformanceCounter(&startTime);
		for (int i = 0; i < nProcessTime; i++) {
			n += ((n & 1) == 0) ? 0 : 1;
		}
		QueryPerformanceCounter(&endTime);
		nIfAddTime = (double) (endTime.QuadPart - startTime.QuadPart) / (double)freq.QuadPart * 1000;

		// 分岐 減算時間計測
		QueryPerformanceCounter(&startTime);
		for (int i = 0; i < nProcessTime; i++) {
			n -= ((n & 1) == 0) ? 0 : 1;
		}
		QueryPerformanceCounter(&endTime);
		nIfSubTime = (double) (endTime.QuadPart - startTime.QuadPart) / (double)freq.QuadPart * 1000;

		// 分岐 乗算時間計測
		QueryPerformanceCounter(&startTime);
		for (int i = 0; i < nProcessTime; i++) {
			n *= ((n & 1) == 0) ? 1 : i;
		}
		QueryPerformanceCounter(&endTime);
		nIfMulTime = (double) (endTime.QuadPart - startTime.QuadPart) / (double)freq.QuadPart * 1000;

		// 分岐 除算時間計測
		QueryPerformanceCounter(&startTime);
		for (int i = 0; i < nProcessTime; i++) {
			n /= ((n & 1) == 0) ? 1 : (i + 1);
		}
		QueryPerformanceCounter(&endTime);
		nIfDivTime = (double) (endTime.QuadPart - startTime.QuadPart) / (double)freq.QuadPart * 1000;

		strTemp.Empty();
		strTemp.GetBuffer(4 * nProcessTime);
		// 文字列 連結時間計測
		QueryPerformanceCounter(&startTime);
		for (int i = 0; i < nProcessTime; i++) {
			strTemp.Append(_T("TEMP"));
		}
		QueryPerformanceCounter(&endTime);
		nStringTime = (double) (endTime.QuadPart - startTime.QuadPart) / (double)freq.QuadPart * 1000;
		strTemp.ReleaseBuffer();

		strTemp.Empty();
		// 文字列　Format 時間計測
		QueryPerformanceCounter(&startTime);
		for (int i = 0; i < nProcessTime; i++) {
			strTemp.Format(_T("%d"), i);
		}
		QueryPerformanceCounter(&endTime);
		nStringFormatTime = (double) (endTime.QuadPart - startTime.QuadPart) / (double)freq.QuadPart * 1000;

		CString resutMessage;
		resutMessage.Format(IDS_SYS_PROCESS_SPPED_RESUT_FORMAT
			, nAddTime, nSubTime, nMulTime, nDivTime, nModTime, nShiftTime, nLogicTime
			, nIfAddTime, nIfSubTime, nIfMulTime, nIfDivTime
			, nStringTime, nStringFormatTime);

		pProcessParam->strResult = resutMessage;
	} catch (...) {
		// 例外は無視
	}

	return 0;
}