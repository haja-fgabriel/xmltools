#pragma once

#include <atlstr.h>


///////////////////////////////////////////////////////////////////////////////
// Texts used throughout the IDD_XPATHVERIFY dialog resource
///////////////////////////////////////////////////////////////////////////////
static LPCWSTR gHowToUseWithFile = _T("An user-provided file will be used for "
	"verifying the XPath query for satisfiability.");
static LPCWSTR gHowToUseWithoutFile = _T("The text in the currently opened tab"
	" will be used for verifying the XPath query for satisfiability.");
static LPCWSTR gDescription = _T("The satisfiability of an XPath query "
	"represents whether exists any XML document valid on the given schema "
	"(XSD) that will give a non-empty result on running the query against it.");
static LPCTSTR gCaption = _T("XPath satisfiability check");
static LPCTSTR gTextValid = _T("The given XPath query is satisfiable on the schema.");
static LPCTSTR gTextInvalid = _T("The given XPath query is not "
	"satisfiable on the schema.");
static LPCTSTR gTextError = _T("One or more errors have occured when checking the "
	"satisfiability of the given XPath query. Click OK to view more details.");
static LPCTSTR gTextUnknown = _T("Unknown status code returned by "
	"libxmlWrapper->isXPathValidOnSchema.");


///////////////////////////////////////////////////////////////////////////////
// Error messages when running the XPath satisfiability test
///////////////////////////////////////////////////////////////////////////////
static LPCTSTR gTextInvalidSchema = _T("The content inside the tab is not a valid "
	"XML schema. Please provide a valid schema in the tab content "
	"or choose a different file.");
static LPCTSTR gTextTabContentError = _T("Could not retrieve the content of the "
	"currently opened tab when verifying if it's a valid "
	"XML schema.");
static LPCTSTR gTextMemoryError = _T("Could not allocate memory for "
	"verifying if the schema is valid.");
static LPCTSTR gTextInvalidSchemaFromFileError = _T("The given file is not existing or it "
	"is not a valid XML schema.");
static LPCTSTR gTextLibxmlWrapperAllocError = _T("Could not allocate memory for verifying "
	"if the schema is valid.");