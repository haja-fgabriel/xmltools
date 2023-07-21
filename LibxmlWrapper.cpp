#include "LibxmlWrapper.h"
#include "libxml/xpath.h"
#include "libxml/xmlschemas.h"
#include "libxml/xmlunicode.h"


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
