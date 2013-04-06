/*! @file  ReplaceFile.cpp
    @brief 置換処理クラス
*/
#include "StdAfx.h"
#include "ReplaceFile.h"

/*!
 @brief コンストラクタ

 @param [in]    strBeforeReplace     置換前
 @param [in]    strAfterReplace      置換後
*/
CReplaceFile::CReplaceFile(CString &strBeforeReplace, CString &strAfterReplace)
	:m_strBeforeReplace(strBeforeReplace) ,
	m_strAfterReplace(strAfterReplace)
{
	m_nReplaceCount = 0;
}

/*!
 @brief 置換を実行

 @param [in]    strFilePath     ファイルパス
 @param [in]    strExtName      対象拡張子リスト
*/
int CReplaceFile::Execute(CString &strFilePath, CString &strExtName)
{
	if ( strFilePath.IsEmpty () == TRUE) {
		return 0;
	}

	SearchFile(strFilePath, strExtName);

	return m_nReplaceCount;
}

/*!
 @brief 指定されたパスのファイルを取得する

 @param [in]    strFilePath     ファイルパス
 @param [in]    strExtName      対象拡張子リスト
*/
void CReplaceFile::SearchFile(CString &strFilePath, CString &strExtName)
{
	CString strSearchFilePath = strFilePath + _T("\\*.???");

	CFileFind inFileFind;
	BOOL bContinue  = inFileFind.FindFile(strSearchFilePath);

	while(bContinue == TRUE) {
		 bContinue = inFileFind.FindNextFile();

		 if (inFileFind.IsDots() == TRUE) {
			 continue;
		 }

		 CString strFindFilePath = inFileFind.GetFilePath();
		 // ディレクトリなら内部のファイルを検索
		 // それ以外なら対象の拡張子を判断し置換を行う。
		 if (inFileFind.IsDirectory() == TRUE) {
			 SearchFile(strFindFilePath, strExtName);
			continue;
		 } else if(CheckExt(strFindFilePath, strExtName) == true) {
			 CStringArray aryFileData;

			 ReadFile(strFindFilePath, aryFileData);

			 int nReplaceCount = RepaceFile(aryFileData);

			 if (nReplaceCount != 0) {
				 WriteFile(strFindFilePath, aryFileData);
				 m_nReplaceCount += nReplaceCount;
			 }
		 }
	}
}

/*!
 @brief 検索する拡張子か判断する。

 @param [in]    strFilePath     ファイルパス
 @param [in]    strExtName      対象拡張子リスト

 @return true 対象の拡張子 false 対象外の拡張子
*/
bool CReplaceFile::CheckExt(CString &strFileName, CString &strExtName)
{
	int nCursolPos = 0;
	CString strRet = strExtName.Tokenize(_T(","), nCursolPos);

	while (strRet.IsEmpty() == false) {
		if (strFileName.Right(strRet.GetLength() + 1).CompareNoCase(_T(".") + strRet) == 0) {
			return true;
		}
		 strRet = strExtName.Tokenize(_T(","), nCursolPos);
	}

	return false;
}

/*!
 @brief ファイルの読み込み

 @param [in]    strFilePath     ファイルパス
 @param [in,out]    aryFileData 読込結果

 @return true 正常終了 false 読み込みエラー
*/
bool CReplaceFile::ReadFile(CString &strFilePath, CStringArray &aryFileData)
{
	CStdioFile inFile;

	if (inFile.Open(strFilePath, CFile::modeRead) == TRUE) {
		CString strReadData;

		while ( inFile.ReadString(strReadData) != NULL) {
			aryFileData.Add(strReadData);
		}

		inFile.Close();

	} else {
		return false;
	}

	return true;
}

/*!
 @brief 置換処理

 @param [in,out]    aryFileData 置換結果

 @return 置換数
*/
int CReplaceFile::RepaceFile(CStringArray &aryFileData)
{
	int nReplaceCount = 0;
	const INT_PTR nDataSize = aryFileData.GetSize();

	for (INT_PTR nDataIndex = 0; nDataIndex < nDataSize; nDataIndex++) {
		CString strReplaceData =  aryFileData.GetAt(nDataIndex);
		nReplaceCount += strReplaceData.Replace(m_strBeforeReplace, m_strAfterReplace);

		aryFileData.SetAt(nDataIndex, strReplaceData);
	}

	return nReplaceCount;
}

/*!
 @brief ファイルへ書き込み

 @param [in]    strFilePath     ファイルパス
 @param [in,out]    aryFileData 置換結果

 @return true 正常終了 false 書き込みエラー
*/
bool CReplaceFile::WriteFile(CString &strFilePath, CStringArray &aryFileData)
{
	CStdioFile inFile;

	if (inFile.Open(strFilePath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite) == TRUE) {
		const INT_PTR nDataSize = aryFileData.GetSize();

		for (INT_PTR nDataIndex = 0; nDataIndex < nDataSize; nDataIndex++) {
			CString a = aryFileData.GetAt(nDataIndex);
			 inFile.WriteString(a);
			 inFile.WriteString(_T("\n"));
		}

		inFile.Close();

	} else {
		return false;
	}

	return true;
}