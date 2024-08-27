#pragma once
#include "afxdialogex.h"
#include "PluginInterface.h"
#include <string>
#include "CVerifyXPathMessageBox.h"
#include "CVerifyXPathDebug.h"


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

private:
	void ShowResultMessage(int retVal);

protected:
	BOOL m_bDoLoadFromFile;
	CStringW m_sXPathToVerify;
	CStringW m_sXmlNamespace;
	CStringW m_sFileToOpen;
	CStringW m_sHowToUse;
	CStringW m_sDescription;
	CVerifyXPathMessageBox* m_pMessageBox;
	CVerifyXPathDebug* m_pDebugWnd;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CStringW ShowOpenFileDlg(CStringW filetypes);
	HWND getCurrentHScintilla(int which);
	int VerifyXPath();
	int IsSchemaValid();
	int GetTabContent(char** data, size_t* currentLength);
	LPCWSTR GetHowToUseText(BOOL getCustomFile);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedVerifyxpath();
	afx_msg void OnBnClickedOpenschemafile();
	afx_msg void OnBnClickedDoLoadfromfile();
	afx_msg void OnBnClickedClearXpathverify();
	afx_msg void OnBnClickedClearXpathverifyXmlns();
	afx_msg void OnEnChangeXpathverifyXmlns();
	afx_msg void OnBnClickedXpathverifyClose();
	afx_msg void OnStnClickedStaticHowtopick();
	afx_msg void OnBnClickedXpathverifyClose2();
	afx_msg void OnBnClickedXpathverifyViewlogs();
};
