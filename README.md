XMLTools
--------
This plugin is a small set of useful tools for editing XML with Notepad++. The plugin is based on MSXML. The plugin features are:
- XML syntax Check
- XML Schema (XSD) + DTD Validation
- XML tag autoclose
- Pretty print
- Linarize XML
- Current XML Path
- Conversion XML &amp;lt;-&amp;gt; Text
- Comment / Uncomment
- XPath expression evaluation

Author: Nicolas Crittin

Homepage: [https://github.com/morbac/xmltools](https://github.com/morbac/xmltools)

Plugin Usage
------------
- You can download the latest release from [https://github.com/morbac/xmltools/releases](https://github.com/morbac/xmltools/releases) based on your Operating System Type (32Bit or 64Bit)
- Create a folder XMLTools within Notepad++\Plugins (Typically C:\Program Files\Notepad++\plugins\) and copy the XMLTools.dll there
- Restart Notepad ++. Now you should be able to see the XMLTools menu within the Plugins section 
- If you encounter this issue when building XMLTools:
```
4>C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Microsoft\VC\v170\Microsoft.CppCommon.targets(159,5): error MSB3073: The command "if EXIST "C:\Program Files (x86)\Notepad++\plugins\" ( xcopy /y "C:\xmltools\Win32\Debug\XMLTools.dll" "C:\Program Files (x86)\Notepad++\plugins\XMLTools\" )
```
Please restart Visual Studio in Administrator mode

