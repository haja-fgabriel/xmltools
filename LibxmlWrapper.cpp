#include "LibxmlWrapper.h"
#include "libxml/xpath.h"
#include "libxml/xmlschemas.h"
#include "libxml/xmlunicode.h"
#include <varargs.h>
#include <string>
#include <locale>
#include <codecvt>


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
    throw std::exception("Not implemented yet");
    return 0;
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

bool LibxmlWrapper::isXPathValidOnSchema(const std::wstring& schemaFilepath, const std::string& xpath) 
{
    // TODO implement
    // xmlSchemaParserCtxtPtr parserCtxt = xmlSchemaNewParserCtxt()
    //xmlSchemaParse()
    return false;
}

bool LibxmlWrapper::isXPathValidOnSchema(const std::string& xpath) {
    // Verifies the XPath query on the schema provided as the XML content in the Notepad++ tab
    return false;
}

// I have trauma with defensive programming in C
#define SAFE_ATTRIB(var, expr) do {var = (expr); if (var == nullptr) goto CleanUp; } while(0);

bool LibxmlWrapper::isValidSchema()
{
    // Verify if the currently loaded XML file is a valid schema
    // TODO pick a different encoding for reading documents
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

    xmlDocPtr doc = nullptr;
    bool isValid = false;
    const char* encoding = "UTF-8";
    
    std::wstring filePathu16 = { filePath };
    std::string filePathu8(filePathu16.begin(), filePathu16.end());
    SAFE_ATTRIB(doc, xmlReadFile(filePathu8.c_str(), encoding, 0));

    isValid = this->isValidSchema(doc);

CleanUp:
    if (doc) xmlFreeDoc(doc);
    return isValid;
}

bool LibxmlWrapper::isValidSchema(xmlDocPtr doc) {
    // Verify if the currently loaded XML file is a valid schema

    // TODO pick a different encoding for reading documents
    xmlSchemaParserCtxtPtr schemaParser = nullptr;
    xmlSchemaPtr schema = nullptr;
    xmlSchemaValidCtxtPtr validSchemaCtxt = nullptr;
    const char* encoding = "UTF-8";
    bool isValid = false;
    
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