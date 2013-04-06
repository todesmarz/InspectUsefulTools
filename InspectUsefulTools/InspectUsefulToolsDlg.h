/*! @file  InspectUsefulToolsDlg.h
    @brief 当ツールメイン画面クラスヘッダ
*/
#pragma once
#include "afxcmn.h"
#include "IUCommonInfoDlg.h"
#include "IUCommonMemoryLeakDlg.h"

#include "UserInfoDlg.h"
#include "SysServiceInfoDlg.h"
#include "FileLockDlg.h"
#include "FileTimeStampDlg.h"
#include "FileAttributeDlg.h"
#include "FileTailDlg.h"
#include "FileReplaceDlg.h"
#include "DummyFileSizeDlg.h"
#include "MemLeakMemoryDlg.h"
#include "MemLeakGdiObjDlg.h"
#include "MemLeakUserObjDlg.h"
#include "MemLeakThreadDlg.h"
#include "MemLockMutexDlg.h"
#include "MemLockFileMappingDlg.h"
#include "TimeChangeTimeDlg.h"
#include "TimeStopWatchDlg.h"
#include "MemoDlg.h"

#include "CApplication.h"
#include "CWorkbooks.h"
#include "CWorkbook.h"
#include "CWindows.h"


/*!
 @class CInspectUsefulToolsDlg
 @brief 当ツールメイン画面クラス
*/
class CInspectUsefulToolsDlg : public CDialog
{
private:
	CTreeCtrl m_inMenuTreeCtrl;
	CTabCtrl m_inContentsTabCtrl;

	CApplication m_inXLApp;								/// Excelインスタンス
	CWorkbook m_inXLBook;								/// Excel Macro Book インスタンス
	BOOL m_bOpenXL;										/// Excel起動フラグ
	BOOL m_bOpenBook;									/// Excel Macro Book 起動フラグ
	CString strTempFilePath;							/// マクロブックファイルパス


	CDialog m_inTopDlg;									/// トップ画面

	CIUCommonInfoDlg m_inSystemInfoDlg;					/// システム情報画面
	CIUCommonInfoDlg m_inSystemEnvInfoDlg;				/// 環境変数画面
	CIUCommonInfoDlg m_inSystemNetworkInfoDlg;			/// ネットワーク情報画面
	CIUCommonInfoDlg m_inSystemDiskInfoDlg;				/// ディスク情報画面
	CIUCommonInfoDlg m_inSystemEventLogDlg;				/// イベントログ画面
	CUserInfoDlg m_inUserInfoDlg;						/// ユーザ情報画面
	CIUCommonInfoDlg m_inSystemUserListDlg;				/// ユーザ一覧画面
	CSysServiceInfoDlg m_inSysServiceInfoDlg;			/// サービス操作画面
	CIUCommonInfoDlg m_inProcessSpeedDlg;				/// 演算速度計測画面
	CIUCommonInfoDlg m_inErrorDetailDlg;				/// 演算速度計測画面

	CFileLockDlg m_inFileLockDlg;						/// ファイルロック画面
	CFileTimeStampDlg m_inFileTimeStampDlg;				/// ファイルタイムスタンプ変更画面
	CFileAttributeDlg m_inFileAttributeDlg;				/// ファイル属性変更画面
	CIUCommonInfoDlg m_inFileTailDlg;					/// ファイル監視画面
	CFileReplaceDlg m_inFileReplaceDlg;					/// ファイル置換画面
	CIUCommonInfoDlg m_inFileCommonOpenInfoDlg;			/// 共有ファイル操作情報画面

	CDummyFileSizeDlg m_inDummyFileSizeDlg;				/// 指定ファイルサイズファイル作成画面

	CMemLeakMemoryDlg m_inMemLeakDlg;					/// メモリーリーク発生画面
	CMemLeakGdiObjDlg m_inMemLeakGdiDlg;				/// メモリーリーク発生画面
	CMemLeakUserObjDlg m_inMemLeakUserDlg;				/// メモリーリーク発生画面
	CMemLeakThreadDlg m_inMemLeakThreadDlg;				/// メモリーリーク発生画面
	CMemLockMutexDlg m_inMemLockMutexDlg;				/// Mutexロック画面
	CMemLockFileMappingDlg m_inMemLockFileMappingDlg;	/// メモリーマップドファイルロック画面

	CTimeChangeTimeDlg m_inTimeChangeTimeDlg;			/// 時刻変更画面
	CTimeStopWatchDlg m_inTimeStopWatchDlg;				/// ストップウォッチ画面

	CIUCommonInfoDlg m_inMd5CryptDlg;					/// MD5取得画面
	CIUCommonInfoDlg m_inSha1CryptDlg;					/// SHA-1取得画面
	CIUCommonInfoDlg m_inSha256CryptDlg;				/// SHA-256取得画面

	CIUCommonInfoDlg m_inNetworkCmdPingDlg;				/// Ping画面
	CIUCommonInfoDlg m_inNetworkCmdNslookupDlg;			/// nslookup画面
	CIUCommonInfoDlg m_inNetworkCmdTraceDlg;			/// tracert画面
	CIUCommonInfoDlg m_inNetworkCmdNbtStatDlg;			/// nbtstat画面
	CIUCommonInfoDlg m_inNetworkCmdNetStatDlg;			/// netstat画面
	CIUCommonInfoDlg m_inNetworkCmdArpDlg;				/// arp画面
	CIUCommonInfoDlg m_inNetworkCmdRouteDlg;			/// route画面

	CMemoDlg m_inMemoDlg;

	CMapStringToPtr m_mapInnerDlg;

// 実装
protected:
	HICON m_hIcon;										/// ウィンドウアイコン

// コンストラクション
public:
	CInspectUsefulToolsDlg(CWnd* pParent = NULL);	// 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_INSPECTUSEFULTOOLS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

// 実装
protected:
	// 生成された、メッセージ割り当て関数
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTvnSelchangedMenuTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

private:
	BOOL InitializeMenuTree();
	BOOL InitializeContents();
	BOOL HideContents();

	BOOL ShowContents(CDialog *pDialog);
};
