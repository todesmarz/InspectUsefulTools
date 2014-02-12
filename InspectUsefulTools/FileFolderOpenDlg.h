/*! @file  FileFolderOpenDlg.h
    @brief �t�H���_�I�[�v����ʃN���X�w�b�_
*/
#pragma once
#include "afxwin.h"

/*!
 @class CFileFolderOpenDlg
 @brief �t�H���_�I�[�v����ʃN���X
*/
class CFileFolderOpenDlg : public CDialog
{
	DECLARE_DYNAMIC(CFileFolderOpenDlg)

private:
	CString m_strFolderPath;				/// �t�@�C���p�X
	CListBox m_ctrlFileListBox;				/// �t�@�C�������R���g���[��

public:
	CFileFolderOpenDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^

// �_�C�A���O �f�[�^
	enum { IDD = IDD_FILE_FOLDER_OPEN_DIALOG };

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	afx_msg void OnBnClickedFilePathSelectButton();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnEnChangeFilePathEdit();
	afx_msg void OnBnClickedFileFolderOpenButton();
	afx_msg void OnLbnSelchangeFileFolderOpenRecentlyFileList();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
private:
	static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData);
};
