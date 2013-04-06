/*! @file  IUCommonHashCalcDlg.h
    @brief 共通ハッシュコード計算画面クラスヘッダ
*/
#pragma once

#include <WinCrypt.h>

/*!
 @class CIUCommonHashCalcDlg
 @brief 共通ハッシュコード計算画面クラス
*/
class CIUCommonHashCalcDlg : public CDialog
{
	DECLARE_DYNAMIC(CIUCommonHashCalcDlg)

private:
	HCRYPTKEY m_hCryptKey;							/// 対象ハッシュ
	int m_nHashKeyLength;							/// ハッシュ長

public:
	CIUCommonHashCalcDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

	virtual CString GetInformation(BOOL bInit);
	void SetHashSetting(HCRYPTKEY hCryptKey, int nHashKeyLength);

// ダイアログ データ
	enum { IDD = IDD_IU_COM_HASH_CALC_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedIuComHashCalcFileSelectButton();
	afx_msg void OnBnClickedIuComHashCalcButton();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnEnChangeIuComHashCalcTargetEdit();

	DECLARE_MESSAGE_MAP()

	static bool GetHashCode(const void* pData, DWORD dwLen, HCRYPTKEY hCryptKey, int nHashSize, BYTE *pcbHashData);
};
