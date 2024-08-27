// CVerifyXPathDebug.cpp : implementation file
//

#include "StdAfx.h"
#include "XMLTools.h"
#include "afxdialogex.h"
#include "CVerifyXPathDebug.h"


// CVerifyXPathDebug dialog

IMPLEMENT_DYNAMIC(CVerifyXPathDebug, CDialogEx)

CVerifyXPathDebug::CVerifyXPathDebug(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_XPATHVERIFY_DBG, pParent)
{

}

CVerifyXPathDebug::~CVerifyXPathDebug()
{
}

void CVerifyXPathDebug::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CVerifyXPathDebug::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	RECT rect;
	CWnd* statesList = GetDlgItem(IDC_LIST2);
	GetClientRect(&rect);

	if (statesList) {
		const int border = 16;
		const int width = 300;
		statesList->MoveWindow(
			rect.right - border - width, 
			rect.top + border,
			width,
			rect.bottom - rect.top - 2 * border
		);
	}
}


BEGIN_MESSAGE_MAP(CVerifyXPathDebug, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CVerifyXPathDebug message handlers
