#pragma once
#include "afxdialogex.h"
#include "PluginInterface.h"


// CVerifyXPathOnSchemaDlg dialog

class CVerifyXPathOnSchemaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVerifyXPathOnSchemaDlg)

public:
	CVerifyXPathOnSchemaDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CVerifyXPathOnSchemaDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XPATHVERIFY };
#endif

protected:
	BOOL m_bDoLoadFromFile;
	CStringW m_sXPathToVerify;
	CStringW m_sXmlNamespace;
	CStringW m_sFileToOpen;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CStringW ShowOpenFileDlg(CStringW filetypes);
	HWND getCurrentHScintilla(int which);
	int VerifyXPath();
	int IsSchemaValid();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedVerifyxpath();
	afx_msg void OnBnClickedOpenschemafile();
	afx_msg void OnBnClickedDoLoadfromfile();
	afx_msg void OnBnClickedClearXpathverify();
	afx_msg void OnBnClickedClearXpathverifyXmlns();
	afx_msg void OnEnChangeXpathverifyXmlns();
	afx_msg void OnBnClickedXpathverifyClose();
};
