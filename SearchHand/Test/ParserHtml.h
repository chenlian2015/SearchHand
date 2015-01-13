#pragma once
#include <MsHTML.h>
#include <mshtmcid.h>
#include <mshtmhst.h>
#include <atlbase.h>
#include <vector>
#include <string.h>
#include <set>
#include <iostream>

struct wstringComparer
{
public:
	bool operator()(const std::wstring &x, const std::wstring &y) const
	{
		return true;
	}
};

struct SearchResItem
{
	std::wstring title;
	std::wstring time;
	std::wstring httplink;
	std::wstring aabstract;
	std::set<std::wstring, wstringComparer> em;
};

//std::vector<SearchResItem> SearchResPage;

class ParserHtml
{
public:
	static std::wstring GOOGLE;
	static std::wstring YAHOO;
	static std::wstring BING;
	static std::wstring BAIDU;

public:
	ParserHtml(void);
	~ParserHtml(void);
	int Parse(const wchar_t * szHtmlDom, const std::wstring &searchEngineName);

	std::vector<SearchResItem>		  m_pageItems;
private:
	ATL::CComQIPtr<IHTMLDocument2>    m_pHTMLDocument;
	ATL::CComPtr<IUnknown>            m_punk;
	


private:
	int CreateDom(void);
	
	int ParseGoogleDom(void);
	int ParseYahooDom(void);
	int ParseBingDom(void);

	int GetChild(const std::wstring& szClassName, const std::wstring& szTagName, CComQIPtr<IHTMLElement> &spEleParent, std::vector<CComQIPtr<IHTMLElement>> &vecSpEleOut, const bool &bAll = false);
	bool checkTagClass(CComQIPtr<IHTMLElement> &spEle, const std::wstring &className, const std::wstring &tagName, const std::wstring & target);
	
	
	
public:
	int showForDebug();

};

