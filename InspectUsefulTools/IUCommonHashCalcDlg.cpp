/*! @file  IUCommonHashCalcDlg.cpp
    @brief 共通ハッシュコード計算画面クラス
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "IUCommonHashCalcDlg.h"

IMPLEMENT_DYNAMIC(CIUCommonHashCalcDlg, CDialog)

/*!
 @brief コンストラクタ

 @param [in]    pParent     親ウィンドウインスタンス
*/
CIUCommonHashCalcDlg::CIUCommonHashCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIUCommonHashCalcDlg::IDD, pParent)
{

}

/*!
 @brief ダイアログ データの交換と有効性チェックのためにフレームワークが呼び出します。

 @param [in]    pDX     CDataExchange オブジェクトへのポインタ
*/
void CIUCommonHashCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/*!
 @brief メッセージマップ定義
*/
BEGIN_MESSAGE_MAP(CIUCommonHashCalcDlg, CDialog)
	ON_BN_CLICKED(IDD_IU_COM_HASH_CALC_FILE_SELECT_BUTTON, &CIUCommonHashCalcDlg::OnBnClickedIuComHashCalcFileSelectButton)
	ON_BN_CLICKED(IDD_IU_COM_HASH_CALC_BUTTON, &CIUCommonHashCalcDlg::OnBnClickedIuComHashCalcButton)
	ON_WM_SIZE()
	ON_WM_DROPFILES()
	ON_EN_CHANGE(IDD_IU_COM_HASH_CALC_TARGET_EDIT, &CIUCommonHashCalcDlg::OnEnChangeIuComHashCalcTargetEdit)
END_MESSAGE_MAP()

/*!
 @brief このメソッドは WM_INITDIALOG のメッセージに応答して呼び出されます。
*/
BOOL CIUCommonHashCalcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	::SHAutoComplete(::GetDlgItem(this->m_hWnd, IDD_IU_COM_HASH_CALC_TARGET_EDIT), SHACF_FILESYSTEM);
	((CComboBox *) GetDlgItem(IDD_IU_COM_HASH_CALC_TYPE_COMBO))->SetCurSel(0);
	DragAcceptFiles();


	return TRUE;
}

/*!
 @brief Windows 関数 TranslateMessage や DispatchMessage にディスパッチされる前に、ウィンドウ メッセージをフィルタ処理するには、この関数をオーバーライドします。\n
 既定の実装では、アクセラレータ キーの変換を行います。\n
 そのため、オーバーライドした CWinApp::PreTranslateMessage メンバ関数を呼び出す必要があります。
*/
BOOL CIUCommonHashCalcDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
        return 0;
        //  必ず０で戻ること!!
        //  0以外で戻ると各コントロールへ次の処理が回らなくなる
    }
    return CDialog::PreTranslateMessage(pMsg);
}

/*!
 @brief ファイル選択ボタンをクリックすると呼び出されます。
*/
void CIUCommonHashCalcDlg::OnBnClickedIuComHashCalcFileSelectButton()
{
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST , _T("すべてのファイル (*.*)|*.*||"));

	if (fileDlg.DoModal() == IDOK) {
		GetDlgItem(IDD_IU_COM_HASH_CALC_TARGET_EDIT)->SetWindowText(fileDlg.GetFileName());
	}
}

/*!
 @brief 変換ボタンをクリックすると呼び出されます。
*/
void CIUCommonHashCalcDlg::OnBnClickedIuComHashCalcButton()
{
	// 親ダイアログにイベントを送信
	CWnd *pParent = this->GetParent();
	if (pParent != NULL) {
		pParent->SendMessage(WM_COMMAND, MAKEWPARAM(IDD_IU_COM_COMMAND_EXEC_BUTTON, BN_CLICKED));
	}
}

/*!
 @brief リサイズイベント\n
 WM_SIZEで呼び出されます。

 @param [in]    nType  リサイズタイプ
 @param [in]    cx     幅
 @param [in]    cy     高さ
 */
void CIUCommonHashCalcDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	RECT stRect;

	CButton *pUpdateBtn = ((CButton *) this->GetDlgItem(IDD_IU_COM_HASH_CALC_BUTTON));
	// 変換ボタンをウィンドウサイズに合わせる
	if (pUpdateBtn != NULL && IsWindow(pUpdateBtn->GetSafeHwnd()) == TRUE) {
		pUpdateBtn->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		int nWidth = stRect.right - stRect.left;
		stRect.right = cx;
		stRect.left = cx - nWidth;
		pUpdateBtn->MoveWindow(&stRect);
	}

	int nButtonWidth = 0;
	CButton *pFileSelectBtn = ((CButton *) this->GetDlgItem(IDD_IU_COM_HASH_CALC_FILE_SELECT_BUTTON));
	// ファイル選択ボタンをウィンドウサイズに合わせる
	if (pFileSelectBtn != NULL && IsWindow(pFileSelectBtn->GetSafeHwnd()) == TRUE) {
		pFileSelectBtn->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		int nWidth = stRect.right - stRect.left;
		stRect.right = cx;
		stRect.left = cx - nWidth;
		pFileSelectBtn->MoveWindow(&stRect);
		nButtonWidth = nWidth;
	}

	nButtonWidth += 5;

	CEdit *pTargetEdit = ((CEdit *) this->GetDlgItem(IDD_IU_COM_HASH_CALC_TARGET_EDIT));
	// タイトルエリアをウィンドウサイズに合わせる
	if (pTargetEdit != NULL && IsWindow(pTargetEdit->GetSafeHwnd()) == TRUE) {
		pTargetEdit->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		stRect.right = cx - nButtonWidth;
		pTargetEdit->MoveWindow(&stRect);
	}
}

/*!
 @brief D&Dイベント\n
 WM_SIZEで呼び出されます。

 @param [in]    hDropInfo  ドロップ情報
 */
void CIUCommonHashCalcDlg::OnDropFiles(HDROP hDropInfo)
{
	UINT nDragFileCount = DragQueryFile(hDropInfo,  (UINT)-1, NULL, (UINT)0);

	// 複数のファイルがD&Dされたら中断
	if(nDragFileCount != 1) {
		return;
	}
	
	// ドラッグしたファイル名を取得し読み込みを行う
	TCHAR lpszFilePath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, lpszFilePath, MAX_PATH);

	GetDlgItem(IDD_IU_COM_HASH_CALC_TARGET_EDIT)->SetWindowText(lpszFilePath);

	DragFinish(hDropInfo);
}

/*!
 @brief 対象文字列入力イベント\n
 */
void CIUCommonHashCalcDlg::OnEnChangeIuComHashCalcTargetEdit()
{
	CString strTransData;
	GetDlgItem(IDD_IU_COM_HASH_CALC_TARGET_EDIT)->GetWindowText(strTransData);

	GetDlgItem(IDD_IU_COM_HASH_CALC_BUTTON)->EnableWindow(strTransData.IsEmpty() == FALSE);
}


/*!
 @brief ハッシュ計算設定

 @param [in]    hCryptKey     　ハッシュ方法
 @param [in]    nHashKeyLength  ハッシュ値の長さ
*/
void CIUCommonHashCalcDlg::SetHashSetting(HCRYPTKEY hCryptKey, int nHashKeyLength)
{
	m_hCryptKey = hCryptKey;
	m_nHashKeyLength = nHashKeyLength;
}

/*!
 @brief 情報取得\n
 指定コマンドを実行して情報を取得します。

 @param [in]    bInit 初期化時の呼び出し

 @return 情報
*/
CString CIUCommonHashCalcDlg::GetInformation(BOOL bInit)
{
	if (bInit == TRUE) {
		return _T("");
	}

	// 対象の文字列を取得
	int nTransMode = ((CComboBox *) GetDlgItem(IDD_IU_COM_HASH_CALC_TYPE_COMBO))->GetCurSel();
	CString strTransData;
	GetDlgItem(IDD_IU_COM_HASH_CALC_TARGET_EDIT)->GetWindowText(strTransData);

	BYTE *paryResult = NULL;
	CString strMessage;
	try {
		paryResult = new BYTE[m_nHashKeyLength];

		if (nTransMode == 0) {
			// ファイル変換
			CFile inFile;
			if (inFile.Open(strTransData, CFile::modeRead) == FALSE) {
				return _T("");
			}
			unsigned long long nFileLength = inFile.GetLength();
			BYTE *pFileData = new BYTE[(unsigned int) (nFileLength + 1)];

			if (pFileData != NULL) {
				inFile.Read(pFileData, (UINT) nFileLength);
				pFileData[nFileLength] = NULL;

				// Hash Codeを取得
				GetHashCode(pFileData, (DWORD) nFileLength, m_hCryptKey, m_nHashKeyLength, paryResult);

				delete[] pFileData;
				pFileData = NULL;
			}
		
		} else if (nTransMode == 1) {
			// 文字列変換
			LPCSTR pszTransData = NULL;

#ifndef UNICIODE
			int nSize = ::WideCharToMultiByte(CP_ACP, 0, strTransData, -1, NULL, NULL, NULL, NULL );

			// MBCSに変換してHash計算を行う
			char *pszTempData = new char[nSize];
			ZeroMemory(pszTempData, nSize);
			WideCharToMultiByte(CP_ACP, 0, strTransData, strTransData.GetLength(), pszTempData, nSize, NULL, NULL );
			pszTransData = pszTempData;
#else
			pszTransData = (LPCTSTR) m_strTransData;
#endif

			// Hash Codeを取得
			GetHashCode(pszTransData, strTransData.GetLength(), m_hCryptKey, m_nHashKeyLength, paryResult);

#ifndef UNICIODE
			delete[] pszTempData;
			pszTempData = NULL;
#endif
		}

		// ハッシュ値の表示
		CString strTmp;
		for(int i = 0; i < m_nHashKeyLength; i++)	{
			strTmp.Format(_T("%.2x"),paryResult[i]);
			strMessage += strTmp;
		}
	} catch (...) {
		// 例外処理は行わない
	}

	if (paryResult != NULL) {
		delete[] paryResult;
		paryResult = NULL;
	}

	return strMessage;
}

/*!
 @brief ハッシュ計算結果取得\n

 @param [in]    pData       対象データ
 @param [in]    dwLen       対象データ長
 @param [in]    hCryptKey   ハッシュ方法
 @param [in]    nHashSize   キーサイズ
 @param [out]   pcbHashData ハッシュ値

 @return 結果
*/
bool CIUCommonHashCalcDlg::GetHashCode(const void* pData, DWORD dwLen, HCRYPTKEY hCryptKey, int nHashSize, BYTE *pcbHashData)
{
	bool		ret;
	HCRYPTPROV	hCryptProv;
	HCRYPTHASH	hHash;
	BYTE		pbHash[0x7f];
	DWORD		dwHashLen;

	::ZeroMemory(pcbHashData, nHashSize);
	if (pData == NULL || dwLen == 0) {
		return	false;
	}

	DWORD dwProvType = PROV_RSA_FULL;
	if (hCryptKey == CALG_SHA_256
		|| hCryptKey == CALG_SHA_384
		|| hCryptKey == CALG_SHA_512) {
		// SHA256,384,512は特殊処理
		dwProvType = PROV_RSA_AES;
	}

	// ハッシュ値の計算
	dwHashLen = nHashSize;
	hHash = NULL;
	hCryptProv = NULL;
	ret = false;
	if(::CryptAcquireContext(&hCryptProv, NULL, NULL, dwProvType, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET))
	{
		if(::CryptCreateHash(hCryptProv, hCryptKey, 0, 0, &hHash))
		{
			if(::CryptHashData(hHash,(BYTE*)pData, dwLen, 0))
			{
				if(::CryptGetHashParam(hHash, HP_HASHVAL, pbHash,&dwHashLen, 0)) 
				{
					// 計算結果をコピー
					::memcpy_s(pcbHashData, dwHashLen, pbHash, dwHashLen);
					ret = true;
				}
			}
		}
	}

	if (hHash != NULL) {
		::CryptDestroyHash(hHash);
	}

	if (hCryptProv != NULL) {
		::CryptReleaseContext(hCryptProv, 0);
	}

	if (ret == false) {
		::ZeroMemory(pcbHashData, dwHashLen);
	}

	return	ret;
}