#pragma once
#include "afxdialogex.h"


// CVerifyXPathDebug dialog

class CVerifyXPathDebug : public CDialogEx
{
	DECLARE_DYNAMIC(CVerifyXPathDebug)

public:
	CVerifyXPathDebug(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CVerifyXPathDebug();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XPATHVERIFY_DBG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};
