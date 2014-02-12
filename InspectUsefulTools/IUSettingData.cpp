#include "StdAfx.h"
#include "IUSettingData.h"

#include <io.h>

/*!
 @brief コンストラクタ
*/
CIUSettingData::CIUSettingData(void)
{
	Load();
}

/*!
 @brief 読み込み処理
*/
void CIUSettingData::Load() 
{
	TCHAR buff[MAX_PATH];

	// 実行中プログラムのフルパス
	GetModuleFileName(NULL, buff, MAX_PATH);
	PathRemoveFileSpec( buff );

	CString strFilePath = buff;
	strFilePath += _T("\\IUSetting.ini");
	if (_taccess(strFilePath, 0) != 0) {
		// 設定ファイルが存在しない
		return;
	}

	TCHAR profileBuf[1024];

	//
	// File
	//
	CString strAppName = _T("File");
	CString strKeyName = _T("LockFilePath");
	GetPrivateProfileString(strAppName, strKeyName, _T(""), profileBuf, sizeof(profileBuf) / sizeof(TCHAR), strFilePath);
	m_strFileLockPath = profileBuf;
	
	strKeyName = _T("TimeStampFilePath");
	GetPrivateProfileString(strAppName, strKeyName, _T(""), profileBuf, sizeof(profileBuf) / sizeof(TCHAR), strFilePath);
	m_strFileTimeStampPath = profileBuf;

	strKeyName = _T("AttributeFilePath");
	GetPrivateProfileString(strAppName, strKeyName, _T(""), profileBuf, sizeof(profileBuf) / sizeof(TCHAR), strFilePath);
	m_strAttributePath = profileBuf;
	
	strKeyName = _T("TailFilePath");
	GetPrivateProfileString(strAppName, strKeyName, _T(""), profileBuf, sizeof(profileBuf) / sizeof(TCHAR), strFilePath);
	m_strTailPath = profileBuf;
	
	strKeyName = _T("ReplaceFilePath");
	GetPrivateProfileString(strAppName, strKeyName, _T(""), profileBuf, sizeof(profileBuf) / sizeof(TCHAR), strFilePath);
	m_strReplacePath = profileBuf;
	
	strKeyName = _T("ReplaceSourceWord");
	GetPrivateProfileString(strAppName, strKeyName, _T(""), profileBuf, sizeof(profileBuf) / sizeof(TCHAR), strFilePath);
	m_strReplaceSourceWord = profileBuf;
	
	strKeyName = _T("ReplaceDestWord");
	GetPrivateProfileString(strAppName, strKeyName, _T(""), profileBuf, sizeof(profileBuf) / sizeof(TCHAR), strFilePath);
	m_strReplaceDestWord = profileBuf;
	
	strKeyName = _T("FolderOpenFilePath");
	GetPrivateProfileString(strAppName, strKeyName, _T(""), profileBuf, sizeof(profileBuf) / sizeof(TCHAR), strFilePath);
	m_strFolderOpenPath = profileBuf;
	
	strKeyName = _T("FolderOpenFilePathList");
	GetPrivateProfileString(strAppName, strKeyName, _T(""), profileBuf, sizeof(profileBuf) / sizeof(TCHAR), strFilePath);
	m_strFolderOpenPathList = profileBuf;
	
	//
	// Memo
	//
	strAppName = _T("Memo");
	strKeyName = _T("Content");

	int nMemoBufSize = 1024;
	while (true) {
		TCHAR *pMemoBuf = new TCHAR[nMemoBufSize];

		int nRetSize = GetPrivateProfileString(strAppName, strKeyName, _T(""), pMemoBuf, nMemoBufSize, strFilePath);
		if (nMemoBufSize - 1 <= nRetSize) {
			nMemoBufSize = nMemoBufSize * 2 + 1;
			delete[] pMemoBuf;
			continue;
		}

		m_strMemo = pMemoBuf;	
		delete[] pMemoBuf;
		break;
	}
	m_strMemo.Replace(_T("\\n"), _T("\n"));
}

/*!
 @brief 保存処理
*/
void CIUSettingData::Save() 
{
	TCHAR buff[MAX_PATH];

	// 実行中プログラムのフルパス
	GetModuleFileName(NULL, buff, MAX_PATH);
	PathRemoveFileSpec( buff );

	CString strFilePath = buff;
	strFilePath += _T("\\IUSetting.ini");
	if (_taccess(strFilePath, 0) != 0) {
		// 設定ファイルが存在しない
		return;
	}
	
	//
	// File
	//
	CString strAppName = _T("File");
	CString strKeyName = _T("LockFilePath");
	WritePrivateProfileString(strAppName, strKeyName, m_strFileLockPath, strFilePath);

	strKeyName = _T("TimeStampFilePath");	
	WritePrivateProfileString(strAppName, strKeyName, m_strFileTimeStampPath, strFilePath);
	
	strKeyName = _T("AttributeFilePath");	
	WritePrivateProfileString(strAppName, strKeyName, m_strAttributePath, strFilePath);
	
	strKeyName = _T("TailFilePath");	
	WritePrivateProfileString(strAppName, strKeyName, m_strTailPath, strFilePath);
	
	strKeyName = _T("ReplaceFilePath");	
	WritePrivateProfileString(strAppName, strKeyName, m_strReplacePath, strFilePath);

	strKeyName = _T("ReplaceSourceWord");	
	WritePrivateProfileString(strAppName, strKeyName, m_strReplaceSourceWord, strFilePath);
	
	strKeyName = _T("ReplaceDestWord");	
	WritePrivateProfileString(strAppName, strKeyName, m_strReplaceDestWord, strFilePath);
	
	strKeyName = _T("FolderOpenFilePath");	
	WritePrivateProfileString(strAppName, strKeyName, m_strFolderOpenPath, strFilePath);
	
	strKeyName = _T("FolderOpenFilePathList");	
	WritePrivateProfileString(strAppName, strKeyName, m_strFolderOpenPathList, strFilePath);

	//
	// Memo
	//
	CString strMemo = m_strMemo;
	strMemo.Replace(_T("\r\n"), _T("\\n"));

	strAppName = _T("Memo");
	strKeyName = _T("Content");	
	WritePrivateProfileString(strAppName, strKeyName, strMemo, strFilePath);
}