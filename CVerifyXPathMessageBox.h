#pragma once
#include "afxdialogex.h"


// CVerifyXPathMessageBox dialog

class CVerifyXPathMessageBox : public CDialogEx
{
	DECLARE_DYNAMIC(CVerifyXPathMessageBox)

public:
	CVerifyXPathMessageBox(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CVerifyXPathMessageBox();

	void AddAll(const std::vector<LoggingEntryType>& errors);
	void AddItem(CListCtrl* list, CStringW context, CStringW level, CStringW message);
	void ClearList();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XPATHVERIFY_LOG };
#endif

protected:
	std::vector< std::tuple< CStringW, CStringW, CStringW > > m_listErrors;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedListXpathvClear();
};
