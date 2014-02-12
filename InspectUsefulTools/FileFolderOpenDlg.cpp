/*! @file  FileFolderOpenDlg.cpp
    @brief �t�H���_�I�[�v����ʃN���X
*/
#include "stdafx.h"
#include "InspectUsefulTools.h"
#include "FileFolderOpenDlg.h"
#include "IUSettingData.h"

#include <locale.h>

IMPLEMENT_DYNAMIC(CFileFolderOpenDlg, CDialog)

/*!
 @brief �R���X�g���N�^

 @param [in]    pParent     �e�E�B���h�E�C���X�^���X
*/
CFileFolderOpenDlg::CFileFolderOpenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileFolderOpenDlg::IDD, pParent)
{

}

/*!
 @brief �_�C�A���O �f�[�^�̌����ƗL�����`�F�b�N�̂��߂Ƀt���[�����[�N���Ăяo���܂��B

 @param [in]    pDX     CDataExchange �I�u�W�F�N�g�ւ̃|�C���^
*/
void CFileFolderOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDD_FILE_FOLDER_OPEN_PATH_EDIT, m_strFolderPath);
	DDX_Control(pDX, IDD_FILE_FOLDER_OPEN_RECENTLY_FILE_LIST, m_ctrlFileListBox);
}

/*!
 @brief ���b�Z�[�W�}�b�v��`
*/
BEGIN_MESSAGE_MAP(CFileFolderOpenDlg, CDialog)
	ON_BN_CLICKED(IDD_FILE_FOLDER_OPEN_PATH_SELECT_BUTTON, &CFileFolderOpenDlg::OnBnClickedFilePathSelectButton)
	ON_WM_DROPFILES()
	ON_EN_CHANGE(IDD_FILE_FOLDER_OPEN_PATH_EDIT, &CFileFolderOpenDlg::OnEnChangeFilePathEdit)
	ON_BN_CLICKED(IDD_FILE_FOLDER_OPEN_BUTTON, &CFileFolderOpenDlg::OnBnClickedFileFolderOpenButton)
	ON_LBN_SELCHANGE(IDD_FILE_FOLDER_OPEN_RECENTLY_FILE_LIST, &CFileFolderOpenDlg::OnLbnSelchangeFileFolderOpenRecentlyFileList)
	ON_WM_SIZE()
END_MESSAGE_MAP()

/*!
 @brief ���̃��\�b�h�� WM_INITDIALOG �̃��b�Z�[�W�ɉ������ČĂяo����܂��B
*/
BOOL CFileFolderOpenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// D&D�ݒ�
	::SHAutoComplete(::GetDlgItem(this->m_hWnd, IDD_FILE_FOLDER_OPEN_PATH_EDIT), SHACF_FILESYSTEM);
	DragAcceptFiles();

	
	m_strFolderPath = CIUSettingData::GetInstance().m_strFolderOpenPath;

	CString strList = CIUSettingData::GetInstance().m_strFolderOpenPathList;
	int nIndex = 0;
	while (0 <= (nIndex = strList.Find(_T("::")))) {
		CString filePath =  strList.Left(nIndex);

		//���ϐ���W�J����̂ɕK�v�ȃT�C�Y���擾����
		DWORD bufferSize = ExpandEnvironmentStrings(m_strFolderPath, NULL, 0);
		//�K�v�ȃT�C�Y���m��
		PTSTR pBuffer = new TCHAR[bufferSize];
		//���ϐ���W�J
		bufferSize = ExpandEnvironmentStrings(m_strFolderPath, pBuffer, bufferSize);

		if (_taccess(pBuffer, 0) == 0) {
			m_ctrlFileListBox.InsertString(0, filePath);
		}
		strList = strList.Mid(nIndex + 2);
	}

	//���ϐ���W�J����̂ɕK�v�ȃT�C�Y���擾����
	DWORD bufferSize = ExpandEnvironmentStrings(m_strFolderPath, NULL, 0);
	//�K�v�ȃT�C�Y���m��
	PTSTR pBuffer = new TCHAR[bufferSize];
	//���ϐ���W�J
	bufferSize = ExpandEnvironmentStrings(m_strFolderPath, pBuffer, bufferSize);

	if (strList.IsEmpty() == false && _taccess(pBuffer, 0) == 0) {
		m_ctrlFileListBox.InsertString(0, strList);
	}
	UpdateData(FALSE);

	if (m_strFolderPath.IsEmpty() == false) {
			((CDialog *) this->GetDlgItem(IDD_FILE_FOLDER_OPEN_BUTTON))->EnableWindow(TRUE);
	}

	return TRUE;
}

/*!
 @brief Windows �֐� TranslateMessage �� DispatchMessage �Ƀf�B�X�p�b�`�����O�ɁA�E�B���h�E ���b�Z�[�W���t�B���^��������ɂ́A���̊֐����I�[�o�[���C�h���܂��B\n
 ����̎����ł́A�A�N�Z�����[�^ �L�[�̕ϊ����s���܂��B\n
 ���̂��߁A�I�[�o�[���C�h���� CWinApp::PreTranslateMessage �����o�֐����Ăяo���K�v������܂��B
*/
BOOL CFileFolderOpenDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
        return 0;
        //  �K���O�Ŗ߂邱��!!
        //  0�ȊO�Ŗ߂�Ɗe�R���g���[���֎��̏��������Ȃ��Ȃ�
    }
    return CDialog::PreTranslateMessage(pMsg);
}

/*!
 @brief �t�@�C���I���{�^�����N���b�N����ƌĂяo����܂��B
*/
void CFileFolderOpenDlg::OnBnClickedFilePathSelectButton()
{
	// ���ׂẴt�H���_��Ώ�
	BROWSEINFO sBrowseInfo;
	TCHAR szDisplayName[MAX_PATH];

	UpdateData(TRUE);
	_tcscpy_s(szDisplayName, m_strFolderPath);

	sBrowseInfo.hwndOwner = this->m_hWnd;
	sBrowseInfo.pidlRoot = NULL;
	sBrowseInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	sBrowseInfo.lParam = (LPARAM)szDisplayName;
	sBrowseInfo.pszDisplayName = szDisplayName;
	sBrowseInfo.lpszTitle = NULL;
	sBrowseInfo.lpfn = &CFileFolderOpenDlg::BrowseCallbackProc;

	LPITEMIDLIST pidlBrowse =::SHBrowseForFolder(&sBrowseInfo);

	if (pidlBrowse != NULL) {
		TCHAR szSelectFolderPath[MAX_PATH];
		if (SHGetPathFromIDList(pidlBrowse, szSelectFolderPath) == TRUE) {
			m_strFolderPath = szSelectFolderPath;

			((CDialog *) this->GetDlgItem(IDD_FILE_FOLDER_OPEN_BUTTON))->EnableWindow(TRUE);

			UpdateData(FALSE);
		}
	}
}

/*!
 @brief �t�H���_�I���R�[���o�b�N�֐�

 @param [in]    hwnd     �E�B���h�E�n���h��
 @param [in]    uMsg     ���b�Z�[�W
 @param [in]    lParam   �p�����[�^
 @param [in]    lpData   �p�����[�^
*/
int CALLBACK CFileFolderOpenDlg::BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM /*lParam*/,LPARAM lpData)
{
    if(uMsg==BFFM_INITIALIZED && lpData != NULL){
		::SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
    }
    return 0;
}

/*!
 @brief D&D����ƌĂяo����܂��B

 @param [in]    hDropInfo     D&D���
*/
void CFileFolderOpenDlg::OnDropFiles(HDROP hDropInfo)
{
	UINT nDragFileCount = DragQueryFile(hDropInfo,  (UINT)-1, NULL, (UINT)0);

	// �����̃t�@�C����D&D���ꂽ�璆�f
	if(nDragFileCount != 1) {
		return;
	}
	
	// �h���b�O�����t�@�C�������擾���ǂݍ��݂��s��
	TCHAR lpszFilePath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, lpszFilePath, MAX_PATH);

	m_strFolderPath = lpszFilePath;
	((CDialog *) this->GetDlgItem(IDD_FILE_FOLDER_OPEN_BUTTON))->EnableWindow(TRUE);
	UpdateData(FALSE);

	DragFinish(hDropInfo);
}

/*!
 @brief �t�@�C���p�X��ύX����ƌĂяo����܂��B
*/
void CFileFolderOpenDlg::OnEnChangeFilePathEdit()
{
	UpdateData(TRUE);
	
	//���ϐ���W�J����̂ɕK�v�ȃT�C�Y���擾����
	DWORD bufferSize = ExpandEnvironmentStrings(m_strFolderPath, NULL, 0);
	//�K�v�ȃT�C�Y���m��
	PTSTR pBuffer = new TCHAR[bufferSize];
	//���ϐ���W�J
	bufferSize = ExpandEnvironmentStrings(m_strFolderPath, pBuffer, bufferSize);

	bool bEnable = false;
	if (_taccess_s(pBuffer, 0) == 0) {
		// �t�@�C�������݂���
		bEnable = true;
	}

	((CDialog *) this->GetDlgItem(IDD_FILE_FOLDER_OPEN_BUTTON))->EnableWindow(bEnable);
}

/*!
 @brief �t�H���_���J��
*/
void CFileFolderOpenDlg::OnBnClickedFileFolderOpenButton()
{
	//���ϐ���W�J����̂ɕK�v�ȃT�C�Y���擾����
	DWORD bufferSize = ExpandEnvironmentStrings(m_strFolderPath, NULL, 0);
	//�K�v�ȃT�C�Y���m��
	PTSTR pBuffer = new TCHAR[bufferSize];
	//���ϐ���W�J
	bufferSize = ExpandEnvironmentStrings(m_strFolderPath, pBuffer, bufferSize);

	ShellExecute(NULL, _T("open"), pBuffer, NULL, NULL, SW_SHOWNORMAL);

	int nFindIndex = m_ctrlFileListBox.FindString(0, m_strFolderPath);
	if (0 <= nFindIndex) {
		// �����ɑ��݂���ꍇ�͍폜���A��ɒǉ�����
		m_ctrlFileListBox.DeleteString(nFindIndex);
	}
	
	// �ݒ�ɔ��f
	CIUSettingData::GetInstance().m_strFolderOpenPath = m_strFolderPath;

	m_ctrlFileListBox.InsertString(0, m_strFolderPath);

	
	CString strList;
	int nItemCount = m_ctrlFileListBox.GetCount();
	for (int nAryIndex = nItemCount - 1;  max(nItemCount - 10, 0) <= nAryIndex; nAryIndex--) {
		CString strItem;
		m_ctrlFileListBox.GetText(nAryIndex, strItem);
		strList += strItem + _T("::");
	}
	CIUSettingData::GetInstance().m_strFolderOpenPathList = strList;
}

/*!
 @brief �I��ύX��
*/
void CFileFolderOpenDlg::OnLbnSelchangeFileFolderOpenRecentlyFileList()
{
	CString strSelectItem;
	int nSelIndex = m_ctrlFileListBox.GetCurSel();
	if (nSelIndex < 0) {
		return;
	}

	m_ctrlFileListBox.GetText(nSelIndex, strSelectItem);

	m_strFolderPath = strSelectItem;
	UpdateData(FALSE);
	
	//���ϐ���W�J����̂ɕK�v�ȃT�C�Y���擾����
	DWORD bufferSize = ExpandEnvironmentStrings(m_strFolderPath, NULL, 0);
	//�K�v�ȃT�C�Y���m��
	PTSTR pBuffer = new TCHAR[bufferSize];
	//���ϐ���W�J
	bufferSize = ExpandEnvironmentStrings(m_strFolderPath, pBuffer, bufferSize);

	bool bEnable = false;
	if (_taccess_s(pBuffer, 0) == 0) {
		// �t�@�C�������݂���
		bEnable = true;
	}

	((CDialog *) this->GetDlgItem(IDD_FILE_FOLDER_OPEN_BUTTON))->EnableWindow(bEnable);
}

/*!
 @brief ���T�C�Y�C�x���g\n
 WM_SIZE�ŌĂяo����܂��B

 @param [in]    nType  ���T�C�Y�^�C�v
 @param [in]    cx     ��
 @param [in]    cy     ����
 */
void CFileFolderOpenDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	RECT stRect;

	int nWidth = 0;
	CButton *pSelectButton = ((CButton *) this->GetDlgItem(IDD_FILE_FOLDER_OPEN_PATH_SELECT_BUTTON));
	// �\�����x�����E�B���h�E�T�C�Y�ɍ��킹��
	if (pSelectButton != NULL && IsWindow(pSelectButton->GetSafeHwnd()) == TRUE) {
		pSelectButton->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		nWidth = stRect.right - stRect.left;
		stRect.right = cx;
		stRect.left = stRect.right - nWidth;
		pSelectButton->MoveWindow(&stRect);
	}

	nWidth += 5;

	CEdit *pFilePathEdit = ((CEdit *) this->GetDlgItem(IDD_FILE_FOLDER_OPEN_PATH_EDIT));
	// �\�����x�����E�B���h�E�T�C�Y�ɍ��킹��
	if (pFilePathEdit != NULL && IsWindow(pFilePathEdit->GetSafeHwnd()) == TRUE) {
		pFilePathEdit->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		stRect.right = cx- nWidth;
		pFilePathEdit->MoveWindow(&stRect);
	}

	CButton *pOpenButton = ((CButton *) this->GetDlgItem(IDD_FILE_FOLDER_OPEN_BUTTON));
	// �\�����x�����E�B���h�E�T�C�Y�ɍ��킹��
	if (pOpenButton != NULL && IsWindow(pOpenButton->GetSafeHwnd()) == TRUE) {
		pOpenButton->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		nWidth = stRect.right - stRect.left;
		stRect.right = cx;
		stRect.left = stRect.right - nWidth;
		pOpenButton->MoveWindow(&stRect);
	}

	CListBox *pRecentlyList = ((CListBox *) this->GetDlgItem(IDD_FILE_FOLDER_OPEN_RECENTLY_FILE_LIST));
	// �\�����x�����E�B���h�E�T�C�Y�ɍ��킹��
	if (pRecentlyList != NULL && IsWindow(pRecentlyList->GetSafeHwnd()) == TRUE) {
		pRecentlyList->GetWindowRect(&stRect);
		this->ScreenToClient(&stRect);
		stRect.bottom = cy;
		stRect.right = cx;
		pRecentlyList->MoveWindow(&stRect);
	}
}
