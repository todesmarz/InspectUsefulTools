﻿/*! @file  FileTailDlg.h
    @brief ファイル監視画面クラスヘッダ
*/
#pragma once

#include "IUCommonFileDlg.h"

/*!
 @class CFileTailDlg
 @brief ファイル監視画面クラス
*/
class CFileTailDlg : public CIUCommonFileDlg
{
private:
	CFile *m_pFile;
	ULONGLONG m_nReadIndex;
	CString m_strFileData;
	BOOL m_bExecuteMode;

	CString m_strTailFilePath;

public:
	CFileTailDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	virtual CString GetInformation(BOOL bInit);
	virtual void ExecuteOperation();
};
