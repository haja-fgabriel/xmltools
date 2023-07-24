// OptionsDlg.cpp�: fichier d'impl�mentation
//

#include "StdAfx.h"
#include "XMLTools.h"
#include "OptionsDlg.h"
#include "nppMenu.h"
#include "afxdialogex.h"

#include <string>

// Bo�te de dialogue COptionsDlg

IMPLEMENT_DYNAMIC(COptionsDlg, CDialogEx)

COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
  : CDialogEx(COptionsDlg::IDD, pParent)
{
}

COptionsDlg::~COptionsDlg() {
}

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  /*
  DDX_Control(pDX, IDC_EDITPROXYHOST, editProxyHost);
  DDX_Control(pDX, IDC_EDITPROXYPORT, editProxyPort);
  DDX_Control(pDX, IDC_EDITPROXYUSERNAME, editProxyUsername);
  DDX_Control(pDX, IDC_EDITPROXYPASSWORD, editProxyPassword);
  */
  DDX_Control(pDX, IDC_PROPERTIES, m_wndPropList);
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialogEx)
  /*
  ON_BN_CLICKED(IDC_CHKENABLEPROXY, &COptionsDlg::OnBnClickedChkenableproxy)
  */
  ON_BN_CLICKED(IDOK, &COptionsDlg::OnBnClickedOk)
  ON_BN_CLICKED(IDC_BTNVIEWANNOTATION, &COptionsDlg::OnBnClickedBtnviewannotation)
    ON_BN_CLICKED(IDCANCEL, &COptionsDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

void COptionsDlg::UpdateProperty(CMFCPropertyGridProperty* src, enumOptionType type) {
  COleVariant val = src->GetValue();
  DWORD_PTR obj = src->GetData();
  switch (type) {
      case enumOptionType::TYPE_BOOL: {
          (*((bool*)obj)) = (val.boolVal == VARIANT_TRUE);
          break;
      }
      case enumOptionType::TYPE_TRISTATE: {
          if (!wcscmp(val.bstrVal, L"False")) (*((int*)obj)) = 0;
          else if (!wcscmp(val.bstrVal, L"True")) (*((int*)obj)) = 1;
          else (*((int*)obj)) = -1;
          break;
      }
      case enumOptionType::TYPE_INT: {
          (*((int*)obj)) = (val.intVal);
          break;
      }
      case enumOptionType::TYPE_LONG: {
          (*((long*)obj)) = (val.lVal);
          break;
      }
      case enumOptionType::TYPE_WSTRING: {
          if (obj == 0) obj = obj;
          else *((std::wstring*) obj) = val.bstrVal;
          break;
      }
      case enumOptionType::TYPE_WCHAR255: {
          rsize_t len = SysStringLen(val.bstrVal);
          if (len > 255) len = 255;
          memset((wchar_t*)obj, '\0', 255 * sizeof(wchar_t));
          if (len > 0) {
              wcscpy_s((wchar_t*)obj, len, (wchar_t*)bstr_t(val.bstrVal));
          }
          break;
      }
      default: {
          break;
      }
  }
}

BOOL COptionsDlg::OnInitDialog() {
  CMFCPropertyGridProperty* pTmpOption = NULL;
  RECT r;

  CDialogEx::OnInitDialog();

  testAnnotation = false;

  GetClientRect(&r);
  m_wndPropList.SetLeftColumnWidth((r.right - r.left) / 2);
  m_wndPropList.SetVSDotNetLook(TRUE);

  vIntProperties.clear();
  vLongProperties.clear();
  vBoolProperties.clear();
  vTristateProperties.clear();
  vWStringProperties.clear();

  CMFCPropertyGridProperty* pGrpOptions = new CMFCPropertyGridProperty(L"Options");
  m_wndPropList.AddProperty(pGrpOptions);

  pTmpOption = new CMFCPropertyGridProperty(L"Debug level", COleVariant((long)config.dbgLevel, VT_INT), L"0 = TRACE, 1 = INFO, 2 = WARNINGS, 3 = ERRORS", (DWORD_PTR)&config.dbgLevel);
  pGrpOptions->AddSubItem(pTmpOption); vIntProperties.push_back(pTmpOption);

  pTmpOption = new CMFCPropertyGridProperty(L"Error display mode", COleVariant(xmltoolsoptions.errorDisplayMode.c_str()), L"This property let you choose the mode use to report errors.\r\n- Annotation: make errors display inline. Use Annotations style option below to customize the annotations style.\r\n- Dialog: make the error display in a scrollable dialog. All errors are display in a single dialog.\r\n- Alert: display error in a single alert dialog.", (DWORD_PTR)&xmltoolsoptions.errorDisplayMode);
  pTmpOption->AddOption(L"Annotation"); pTmpOption->AddOption(L"Dialog"); pTmpOption->AddOption(L"Alert");
  pGrpOptions->AddSubItem(pTmpOption); vWStringProperties.push_back(pTmpOption);
  
  this->pAnnotationStyleProperty = new CMFCPropertyGridProperty(L"Annotations style", COleVariant((long) xmltoolsoptions.annotationStyle, VT_INT), L"Error messages style when displayed as annotations.\r\nUse \"Annotation preview\" button below to display an annotation sample.", (DWORD_PTR) &xmltoolsoptions.annotationStyle);
  pGrpOptions->AddSubItem(this->pAnnotationStyleProperty); vIntProperties.push_back(this->pAnnotationStyleProperty);
  this->pAnnotationHighlightStyleProperty = new CMFCPropertyGridProperty(L"Highlighted annotations style", COleVariant((long)xmltoolsoptions.annotationHighlightStyle, VT_INT), L"Highlighted error messages style when displayed as annotations.\r\nUse \"Annotation preview\" button below to display an annotation sample.", (DWORD_PTR)&xmltoolsoptions.annotationHighlightStyle);
  pGrpOptions->AddSubItem(this->pAnnotationHighlightStyleProperty); vIntProperties.push_back(this->pAnnotationHighlightStyleProperty);
  pTmpOption = new CMFCPropertyGridProperty(L"Max displayed errors", COleVariant((long)xmltoolsoptions.maxErrorsNum, VT_INT), L"Maximum number of displayed errors. You can allow an infinite number of errors by setting the value `-1`, but be aware that it can slow down the application when XML has many errors.", (DWORD_PTR)&xmltoolsoptions.maxErrorsNum);
  pGrpOptions->AddSubItem(pTmpOption); vIntProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Add node position in XPath", COleVariant((short)(xmltoolsoptions.printXPathIndex ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Additionally shows the nodes position in XPath. When enabled, the XPath of \"<a><b></b><b>Content</b></a>\" will resolve to \"/a/b[2]\" instead of \"/a/b\".", (DWORD_PTR)&xmltoolsoptions.printXPathIndex);
  pGrpOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);


  CMFCPropertyGridProperty* pGrpStatusbar = new CMFCPropertyGridProperty(L"Status bar");
  m_wndPropList.AddProperty(pGrpStatusbar);
  pTmpOption = new CMFCPropertyGridProperty(L"Display current XPath in statusbar", COleVariant((short)(xmltoolsoptions.xpathOnStatusbar ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"When enabled, current XPath is automatically printed in statusbar", (DWORD_PTR)&xmltoolsoptions.xpathOnStatusbar);
  pGrpStatusbar->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Identity attribute names", COleVariant(xmltoolsoptions.identityAttributes.c_str()), L"Specifies a semi-colon separated list of attributes names which could contain identity values. The values of theses attributes are copied into statusbar xpath value. For instance, if attribute \"id\" is referenced and a tag \"<sample id='hello'>\" is present, the status bar will contain \".../sample[id='hello']/...\".", (DWORD_PTR)&xmltoolsoptions.identityAttributes);
  pGrpStatusbar->AddSubItem(pTmpOption); vWStringProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Dump attributes name", COleVariant((short)(xmltoolsoptions.dumpAttributeName ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Introduce the indentity attributes with their name. When enabled, elements are dumped like \".../sample[id='hello']/...\". When disabled, elements are dumped like \".../sample[hello]/...\".", (DWORD_PTR)&xmltoolsoptions.dumpAttributeName);
  pGrpStatusbar->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);

  CMFCPropertyGridProperty* pGrpXml2Txt = new CMFCPropertyGridProperty(L"XML to Text conversion");
  m_wndPropList.AddProperty(pGrpXml2Txt);

  pTmpOption = new CMFCPropertyGridProperty(L"& / &amp;", COleVariant((short)(xmltoolsoptions.convertAmp ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"When enabled, & are converted into &amp; (and reverse) when using XML / Text conversion functions", (DWORD_PTR)&xmltoolsoptions.convertAmp);
  pGrpXml2Txt->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"< / &lt;", COleVariant((short)(xmltoolsoptions.convertLt ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"When enabled, < are converted into &lt; (and reverse) when using XML / Text conversion functions", (DWORD_PTR)&xmltoolsoptions.convertLt);
  pGrpXml2Txt->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"> / &gt;", COleVariant((short)(xmltoolsoptions.convertGt ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"When enabled, > are converted into &gt; (and reverse) when using XML / Text conversion functions", (DWORD_PTR)&xmltoolsoptions.convertGt);
  pGrpXml2Txt->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"\" / &quote;", COleVariant((short)(xmltoolsoptions.convertQuote ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"When enabled, \" are converted into &quote; (and reverse) when using XML / Text conversion functions", (DWORD_PTR)&xmltoolsoptions.convertQuote);
  pGrpXml2Txt->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"' / &apos;", COleVariant((short)(xmltoolsoptions.convertApos ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"When enabled, ' are converted into &apos; (and reverse) when using XML / Text conversion functions", (DWORD_PTR)&xmltoolsoptions.convertApos);
  pGrpXml2Txt->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);


  CMFCPropertyGridProperty* pGrpPrettyPrint = new CMFCPropertyGridProperty(L"Pretty print options");
  m_wndPropList.AddProperty(pGrpPrettyPrint);
  pTmpOption = new CMFCPropertyGridProperty(L"Formating engine", COleVariant(xmltoolsoptions.formatingEngine.c_str()), L"This property let you choose the pretty print and linearize formating engine. Currently, you have choice between:\r\n- SimpleXml: A fast and low memory engine developped by LetMeSleepAlready (https://github.com/LetMeSleepAlready)\r\n- QuickXml: A quick and simple engine which focus on conformity\r\n- StringXml: An engine based on string processing. This engine is more permissive on invalid xml, but very slow on big xml files.", (DWORD_PTR)&xmltoolsoptions.formatingEngine);
  pTmpOption->AddOption(L"SimpleXml"); pTmpOption->AddOption(L"QuickXml"); pTmpOption->AddOption(L"StringXml");
  pGrpPrettyPrint->AddSubItem(pTmpOption); vWStringProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Auto-close tags", COleVariant((short)(xmltoolsoptions.ppAutoclose ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Enable auto-close tags on pretty print. For instance, when enabled, \"<sample></sample>\" is replaced with \"<sample/>\".", (DWORD_PTR)&xmltoolsoptions.ppAutoclose);
  pGrpPrettyPrint->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Ensure conformity", COleVariant((short)(xmltoolsoptions.ensureConformity ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Make the prettyprint/linearize engine maintain XML conformity. Whitespace are trimmed and linebreaks are added only if it does not modify the xml structure. Currently only available on QuickXml.", (DWORD_PTR)&xmltoolsoptions.ensureConformity);
  pGrpPrettyPrint->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Max indent level", COleVariant((long)xmltoolsoptions.maxIndentLevel, VT_INT), L"The max indentation level for pretty print. A zero (0) value means no indentation limit", (DWORD_PTR)&xmltoolsoptions.maxIndentLevel);
  pGrpPrettyPrint->AddSubItem(pTmpOption); vIntProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Apply xml:space=\"preserve\"", COleVariant((short)(xmltoolsoptions.applySpacePreserve ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Make the formating engine take care of xml:space=\"preserve\" and xml:space=\"default\" declarations. Currently only available on QuickXml.", (DWORD_PTR)&xmltoolsoptions.applySpacePreserve);
  pGrpPrettyPrint->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);


  CMFCPropertyGridProperty* pGrpXmlFeatures = new CMFCPropertyGridProperty(L"MSXML Features");
  m_wndPropList.AddProperty(pGrpXmlFeatures);

  pTmpOption = new CMFCPropertyGridProperty(L"Allow document function", (msxmloptions.allowDocumentFunction > 0 ? L"True" : (msxmloptions.allowDocumentFunction == 0 ? L"False" : L"Default")), L"Enables (true) or disables (false) the document function in XLST. Attempts to call the disabled document function will result in an \"Access Denied\" error.\r\nFor more information, see MSXML Security Overview (https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms754611%28v%3dvs.85%29) and DOM Security (https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms761392%28v%3dvs.85%29).", (DWORD_PTR)&msxmloptions.allowDocumentFunction);
  pTmpOption->AddOption(L"Default"); pTmpOption->AddOption(L"True"); pTmpOption->AddOption(L"False"); pTmpOption->AllowEdit(FALSE);
  pGrpXmlFeatures->AddSubItem(pTmpOption); vTristateProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Allow Xslt script", (msxmloptions.allowXsltScript > 0 ? L"True" : (msxmloptions.allowXsltScript == 0 ? L"False" : L"Default")), L"Enables (true) or disables (false) the <msxsl:script> element functionality in XLST. If this property is set to false, attempts to use a stylesheet that uses script will result in a \"Security settings do not allow the execution of script code within this stylesheet\" error.\r\n For more information, see MSXML Security Overview (https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms754611%28v%3dvs.85%29) and DOM Security (https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms761392%28v%3dvs.85%29).", (DWORD_PTR)&msxmloptions.allowXsltScript);
  pTmpOption->AddOption(L"Default"); pTmpOption->AddOption(L"True"); pTmpOption->AddOption(L"False"); pTmpOption->AllowEdit(FALSE);
  pGrpXmlFeatures->AddSubItem(pTmpOption); vTristateProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Force resync", (msxmloptions.forceResync > 0 ? L"True" : (msxmloptions.forceResync == 0 ? L"False" : L"Default")), L"When you load and parse an XML document from a URL that is not local (i.e. is on a different web site), that document may be cached by any number of servers on the line. If you are parsing an XML document that changes often, and it is important that your parser always retrieves the latest version of the document, you can set this property to true. When you set this property to true, it causes the XML document to not be cached; instead it is retrieved from its source server each time that you reparse the document.\r\nThis property is supported in MSXML 3.0 and 6.0. The default value is true.", (DWORD_PTR)&msxmloptions.forceResync);
  pTmpOption->AddOption(L"Default"); pTmpOption->AddOption(L"True"); pTmpOption->AddOption(L"False"); pTmpOption->AllowEdit(FALSE);
  pGrpXmlFeatures->AddSubItem(pTmpOption); vTristateProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Max element depth", COleVariant((long) msxmloptions.maxElementDepth, VT_INT), L"Limits the element depth of an XML document to be loaded into a DOM object. A zero (0) value means no limits on the element depth of an XML document. A non-zero value specifies the maximum depth.\r\nThis property helps to avoid denial of service attacks in which a document is submitted that has excessive element depth.Failure to limit the element depth leaves you open to the exploit where a relatively small XML document can cause denial of service.The MaxElementDepth property mitigates this threat.\r\nIf you are concerned about denial of service, you should set this property to a value that allows you to load your documents yet limits depth to a reasonable level.\r\nThis property is supported in MSXML 3.0 and 6.0.The default value is 0 for 3.0.The default value is 256 for 6.0.\r\n For more information, see MSXML Security Overview (https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms754611%28v%3dvs.85%29) and DOM Security (https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms761392%28v%3dvs.85%29).", (DWORD_PTR)&msxmloptions.maxElementDepth);
  pGrpXmlFeatures->AddSubItem(pTmpOption); vIntProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Max XML size", COleVariant((long)msxmloptions.maxXMLSize, VT_INT), L"Limits the size of an XML document to be loaded into a DOM object. A zero (0) value means no limits on the size of an XML document. A non-zero value specifies the maximum size in blocks of 1024 characters. For ASCII, the block size is one KiloByte (KB).\r\nThis property is supported in MSXML 3.0 and 6.0.The default value is 0.\r\n For more information, see MSXML Security Overview (https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms754611%28v%3dvs.85%29) and DOM Security (https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms761392%28v%3dvs.85%29).", (DWORD_PTR)&msxmloptions.maxXMLSize);
  pGrpXmlFeatures->AddSubItem(pTmpOption); vIntProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Multiple error messages", (msxmloptions.multipleErrorMessages > 0 ? L"True" : (msxmloptions.multipleErrorMessages == 0 ? L"False" : L"Default")), L"Specifies whether or not a ParseError object can return multiple validation errors through the IXMLDOMParseError2 interface. To get all validation errors, set this property to true. Otherwise, only a single validation error is returned.\r\nThis property is supported in MSXML 6.0. The default value is false.", (DWORD_PTR)&msxmloptions.multipleErrorMessages);
  pTmpOption->AddOption(L"Default"); pTmpOption->AddOption(L"True"); pTmpOption->AddOption(L"False"); pTmpOption->AllowEdit(FALSE);
  pGrpXmlFeatures->AddSubItem(pTmpOption); vTristateProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"New parser", (msxmloptions.newParser > 0 ? L"True" : (msxmloptions.newParser == 0 ? L"False" : L"Default")), L"Specifies whether to enable (true) or disable (false) the use of the new parser, which was introduced in MSXML 4.0, to load a DOM document. Setting the NewParser property to false causes subsequent DOM documents to be loaded using the old parser. Setting this property to true causes DOM documents to be loaded using the new parser.\r\nThis property is supported in MSXML 6.0. The default value is false.", (DWORD_PTR)&msxmloptions.newParser);
  pTmpOption->AddOption(L"Default"); pTmpOption->AddOption(L"True"); pTmpOption->AddOption(L"False"); pTmpOption->AllowEdit(FALSE);
  pGrpXmlFeatures->AddSubItem(pTmpOption); vTristateProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Normalize attribute values ", (msxmloptions.normalizeAttributeValues > 0 ? L"True" : (msxmloptions.normalizeAttributeValues == 0 ? L"False" : L"Default")), L"Specifies whether to enable (true) or disable (false) the normalization of attribute values. Setting the NormalizeAttributeValues property to true causes the DOM document to report normalized attribute values, and to serialize per the specification. Setting this property to false causes attribute values to have the existing, non-conformant behavior.\r\nThis property is supported in MSXML 6.0. The default value is false.", (DWORD_PTR)&msxmloptions.normalizeAttributeValues);
  pTmpOption->AddOption(L"Default"); pTmpOption->AddOption(L"True"); pTmpOption->AddOption(L"False"); pTmpOption->AllowEdit(FALSE);
  pGrpXmlFeatures->AddSubItem(pTmpOption); vTristateProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Populate element default values", (msxmloptions.populateElementDefaultValues > 0 ? L"True" : (msxmloptions.populateElementDefaultValues == 0 ? L"False" : L"Default")), L"Enables (true) or disables (false) population of default values when the validate Method1 method is called.\r\nDefault values for elements are created as regular nodes when users call the validate Method1. After a default value node is created, it behaves as any other node.Users can navigate to it, change it, move it, and delete it.When saving with the DOM, this node will be saved.Once a node has been created, consecutive calls to the validate Method1 will do nothing unless the node was deleted.If it has been deleted, a new call to the validate Method1 will repopulate the default value.\r\nThis property is supported in MSXML 6.0. The default value is false.", (DWORD_PTR)&msxmloptions.populateElementDefaultValues);
  pTmpOption->AddOption(L"Default"); pTmpOption->AddOption(L"True"); pTmpOption->AddOption(L"False"); pTmpOption->AllowEdit(FALSE);
  pGrpXmlFeatures->AddSubItem(pTmpOption); vTristateProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Prohibit DTD", (msxmloptions.prohibitDTD > 0 ? L"True" : (msxmloptions.prohibitDTD == 0 ? L"False" : L"Default")), L"Specifies whether to prohibit (true) or allow (false) the inclusion of a DTD in the XML DOM document.\r\nThis property is supported in MSXML 3.0 and 6.0.The default value is false for 3.0.The default value is true for 6.0.\r\nThis property has security implications.\r\nFor more information, see MSXML Security Overview (https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms754611%28v%3dvs.85%29) and DOM Security (https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms761392%28v%3dvs.85%29).", (DWORD_PTR)&msxmloptions.prohibitDTD);
  pTmpOption->AddOption(L"Default"); pTmpOption->AddOption(L"True"); pTmpOption->AddOption(L"False"); pTmpOption->AllowEdit(FALSE);
  pGrpXmlFeatures->AddSubItem(pTmpOption); vTristateProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Resolve externals", (msxmloptions.resolveExternals > 0 ? L"True" : (msxmloptions.resolveExternals == 0 ? L"False" : L"Default")), L"Specifies whether external definitions, resolvable namespaces, external subsets of document type definitions (DTDs), and external entity references are to be resolved at the time the document is parsed.\r\nThis property is supported in MSXML 6.0, and the default value is false.\r\nFor more information, see MSXML Security Overview (https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms754611%28v%3dvs.85%29) and DOM Security (https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms761392%28v%3dvs.85%29).", (DWORD_PTR)&msxmloptions.resolveExternals);
  pTmpOption->AddOption(L"Default"); pTmpOption->AddOption(L"True"); pTmpOption->AddOption(L"False"); pTmpOption->AllowEdit(FALSE);
  pGrpXmlFeatures->AddSubItem(pTmpOption); vTristateProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Selection language", COleVariant(msxmloptions.selectionLanguage.c_str()), L"Used in MSXML 3.0 to specify whether the DOM object should use XPath language (\"XPath\") or the old XSLPattern language (default) as the query language.\r\nThis property is supported in MSXML 3.0 6.0.The default value is \"XSLPattern\" for 3.0.The default value is \"XPath\" for 6.0.", (DWORD_PTR)&msxmloptions.selectionLanguage);
  pGrpXmlFeatures->AddSubItem(pTmpOption); vWStringProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Selection namespaces", COleVariant(msxmloptions.selectionNamespace.c_str()), L"Specifies namespaces for use in XPath expressions when it is necessary to define new namespaces externally. Namespaces are defined in the XML style, as a space-separated list of namespace declaration attributes. You can use this property to set the default namespace as well.\r\nThis property is supported in MSXML 3.0 and 6.0.The default value is \"\".", (DWORD_PTR)&msxmloptions.selectionNamespace);
  pGrpXmlFeatures->AddSubItem(pTmpOption); vWStringProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Server HTTP Request", (msxmloptions.serverHTTPRequest > 0 ? L"True" : (msxmloptions.serverHTTPRequest == 0 ? L"False" : L"Default")), L"Specifies whether to enable (true) or disable (false) the use of the ServerHTTPRequest object in a server application. Setting the property to false causes the DOM object not to use the HTTPRequest object. Setting this property to true causes DOM documents to use ServerHTTPRequest.\r\nThis property is supported in MSXML 3.0 and 6.0.The default value is false.", (DWORD_PTR)&msxmloptions.serverHTTPRequest);
  pTmpOption->AddOption(L"Default"); pTmpOption->AddOption(L"True"); pTmpOption->AddOption(L"False"); pTmpOption->AllowEdit(FALSE);
  pGrpXmlFeatures->AddSubItem(pTmpOption); vTristateProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Use inline schema", (msxmloptions.useInlineSchema > 0 ? L"True" : (msxmloptions.useInlineSchema == 0 ? L"False" : L"Default")), L"Specifies whether inline schemas should be processed (true) or not (false). When this property is set to true, inline schemas are used for validation. When this property is set to false, inline schemas are treated like any other XML fragments.\r\nThis property is supported in MSXML 6.0, and the default value is false.\r\nFor more information, see MSXML Security Overview (https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms754611%28v%3dvs.85%29) and DOM Security (https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms761392%28v%3dvs.85%29).", (DWORD_PTR)&msxmloptions.useInlineSchema);
  pTmpOption->AddOption(L"Default"); pTmpOption->AddOption(L"True"); pTmpOption->AddOption(L"False"); pTmpOption->AllowEdit(FALSE);
  pGrpXmlFeatures->AddSubItem(pTmpOption); vTristateProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Validate on parse", (msxmloptions.validateOnParse > 0 ? L"True" : (msxmloptions.validateOnParse == 0 ? L"False" : L"Default")), L"This property is used to specify whether the XML parser should validate (true) this document against document type definition (DTD), schema, or schema cache on loading.\r\nThis property is supported in MSXML 6.0 and the default value is true.\r\nFor more information, see MSXML Security Overview (https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms754611%28v%3dvs.85%29) and DOM Security (https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms761392%28v%3dvs.85%29).", (DWORD_PTR)&msxmloptions.validateOnParse);
  pTmpOption->AddOption(L"Default"); pTmpOption->AddOption(L"True"); pTmpOption->AddOption(L"False"); pTmpOption->AllowEdit(FALSE);
  pGrpXmlFeatures->AddSubItem(pTmpOption); vTristateProperties.push_back(pTmpOption);
  

  CMFCPropertyGridProperty* pGrpToolbarOptions = new CMFCPropertyGridProperty(L"Toolbar");
  m_wndPropList.AddProperty(pGrpToolbarOptions);
  pTmpOption = new CMFCPropertyGridProperty(L"Enable toolbar icons", COleVariant((short)(xmltoolsoptions.tbEnabled ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Enable toolbar icons. All options below are ignored if toolbar is disabled. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbEnabled);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Check XML syntax now", COleVariant((short)(xmltoolsoptions.tbCheckXML ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Shows the `Check XML syntax now` icon in toolbar. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbCheckXML);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Validate now", COleVariant((short)(xmltoolsoptions.tbValidateXML ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Shows the `Validate now` icon in toolbar. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbValidateXML);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"First error", COleVariant((short)(xmltoolsoptions.tbFirstError ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Shows the `First error` icon in toolbar. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbFirstError);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Previous error", COleVariant((short)(xmltoolsoptions.tbPrevError ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Shows the `Previous error` icon in toolbar. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbPrevError);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Next error", COleVariant((short)(xmltoolsoptions.tbNextError ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Shows the `Next error` icon in toolbar. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbNextError);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Last error", COleVariant((short)(xmltoolsoptions.tbLastError ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Shows the `Last error` icon in toolbar. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbLastError);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Pretty print", COleVariant((short)(xmltoolsoptions.tbPrettyPrint ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Shows the `Pretty print` icon in toolbar. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbPrettyPrint);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Pretty print - indent attributes", COleVariant((short)(xmltoolsoptions.tbPrettyPrintIndentAttr ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Shows the `Pretty print - indent attributes` icon in toolbar. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbPrettyPrintIndentAttr);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Pretty print - indent only", COleVariant((short)(xmltoolsoptions.tbPrettyPrintIndentOnly ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Shows the `Pretty print - indent only` icon in toolbar. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbPrettyPrintIndentOnly);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Linearize", COleVariant((short)(xmltoolsoptions.tbLinearize ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Shows the `Linearize` icon in toolbar. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbLinearize);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Current XML Path", COleVariant((short)(xmltoolsoptions.tbCurrentXMLPath ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Shows the `Current XML Path` icon in toolbar. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbCurrentXMLPath);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Current XML Path with predicates", COleVariant((short)(xmltoolsoptions.tbCurrentXMLPathNS ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Shows the `Current XML Path with predicates` icon in toolbar. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbCurrentXMLPathNS);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Evaluate XPath expression...", COleVariant((short)(xmltoolsoptions.tbEvalXPath ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Shows the `Evaluate XPath expression...` icon in toolbar. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbEvalXPath);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"XSL Transformation...", COleVariant((short)(xmltoolsoptions.tbXSLTransform ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Shows the `XSL Transformation...` icon in toolbar. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbXSLTransform);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Escape characters in selector", COleVariant((short)(xmltoolsoptions.tbEscape ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Shows the `Escape characters in selector` icon in toolbar. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbEscape);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Unescape characters in selector", COleVariant((short)(xmltoolsoptions.tbUnescape ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Shows the `Unescape characters in selector` icon in toolbar. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbUnescape);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Comment selection", COleVariant((short)(xmltoolsoptions.tbComment ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Comment selection` icon in toolbar. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbComment);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Comment selection", COleVariant((short)(xmltoolsoptions.tbUncomment ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Uncomment selection` icon in toolbar. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbUncomment);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  pTmpOption = new CMFCPropertyGridProperty(L"Options...", COleVariant((short)(xmltoolsoptions.tbOptions ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Options...` icon in toolbar. Change will be applied upon NPP restart.", (DWORD_PTR)&xmltoolsoptions.tbOptions);
  pGrpToolbarOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption);
  
  pGrpToolbarOptions->Expand(FALSE);


  CMFCPropertyGridProperty* pGrpProxyOptions = new CMFCPropertyGridProperty(L"Proxy options (disabled)");
  m_wndPropList.AddProperty(pGrpProxyOptions);

  pTmpOption = new CMFCPropertyGridProperty(L"Enabled", COleVariant((short)(proxyoptions.status ? VARIANT_TRUE : VARIANT_FALSE), VT_BOOL), L"Activate proxy", (DWORD_PTR)&proxyoptions.status);
  pGrpProxyOptions->AddSubItem(pTmpOption); vBoolProperties.push_back(pTmpOption); pTmpOption->Enable(0);
  pTmpOption = new CMFCPropertyGridProperty(L"Proxy host", proxyoptions.host.c_str(), 0, (DWORD_PTR)&proxyoptions.host);
  pGrpProxyOptions->AddSubItem(pTmpOption); vWStringProperties.push_back(pTmpOption); pTmpOption->Enable(0);
  pTmpOption = new CMFCPropertyGridProperty(L"Proxy port", COleVariant((long)proxyoptions.port, VT_INT), 0, (DWORD_PTR)&proxyoptions.port);
  pGrpProxyOptions->AddSubItem(pTmpOption); vLongProperties.push_back(pTmpOption); pTmpOption->Enable(0);
  pTmpOption = new CMFCPropertyGridProperty(L"Username", proxyoptions.username.c_str(), 0, (DWORD_PTR)&proxyoptions.username);
  pGrpProxyOptions->AddSubItem(pTmpOption); vWStringProperties.push_back(pTmpOption); pTmpOption->Enable(0);
  pTmpOption = new CMFCPropertyGridProperty(L"Password", proxyoptions.password.c_str(), 0, (DWORD_PTR)&proxyoptions.password);
  pGrpProxyOptions->AddSubItem(pTmpOption); vWStringProperties.push_back(pTmpOption); pTmpOption->Enable(0);

  pGrpProxyOptions->Expand(FALSE);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION�: les pages de propri�t�s OCX devraient retourner FALSE
}

void COptionsDlg::OnBnClickedOk() {
  CStringW buffer;

  /*
  proxyoptions.status = (((CButton*) GetDlgItem(IDC_CHKENABLEPROXY))->GetCheck() == BST_CHECKED);
  
  this->editProxyHost.GetWindowText(buffer);
  wcscpy_s(proxyoptions.host, (const WCHAR *)buffer);

  this->editProxyPort.GetWindowText(buffer);
  proxyoptions.port = _wtoi((LPCTSTR)buffer);

  //this->editProxyUsername.GetWindowText(buffer);
  //wcscpy_s(this->proxyoptions->username, (const WCHAR *)buffer);

  //this->editProxyPassword.GetWindowText(buffer);
  //wcscpy_s(this->proxyoptions->password, (const WCHAR *)buffer);
  */
  
  for (std::vector<CMFCPropertyGridProperty*>::iterator it = vBoolProperties.begin(); it != vBoolProperties.end(); ++it) {
    UpdateProperty(*it, enumOptionType::TYPE_BOOL);
  }
  for (std::vector<CMFCPropertyGridProperty*>::iterator it = vTristateProperties.begin(); it != vTristateProperties.end(); ++it) {
    UpdateProperty(*it, enumOptionType::TYPE_TRISTATE);
  }
  for (std::vector<CMFCPropertyGridProperty*>::iterator it = vIntProperties.begin(); it != vIntProperties.end(); ++it) {
    UpdateProperty(*it, enumOptionType::TYPE_INT);
  }
  for (std::vector<CMFCPropertyGridProperty*>::iterator it = vLongProperties.begin(); it != vLongProperties.end(); ++it) {
    UpdateProperty(*it, enumOptionType::TYPE_LONG);
  }
  for (std::vector<CMFCPropertyGridProperty*>::iterator it = vWStringProperties.begin(); it != vWStringProperties.end(); ++it) {
    UpdateProperty(*it, enumOptionType::TYPE_WSTRING);
  }
  for (std::vector<CMFCPropertyGridProperty*>::iterator it = vWChar255Properties.begin(); it != vWChar255Properties.end(); ++it) {
    UpdateProperty(*it, enumOptionType::TYPE_WCHAR255);
  }

  CDialogEx::OnOK();
}

void COptionsDlg::OnBnClickedBtnviewannotation() {
  std::wstring prevStatus = xmltoolsoptions.errorDisplayMode;
  int prevStyle = xmltoolsoptions.annotationStyle;
  int prevHighlightStyle = xmltoolsoptions.annotationHighlightStyle;

  xmltoolsoptions.errorDisplayMode = L"Annotation";
  xmltoolsoptions.annotationStyle = this->pAnnotationStyleProperty->GetValue().intVal;
  xmltoolsoptions.annotationHighlightStyle = this->pAnnotationHighlightStyleProperty->GetValue().intVal;

  testAnnotation = true;
  clearErrors();
  registerError(displayXMLError(L"This is an annotation example."));
  registerError(displayXMLError(L"This is an highlighted annotation example."));
  
  for (int i = 0; i < 64; ++i) {
      xmltoolsoptions.annotationStyle = i;
      registerError(displayXMLError(Report::str_format(L"Example of annotation style #%d", i)));
  }

  highlightError(1);

  xmltoolsoptions.errorDisplayMode = prevStatus;
  xmltoolsoptions.annotationStyle = prevStyle;
  xmltoolsoptions.annotationHighlightStyle = prevHighlightStyle;
}

BOOL COptionsDlg::OnCommand(WPARAM wParam, LPARAM lParam) {
  clearErrors(NULL, true);

  return CDialogEx::OnCommand(wParam, lParam);
}

void COptionsDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    CDialogEx::OnCancel();
}
