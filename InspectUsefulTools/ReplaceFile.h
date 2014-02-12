/*! @file  ReplaceFile.h
    @brief 置換処理クラスヘッダ
*/
#pragma once


typedef enum {
	ASCII,
	EUC_JP,
	UTF_8,
	Unicode

} EReplaceFileEncoding;

/*!
 @class CReplaceFile
 @brief 置換処理クラス
*/
class CReplaceFile
{
private:
	CString m_strBeforeReplace;
	CString m_strAfterReplace;
	int m_nReplaceCount;

public:
	CReplaceFile(CString &strBeforeReplace, CString &strAfterReplace);

	int Execute(CString &strFilePath, CString &strExtName, EReplaceFileEncoding encoding = ASCII);

private:
	
	void SearchFile(CString &strFilePath, CString &strExtName, EReplaceFileEncoding encoding);
	bool ReadFile(CString &strFilePath, CStringArray &aryFileData, EReplaceFileEncoding encoding);
	int RepaceFile(CStringArray &aryFileData);
	bool WriteFile(CString &strFilePath, CStringArray &aryFileData, EReplaceFileEncoding encoding);
	bool CheckExt(CString &strFileName, CString &strExtName);
	bool ConvertToEncodeString(EReplaceFileEncoding encoding, CString &strEncodeString);
};