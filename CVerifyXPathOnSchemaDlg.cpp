// CVerifyXPathOnSchemaDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "Scintilla.h"
#include "XMLTools.h"
#include "afxdialogex.h"
#include "CVerifyXPathOnSchemaDlg.h"
#include "LibxmlWrapper.h"
#include "Report.h"


// CVerifyXPathOnSchemaDlg dialog

IMPLEMENT_DYNAMIC(CVerifyXPathOnSchemaDlg, CDialogEx)

HWND CVerifyXPathOnSchemaDlg::getCurrentHScintilla(int which) {
	return (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;
}

CVerifyXPathOnSchemaDlg::CVerifyXPathOnSchemaDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_XPATHVERIFY, pParent)
{
	m_sXPathToVerify = _T("");
	m_sXmlNamespace = _T("");
	m_sFileToOpen = _T("");
	m_bDoLoadFromFile = FALSE;
}

CVerifyXPathOnSchemaDlg::~CVerifyXPathOnSchemaDlg()
{
}


BEGIN_MESSAGE_MAP(CVerifyXPathOnSchemaDlg, CDialogEx)
	ON_BN_CLICKED(ID_VERIFYXPATH, &CVerifyXPathOnSchemaDlg::OnBnClickedVerifyxpath)
	ON_BN_CLICKED(IDC_OPENSCHEMAFILE, &CVerifyXPathOnSchemaDlg::OnBnClickedOpenschemafile)
	ON_BN_CLICKED(IDC_DO_LOADFROMFILE, &CVerifyXPathOnSchemaDlg::OnBnClickedDoLoadfromfile)
	ON_BN_CLICKED(IDC_CLEAR_XPATHVERIFY, &CVerifyXPathOnSchemaDlg::OnBnClickedClearXpathverify)
	ON_BN_CLICKED(IDC_CLEAR_XPATHVERIFY_XMLNS, &CVerifyXPathOnSchemaDlg::OnBnClickedClearXpathverifyXmlns)
	ON_EN_CHANGE(IDC_XPATHVERIFY_XMLNS, &CVerifyXPathOnSchemaDlg::OnEnChangeXpathverifyXmlns)
	ON_BN_CLICKED(IDC_XPATHVERIFY_CLOSE, &CVerifyXPathOnSchemaDlg::OnBnClickedXpathverifyClose)
END_MESSAGE_MAP()


// CVerifyXPathOnSchemaDlg message handlers

int CVerifyXPathOnSchemaDlg::VerifyXPath() {
	if (m_bDoLoadFromFile) {
		
	}
	else {

	}
	return 0;
}

int CVerifyXPathOnSchemaDlg::IsSchemaValid() {
	if (m_bDoLoadFromFile) {
		// Text is loaded from file, not provided in memory, hence the empty constructor arguments
		LibxmlWrapper* libxmlWrapper = new LibxmlWrapper("", 0);

		if (!libxmlWrapper->isValidSchema(m_sFileToOpen.GetString(), m_sFileToOpen.GetLength())) {
			Report::_printf_err(_T("The given file is not existing or it is not a valid XML schema."));
			return (0);
		}
		return 1;
	}
	else {
		int currentEdit;
		::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&currentEdit);
		HWND hCurrentEditView = getCurrentHScintilla(currentEdit);
		
		size_t currentLength = (size_t) ::SendMessage(hCurrentEditView, SCI_GETLENGTH, 0, 0);

		char* data = new char[currentLength + sizeof(char)];
		if (!data) return(-1);  // allocation error, abort check
		memset(data, '\0', currentLength + sizeof(char));

		::SendMessage(hCurrentEditView, SCI_GETTEXT, currentLength + sizeof(char), reinterpret_cast<LPARAM>(data));

		LibxmlWrapper* libxmlWrapper = new LibxmlWrapper(data, currentLength);

		if (!libxmlWrapper->isValidSchema()) {
			Report::_printf_err(_T("The content inside the tab is not a valid XML schema. Please provide a valid schema in the tab content or choose a different file."));
			return 0;
		}
		return 1;
	}
}

void CVerifyXPathOnSchemaDlg::OnBnClickedVerifyxpath()
{
	this->UpdateData();
	if (IsSchemaValid() > 0) {
		VerifyXPath();
	}
}

void CVerifyXPathOnSchemaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_XPATHVERIFY_QUERY, m_sXPathToVerify);
	DDX_Text(pDX, IDC_XPATHVERIFY_XMLNS, m_sXmlNamespace);
	DDX_Text(pDX, IDC_LOADFILEPATH, m_sFileToOpen);
	DDX_Check(pDX, IDC_DO_LOADFROMFILE, m_bDoLoadFromFile);
}

void CVerifyXPathOnSchemaDlg::OnBnClickedOpenschemafile()
{
	CStringW ret = ShowOpenFileDlg("XML Schema Files|*.xd;*.xsd|All files|*.*|");
	if (ret.GetLength()) GetDlgItem(IDC_LOADFILEPATH)->SetWindowText(ret);
}


void CVerifyXPathOnSchemaDlg::OnBnClickedDoLoadfromfile()
{
	this->UpdateData();
	GetDlgItem(IDC_LOADFILEPATH)->EnableWindow(m_bDoLoadFromFile);
	GetDlgItem(IDC_OPENSCHEMAFILE)->EnableWindow(m_bDoLoadFromFile);
}

CStringW CVerifyXPathOnSchemaDlg::ShowOpenFileDlg(CStringW filetypes) {
	CFileDialog dlg(TRUE, NULL, NULL, NULL, filetypes);
	INT_PTR ret = dlg.DoModal();
	if (ret == IDOK) {
		return dlg.GetPathName();
	}
	return "";
}

void CVerifyXPathOnSchemaDlg::OnBnClickedClearXpathverify()
{
	GetDlgItem(IDC_XPATHVERIFY_QUERY)->SetWindowTextW(_T(""));
}

void CVerifyXPathOnSchemaDlg::OnBnClickedClearXpathverifyXmlns()
{
	GetDlgItem(IDC_XPATHVERIFY_XMLNS)->SetWindowTextW(_T(""));
}

void CVerifyXPathOnSchemaDlg::OnEnChangeXpathverifyXmlns()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CVerifyXPathOnSchemaDlg::OnBnClickedXpathverifyClose()
{
	CDialogEx::OnCancel();
}
