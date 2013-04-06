/*! @file  IUCommonMemoryLeakDlg.h
    @brief 共通メモリーリーク画面クラスヘッダ
*/
#pragma once

/*!
 @class CIUCommonMemoryLeakDlg
 @brief 共通メモリーリーク画面クラス
*/
class CIUCommonMemoryLeakDlg : public CDialog
{
	DECLARE_DYNAMIC(CIUCommonMemoryLeakDlg)

private:
	long long m_nCurrentAllocMemory;		/// 現在確保済みのサイズ
	BOOL m_bSetUnitCombo;					/// 単位設定可能フラグ

protected:
	CPtrArray m_aryMemLeak;					/// 確保した領域の管理リスト

public:
	CIUCommonMemoryLeakDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

	void SetAllocLabel(LPCTSTR pszTitle);
	void SetReleaseLabel(LPCTSTR pszTitle);
	void SetUnitLabel(CStringArray &aryUnitString);

// ダイアログ データ
	enum { IDD = IDD_IU_COM_MEM_LEAK_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedIuComMemLeakButton();
	afx_msg void OnBnClickedIuComMemLeakReleaseButton();

	virtual long long AllocMemory(long long nSize, int nUnit) = 0;
	virtual void ReleaseMemory() = 0;

	DECLARE_MESSAGE_MAP()
};
