#pragma once

#include "XmlWrapperInterface.h"
#include <string>

class LibxmlWrapper : public XmlWrapperInterface
{
	std::string content;
	size_t filepathLength;

public:
	LibxmlWrapper(const char* xml, size_t size);
	~LibxmlWrapper();

	int getCapabilities();
	bool checkSyntax();
	bool checkValidity(std::wstring schemaFilename = L"", std::wstring validationNamespace = L"");
	std::vector<XPathResultEntryType> xpathEvaluate(std::wstring xpath, std::wstring ns = L"");
	bool xslTransform(std::wstring xslfile, XSLTransformResultType* out, std::wstring options = L"", UniMode srcEncoding = UniMode::uniEnd);
	bool isXPathValidOnSchema(const std::wstring& schemaFilepath, const std::string& xpath);
};

