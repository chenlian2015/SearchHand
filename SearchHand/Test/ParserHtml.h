#pragma once
#include <MsHTML.h>
#include <mshtmcid.h>
#include <mshtmhst.h>
#include <atlbase.h>
#include <vector>
#include <string.h>
#include <set>


struct SearchResItem
{
	std::wstring title;
	std::wstring time;
	std::wstring httplink;
	std::wstring aabstract;
	std::set<std::wstring, > em;
};

//std::vector<SearchResItem> SearchResPage;

class ParserHtml
{
public:
	ParserHtml(void);
	~ParserHtml(void);
	int Parse(const wchar_t * szHtmlDom);

private:
	ATL::CComQIPtr<IHTMLDocument2>    m_pHTMLDocument;
	ATL::CComPtr<IUnknown>            m_punk;

	std::vector<SearchResItem>		  m_pageItems;

public:
	int CreateDom(void);
	int ParseGoogleDom(void);
};

