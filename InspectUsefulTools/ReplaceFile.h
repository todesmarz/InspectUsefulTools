/*! @file  ReplaceFile.h
    @brief 置換処理クラスヘッダ
*/
#pragma once

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

	int Execute(CString &strFilePath, CString &strExtName);

private:
	
	void SearchFile(CString &strFilePath, CString &strExtName);
	bool ReadFile(CString &strFilePath, CStringArray &aryFileData);
	int RepaceFile(CStringArray &aryFileData);
	bool WriteFile(CString &strFilePath, CStringArray &aryFileData);
	bool CheckExt(CString &strFileName, CString &strExtName);
};
