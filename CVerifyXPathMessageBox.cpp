// CVerifyXPathMessageBox.cpp : implementation file
//

#include "StdAfx.h"
#include "XMLTools.h"
#include "afxdialogex.h"
#include "CVerifyXPathMessageBox.h"


// CVerifyXPathMessageBox dialog

IMPLEMENT_DYNAMIC(CVerifyXPathMessageBox, CDialogEx)

CVerifyXPathMessageBox::CVerifyXPathMessageBox(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_XPATHVERIFY_LOG, pParent)
{

}

CVerifyXPathMessageBox::~CVerifyXPathMessageBox()
{
}

void CVerifyXPathMessageBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	CListCtrl* listresults = (CListCtrl*)this->GetDlgItem(IDC_LIST_XPATHV_MESSAGES);
	listresults->DeleteAllItems();

}

BOOL CVerifyXPathMessageBox::OnInitDialog()
{
	CDialog::OnInitDialog();
	CListCtrl* listresults = (CListCtrl*)this->GetDlgItem(IDC_LIST_XPATHV_MESSAGES);

	listresults->InsertColumn(0, L"Context", LVCFMT_LEFT, 100);
	listresults->InsertColumn(1, L"Level", LVCFMT_LEFT, 100);
	listresults->InsertColumn(2, L"Message", LVCFMT_LEFT, 470);
	listresults->DeleteAllItems();

	for (auto& err : m_listErrors) {
		/* wtf                                             */
		AddItem(listresults, std::get<0>(err), std::get<1>(err), std::get<2>(err));
	}

	return FALSE;
}


void CVerifyXPathMessageBox::AddItem(CListCtrl* list, CStringW context, CStringW level, CStringW message)
{
	int idx = list->GetItemCount();
	list->InsertItem(idx, context);
	list->SetItemText(idx, 1, level);
	list->SetItemText(idx, 2, message);
}

void CVerifyXPathMessageBox::AddAll(const std::vector<ErrorEntryType>& errors)
{
	m_listErrors.clear();
	m_listErrors.resize(errors.size());
	std::transform(errors.begin(), errors.end(), m_listErrors.begin(), [&](const ErrorEntryType& err) {
		const CStringW context = L"todo_extractFromStruct";
		const CStringW level = L"todo_extractFromStruct";
		CStringW reason = CStringW(err.reason.c_str(), err.reason.size());
		return std::make_tuple(context, level, reason);
	});
}	

void CVerifyXPathMessageBox::ClearList()
{
	m_listErrors.clear();
}

BEGIN_MESSAGE_MAP(CVerifyXPathMessageBox, CDialogEx)
	ON_BN_CLICKED(IDC_LIST_XPATHV_CLEAR, &CVerifyXPathMessageBox::OnBnClickedListXpathvClear)
END_MESSAGE_MAP()


// CVerifyXPathMessageBox message handlers


void CVerifyXPathMessageBox::OnBnClickedListXpathvClear()
{
	CListCtrl* listresults = (CListCtrl*)this->GetDlgItem(IDC_LIST_XPATHV_MESSAGES);
	listresults->DeleteAllItems();
}
