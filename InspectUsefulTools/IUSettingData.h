#pragma once
class CIUSettingData
{
private:
	CIUSettingData(void);

	void Load();

public:
	/*!
	 @brief インスタンス取得
	*/
	static CIUSettingData& GetInstance() {
		static CIUSettingData instance;
		return instance;
	}

	void Save();

public:
	CString m_strFileLockPath;
	CString m_strFileTimeStampPath;
	CString m_strAttributePath;
	CString m_strTailPath;
	CString m_strReplacePath;
	CString m_strReplaceSourceWord;
	CString m_strReplaceDestWord;
	CString m_strFolderOpenPath;
	CString m_strFolderOpenPathList;

	CString m_strMemo;
};

