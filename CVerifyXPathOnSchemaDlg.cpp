// CVerifyXPathOnSchemaDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "Scintilla.h"
#include "XMLTools.h"
#include "afxdialogex.h"
#include "CVerifyXPathOnSchemaDlg.h"
#include "LibxmlWrapper.h"
#include "Report.h"
#include "VerifyXPathMessages.h"

#define _CRTDBG_MAP_ALLOC

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
	m_sHowToUse = gHowToUseWithoutFile;
	m_sDescription = gDescription;
	m_pMessageBox = new CVerifyXPathMessageBox(this);
	m_pDebugWnd = new CVerifyXPathDebug(this);
}

CVerifyXPathOnSchemaDlg::~CVerifyXPathOnSchemaDlg()
{
	delete m_pMessageBox;
}


BEGIN_MESSAGE_MAP(CVerifyXPathOnSchemaDlg, CDialogEx)
	ON_BN_CLICKED(ID_VERIFYXPATH, &CVerifyXPathOnSchemaDlg::OnBnClickedVerifyxpath)
	ON_BN_CLICKED(IDC_OPENSCHEMAFILE, &CVerifyXPathOnSchemaDlg::OnBnClickedOpenschemafile)
	ON_BN_CLICKED(IDC_DO_LOADFROMFILE, &CVerifyXPathOnSchemaDlg::OnBnClickedDoLoadfromfile)
	ON_BN_CLICKED(IDC_CLEAR_XPATHVERIFY, &CVerifyXPathOnSchemaDlg::OnBnClickedClearXpathverify)
	ON_BN_CLICKED(IDC_CLEAR_XPATHVERIFY_XMLNS, &CVerifyXPathOnSchemaDlg::OnBnClickedClearXpathverifyXmlns)
	ON_EN_CHANGE(IDC_XPATHVERIFY_XMLNS, &CVerifyXPathOnSchemaDlg::OnEnChangeXpathverifyXmlns)
	ON_BN_CLICKED(IDC_XPATHVERIFY_CLOSE, &CVerifyXPathOnSchemaDlg::OnBnClickedXpathverifyClose)
	ON_STN_CLICKED(IDC_STATIC_HOWTOPICK, &CVerifyXPathOnSchemaDlg::OnStnClickedStaticHowtopick)
	ON_BN_CLICKED(IDC_XPATHVERIFY_VIEWLOGS, &CVerifyXPathOnSchemaDlg::OnBnClickedXpathverifyViewlogs)
END_MESSAGE_MAP()


// CVerifyXPathOnSchemaDlg message handlers

int CVerifyXPathOnSchemaDlg::VerifyXPath() {
	if (m_sXPathToVerify.GetLength() == 0) {
		Report::_printf_err(_T("Please type in an XPath query."));
		return (-2);
	}
	int retVal = 0;						   
	LibxmlWrapper* libxmlWrapper = nullptr;
	if (m_bDoLoadFromFile) {
		libxmlWrapper = new LibxmlWrapper("", 0);
		if (!libxmlWrapper) {
			Report::_printf_err(_T("Could not allocate memory for verifying "
				"if the XPath query is satisfiable."));
			return (-1);
		}
		// TODO return -1 for allocation errors
		retVal = (int)libxmlWrapper->isXPathValidOnSchema(
			m_sFileToOpen.GetString(), m_sFileToOpen.GetLength(), 
			m_sXPathToVerify.GetString(), m_sXPathToVerify.GetLength()
		);
	}
	else {
		char* data = nullptr;
		size_t length = 0;

		if (GetTabContent(&data, &length) != 0) {
			Report::_printf_err("Could not retrieve the content of the "
				"currently opened tab when verifying if the provided XPath "
				"query is satisifable on the schema.");
			return (-1);
		}
		libxmlWrapper = new LibxmlWrapper(data, length);

		// TODO return -1 for allocation errors
		retVal = (int)libxmlWrapper->isXPathValidOnSchema(
			m_sXPathToVerify.GetString(), m_sXPathToVerify.GetLength());
	}

	m_pMessageBox->AddAll(libxmlWrapper->getLastErrors());

	if (libxmlWrapper) {
		delete libxmlWrapper;
	}
	return retVal;
}

void CVerifyXPathOnSchemaDlg::ShowResultMessage(int retVal) 
{
	LPCTSTR text;
	UINT iconType;

	if (retVal == 1) {
		text = gTextValid;
		iconType = MB_ICONINFORMATION;
	}
	else if (retVal == 0) {
		text = gTextInvalid;
		iconType = MB_ICONINFORMATION;
	}
	else if (retVal < 0) {
		text = gTextError;
		iconType = MB_ICONERROR;
	}
	else {
		text = gTextUnknown;
		iconType = MB_ICONERROR;
	}

	// TODO in case of error or unknown, ask user if he wants to list errors.
	 ::MessageBox(nppData._nppHandle, text, gCaption, MB_OK | iconType);
	 if (retVal > 1 || retVal < 0) {
		 m_pMessageBox->DoModal();
	 }
	// Report::_printf_custom(nppData._nppHandle, text, gCaption, MB_OK | iconType, [] {});
}

int CVerifyXPathOnSchemaDlg::GetTabContent(char** data, size_t* currentLength) {
	int currentEdit;
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&currentEdit);
	HWND hCurrentEditView = getCurrentHScintilla(currentEdit);

	*currentLength = (size_t) ::SendMessage(hCurrentEditView, SCI_GETLENGTH, 0, 0);

	*data = new char[*currentLength + sizeof(char)];
	if (!*data) {
		*currentLength = 0;
		return -1;  // allocation error, abort check
	}
	memset(*data, '\0', *currentLength + sizeof(char));

	::SendMessage(hCurrentEditView, SCI_GETTEXT, *currentLength + sizeof(char), reinterpret_cast<LPARAM>(*data));
	
	return 0;
}

int CVerifyXPathOnSchemaDlg::IsSchemaValid() {
	if (m_bDoLoadFromFile) {
		// Text is loaded from file, not provided in memory, hence the empty constructor arguments
		int retVal = 1;
		LibxmlWrapper* libxmlWrapper = new LibxmlWrapper("", 0);

		if (!libxmlWrapper) {
			Report::_printf_err(gTextLibxmlWrapperAllocError);
			return (-1);
		}

		if (!libxmlWrapper->isValidSchema(
			  m_sFileToOpen.GetString(), m_sFileToOpen.GetLength()
		    )) {
			Report::_printf_err(gTextInvalidSchemaFromFileError);
			retVal = (0);
		}
		delete libxmlWrapper;
		return retVal;
	}
	else {
		char* data = nullptr;
		size_t length = 0;
		int retVal = 1;
		
		if (GetTabContent(&data, &length) != 0) {
			Report::_printf_err(gTextTabContentError);
			return (-1);
		}

		LibxmlWrapper* libxmlWrapper = new LibxmlWrapper(data, length);
		if (!libxmlWrapper) {
			Report::_printf_err(gTextMemoryError);
			delete data;
			return (-1);
		}

		if (!libxmlWrapper->isValidSchema()) {
			Report::_printf_err(gTextInvalidSchema);
			retVal = 0;
		}
		delete libxmlWrapper;
		delete data;
		return retVal;
	}
}

void CVerifyXPathOnSchemaDlg::OnBnClickedVerifyxpath()
{
	this->UpdateData();
	if (IsSchemaValid() > 0) {
		int retVal;
		retVal = VerifyXPath();
		ShowResultMessage(retVal);
	}
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}

void CVerifyXPathOnSchemaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_XPATHVERIFY_QUERY, m_sXPathToVerify);
	DDX_Text(pDX, IDC_XPATHVERIFY_XMLNS, m_sXmlNamespace);
	DDX_Text(pDX, IDC_LOADFILEPATH, m_sFileToOpen);

	DDX_Check(pDX, IDC_DO_LOADFROMFILE, m_bDoLoadFromFile);
	GetDlgItem(IDC_LOADFILEPATH)->EnableWindow(m_bDoLoadFromFile);
	GetDlgItem(IDC_OPENSCHEMAFILE)->EnableWindow(m_bDoLoadFromFile);
	GetDlgItem(IDC_STATIC_HOWTOPICK)->SetWindowTextW(
		GetHowToUseText(m_bDoLoadFromFile));

	DDX_Text(pDX, IDC_STATIC_HOWTOPICK, m_sHowToUse);
	DDX_Text(pDX, IDC_STATIC_XPATHVERIFY_DESCRIPTION, m_sDescription);
}

void CVerifyXPathOnSchemaDlg::OnBnClickedOpenschemafile()
{
	CStringW ret = ShowOpenFileDlg("XML Schema Files|*.xd;*.xsd|All files|*.*|");
	if (ret.GetLength()) GetDlgItem(IDC_LOADFILEPATH)->SetWindowText(ret);
}


void CVerifyXPathOnSchemaDlg::OnBnClickedDoLoadfromfile()
{
	this->UpdateData();
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

LPCWSTR CVerifyXPathOnSchemaDlg::GetHowToUseText(BOOL getCustomFile)
{
	return (getCustomFile ? gHowToUseWithFile : gHowToUseWithoutFile);
}


void CVerifyXPathOnSchemaDlg::OnBnClickedXpathverifyClose()
{
	CDialogEx::OnCancel();
}


void CVerifyXPathOnSchemaDlg::OnStnClickedStaticHowtopick()
{
	// TODO: Add your control notification handler code here
}



void CVerifyXPathOnSchemaDlg::OnBnClickedXpathverifyViewlogs()
{
	m_pDebugWnd->DoModal();
	// TODO: Add your control notification handler code here
}
