#include "LibxmlWrapper.h"
#include "libxml/xpath.h"
#include "libxml/xmlschemas.h"
#include "libxml/xmlunicode.h"
#include <varargs.h>
#include <string>
#include <locale>
#include <codecvt>
#include <memory>


static xmlDocPtr CreateAndInitDOM(const std::string& filepath) {
    return nullptr;
}

LibxmlWrapper::LibxmlWrapper(const char* xml, size_t size): content(xml), filepathLength(size)
{
}

LibxmlWrapper::~LibxmlWrapper()
{
}

int LibxmlWrapper::getCapabilities()
{
    return XmlCapabilityType::GET_ERROR_DETAILS;
}

bool LibxmlWrapper::checkSyntax()
{
    throw std::exception("Not implemented yet");
    return false;
}

bool LibxmlWrapper::checkValidity(std::wstring schemaFilename, std::wstring validationNamespace)
{
    throw std::exception("Not implemented yet");
    return false;
}

std::vector<XPathResultEntryType> LibxmlWrapper::xpathEvaluate(std::wstring xpath, std::wstring ns)
{
    std::vector<XPathResultEntryType> result;
    xmlDocPtr doc = xmlParseDoc((xmlChar*)this->content.c_str());
    xmlXPathContextPtr context = xmlXPathNewContext(doc);

    // TODO convert XPath to UTF-8
    const xmlChar* xpathEncoded = nullptr;

    // xmlXPathObjectPtr result = xmlXPathEvalExpression(xpathEncoded, context);

    // TODO extract result


CleanUp:
    xmlXPathFreeContext(context);
    xmlFreeDoc(doc);

    return std::vector<XPathResultEntryType>();
}

bool LibxmlWrapper::xslTransform(std::wstring xslfile, XSLTransformResultType* out, std::wstring options, UniMode srcEncoding)
{
    throw std::exception("Not implemented yet");
    return false;
}

void LibxmlWrapper::addError(LoggingEntryType error)
{
    errors.push_back(error);
}

// I have trauma with defensive programming in C
#define SAFE_ATTRIB(var, expr) do {var = (expr); if (var == nullptr) { goto CleanUp; } } while(0);

std::wstring getLibxmlErrorLevelStr(xmlErrorPtr error)
{
    std::wstring level;

    switch (error->level) {
    case XML_ERR_WARNING:
        level = L"Warning";
        break;
    case XML_ERR_ERROR:
        level = L"Error";
        break;
    case XML_ERR_FATAL:
        level = L"Fatal";
        break;
    default:
        level = L"";
    }
    
    return level;
}

std::wstring getLibxmlErrorMessage(xmlErrorPtr error)
{
    std::wstring message;
    wchar_t* lpwszMessage = Report::castChar(error->message, UniMode::uniUTF8);
    if (lpwszMessage) {
        message.assign(lpwszMessage);
    }
    else {
        message = L"Unknown";
    }
    return message;
}

extern "C" void __myCallback(void* data, xmlErrorPtr error)
{
    LibxmlWrapper* wrapper = (LibxmlWrapper*)data;
    std::wstring context = L"";
    std::wstring level = getLibxmlErrorLevelStr(error);
    std::wstring message = getLibxmlErrorMessage(error);
    wrapper->addError({context, level, message});
}

int LibxmlWrapper::isXPathValidOnSchema(LPCWSTR schemaFilepath, int filepathLength, LPCWSTR xpath, int xpathLength) 
{
    int retVal = false;
    xmlDocPtr doc = nullptr;
    char* filePathu8 = nullptr;
    const char* encoding = "UTF-8";
    int flags = 0;
    char* xpathu8 = nullptr;

    // Convert the XPath query from UTF-16 encoding to UTF-8
    SAFE_ATTRIB(xpathu8, new char[xpathLength + 1]);
    Report::UTF8FromUCS2(xpath, xpathLength, xpathu8, xpathLength);

    // Convert the path to the schema (XSD) file from UTF-16 encoding to UTF-8
    SAFE_ATTRIB(filePathu8, new char[filepathLength + 1]);
    Report::UTF8FromUCS2(schemaFilepath, filepathLength, filePathu8, filepathLength);

    // Read the content of the XML schema file and parse it as XML
    SAFE_ATTRIB(doc, xmlReadFile(filePathu8, encoding, flags));

    // TODO return -1 for allocation errors
    retVal = this->isXPathValidOnSchema(doc, xpathu8);

 CleanUp:
    if (doc) xmlFreeDoc(doc);
    if (filePathu8) delete[] filePathu8;
    if (xpathu8) delete[] xpathu8;
    return retVal;
}

int LibxmlWrapper::isXPathValidOnSchema(LPCWSTR xpath, int xpathLength) {
    // Verifies the XPath query on the schema provided as the XML content in the Notepad++ tab
    xmlDocPtr doc = nullptr;
    int retVal = 0;
    char* xpathu8 = nullptr;
    const char* encoding = "UTF-8";
    int flags = 0;

    // Convert the XPath query from UTF-16 encoding to UTF-8
    SAFE_ATTRIB(xpathu8, new char[xpathLength + 1]);
    Report::UTF8FromUCS2(xpath, xpathLength, xpathu8, xpathLength);

    // Parse the current content as an XML document
    SAFE_ATTRIB(doc, xmlReadDoc((xmlChar*)this->content.c_str(), nullptr, encoding, flags));

    // TODO return -1 for allocation errors
    retVal = this->isXPathValidOnSchema(doc, xpathu8);

CleanUp:
    if (doc) xmlFreeDoc(doc);
    if (xpathu8) delete[] xpathu8;

    return retVal;
}

int LibxmlWrapper::isXPathValidOnSchema(xmlDocPtr doc, const char* xpath) {
    // TODO pick a different encoding for reading documents
    // TODO return -1 for allocation errors
    xmlSchemaParserCtxtPtr schemaParser = nullptr;
    xmlSchemaPtr schema = nullptr;
    xmlSchemaValidCtxtPtr validSchemaCtxt = nullptr;
    xmlSchemaVerifyXPathCtxtPtr verifyCtxt = nullptr;
    const char* encoding = "UTF-8";
    int retVal = 0;
    Report::clearLog();

    SAFE_ATTRIB(schemaParser, xmlSchemaNewDocParserCtxt(doc));
    xmlSchemaSetParserErrors(schemaParser, Report::registerError, Report::registerWarn, nullptr);

    SAFE_ATTRIB(schema, xmlSchemaParse(schemaParser));

    SAFE_ATTRIB(validSchemaCtxt, xmlSchemaNewValidCtxt(schema));
    xmlSchemaSetValidErrors(validSchemaCtxt, Report::registerError, Report::registerWarn, nullptr);

    SAFE_ATTRIB(verifyCtxt, xmlSchemaNewVerifyXPathCtxt(validSchemaCtxt, (xmlChar*)xpath));
    xmlSchemaSetVerifyXPathErrors(verifyCtxt, Report::registerError, Report::registerWarn, nullptr);
    xmlSetStructuredErrorFunc(this, __myCallback);

    retVal = xmlSchemaVerifyXPath(verifyCtxt);

CleanUp:
    if (Report::getLog().length()) {
        Report::_printf_err(Report::getLog());
    }

    if (verifyCtxt) xmlSchemaFreeVerifyXPathCtxt(verifyCtxt);
    if (validSchemaCtxt) xmlSchemaFreeValidCtxt(validSchemaCtxt);
    if (schema) xmlSchemaFree(schema);
    if (schemaParser) xmlSchemaFreeParserCtxt(schemaParser);

    return retVal;
}

bool LibxmlWrapper::isValidSchema()
{
    // Verify if the currently loaded XML file is a valid schema
    // TODO pick a different encoding for reading documents
    // TODO return -1 for allocation errors

    xmlDocPtr doc = nullptr;
    bool isValid = false;
    const char* encoding = "UTF-8";

    SAFE_ATTRIB(doc, xmlReadDoc((xmlChar*)this->content.c_str(), NULL, encoding, 0));
    isValid = this->isValidSchema(doc);

CleanUp:
    if (doc) xmlFreeDoc(doc);
    return isValid;
}

bool LibxmlWrapper::isValidSchema(LPCWSTR filePath, int filepathLength) {
    // Verify if the currently loaded XML file is a valid schema
    // TODO pick a different encoding for reading documents
    // TODO return -1 for allocation errors

    char* filePathu8 = nullptr;
    xmlDocPtr doc = nullptr;
    bool isValid = false;
    const char* encoding = "UTF-8";
    int xmlReadFlags = 0;
    
    SAFE_ATTRIB(filePathu8, new char[filepathLength + 1]);
    memset(filePathu8, '\0', sizeof(char) * (filepathLength + 1));
    Report::UTF8FromUCS2(filePath, filepathLength, filePathu8, filepathLength);

    SAFE_ATTRIB(doc, xmlReadFile(filePathu8, encoding, xmlReadFlags));

    isValid = this->isValidSchema(doc);

CleanUp:
    if (doc) xmlFreeDoc(doc);
    if (filePathu8) delete[] filePathu8;
    return isValid;
}

bool LibxmlWrapper::isValidSchema(xmlDocPtr doc) {
    // Verify if the currently loaded XML file is a valid schema

    // TODO pick a different encoding for reading documents
    // TODO return -1 for allocation errors
    xmlSchemaParserCtxtPtr schemaParser = nullptr;
    xmlSchemaPtr schema = nullptr;
    xmlSchemaValidCtxtPtr validSchemaCtxt = nullptr;
    const char* encoding = "UTF-8";
    bool isValid = false;
    Report::clearLog();
    
    SAFE_ATTRIB(schemaParser, xmlSchemaNewDocParserCtxt(doc));

    xmlSchemaSetParserErrors(schemaParser, Report::registerError, Report::registerWarn, nullptr);
    SAFE_ATTRIB(schema, xmlSchemaParse(schemaParser));

    xmlSchemaSetValidErrors(validSchemaCtxt, Report::registerError, Report::registerWarn, nullptr);
    SAFE_ATTRIB(validSchemaCtxt, xmlSchemaNewValidCtxt(schema));

    isValid = xmlSchemaIsValid(validSchemaCtxt);

CleanUp:
    if (Report::getLog().length()) {
        Report::_printf_err(Report::getLog());
    }

    if (validSchemaCtxt) xmlSchemaFreeValidCtxt(validSchemaCtxt);
    if (schema) xmlSchemaFree(schema);
    if (schemaParser) xmlSchemaFreeParserCtxt(schemaParser);

    return isValid;

}