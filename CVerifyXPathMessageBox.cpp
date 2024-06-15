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

	for (auto& err : m_listErrors) {
                             /* wtf                                             */
		AddItem(listresults, std::get<0>(err), std::get<1>(err), std::get<2>(err));
	}
}

BOOL CVerifyXPathMessageBox::OnInitDialog()
{
	CDialog::OnInitDialog();
	CListCtrl* listresults = (CListCtrl*)this->GetDlgItem(IDC_LIST_XPATHV_MESSAGES);

	listresults->InsertColumn(0, L"Context", LVCFMT_LEFT, 400);
	listresults->InsertColumn(1, L"Level", LVCFMT_LEFT, 400);
	listresults->InsertColumn(2, L"Message", LVCFMT_LEFT, 470);
	listresults->DeleteAllItems();

	return FALSE;
}


void CVerifyXPathMessageBox::AddItem(CListCtrl* list, CStringW context, CStringW level, CStringW message)
{
	int idx = list->GetItemCount();
	BOOL isDone = FALSE;
	BOOL ret1, ret2, ret3;
	BOOL bIsVisible = FALSE;
	BOOL bIsEnsuredVisible = FALSE;
	int err1, err2, err3;
	ret1 = list->InsertItem(idx, context);
	bIsEnsuredVisible = list->EnsureVisible(idx, TRUE);

	CRect rect;
	CRect itemRect;
	DWORD dwStyle = list->GetStyle();

	list->GetClientRect(&rect);
	list->GetItemRect(idx, &itemRect, LVIR_BOUNDS);
	
	bool bIsClientTooSmall = (rect.Width() <= 0 || rect.Height() <= 0);
	bool bIsItemInRect = (rect.PtInRect(itemRect.TopLeft()) && rect.PtInRect(itemRect.BottomRight()));
	bool bIsInvalidStyle = ((dwStyle & LVS_TYPEMASK) != LVS_REPORT && (dwStyle & LVS_TYPEMASK) != LVS_LIST);

	bIsVisible = list->IsItemVisible(idx);
	err1 = GetLastError();
	//ret2 = list->SetItemText(idx, 1, level);
	err2 = GetLastError();
	//ret3 = list->SetItemText(idx, 2, message);
	err3 = GetLastError();
	int itemCount = list->GetItemCount();
	// list->Update(idx);
	return;
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
