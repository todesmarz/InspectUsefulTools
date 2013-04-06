/*! @file  SysProcessSpeedDlg.h
    @brief 処理時間計測画面クラスヘッダ
*/
#pragma once
#include "resource.h"
#include "IUCommonNumExecDlg.h"

/*!
 @class CSysProcessSpeedDlg
 @brief 処理時間計測画面クラス
*/
class CSysProcessSpeedDlg : public CIUCommonNumExecDlg
{
public:
	CSysProcessSpeedDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

	virtual CString GetInformation(BOOL bInit);

protected:
	virtual BOOL OnInitDialog();

private:
	static UINT ProcessSpeedThread(LPVOID pParam);
};
