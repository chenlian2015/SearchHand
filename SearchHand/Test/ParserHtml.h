#pragma once
#include "stltool.h"
#include <MsHTML.h>
#include <mshtmcid.h>
#include <mshtmhst.h>
#include <atlbase.h>
#include <vector>
#include <string.h>
#include <set>
#include <iostream>



struct SearchResItem
{
	std::wstring title;
	std::wstring time;
	std::wstring httplink;
	std::wstring aabstract;
	std::set<std::wstring, wstringComparer> em;
	std::wstring from;//来自哪个搜索引擎
	int length()
	{
		int nLen = title.length()+	time.length() + httplink.length() +	aabstract.length() + from.length();
		for (std::set<std::wstring, wstringComparer>::iterator it=em.begin(); it != em.end(); it++)
		{
			nLen += it->length();
		}
		return nLen;
	}
};

//std::vector<SearchResItem> SearchResPage;

class SearchEngineHtmlParser
{
public:
	static std::wstring GOOGLE;//0
	static std::wstring YAHOO;//1
	static std::wstring BING;//2
	static std::wstring BAIDU;//3

public:
	SearchEngineHtmlParser(void);
	~SearchEngineHtmlParser(void);
	int Parse(const wchar_t * szHtmlDom, const std::wstring &searchEngineName);

	std::vector<SearchResItem>		  m_pageItems;
private:
	ATL::CComQIPtr<IHTMLDocument2>    m_pHTMLDocument;
	ATL::CComPtr<IUnknown>            m_punk;
	std::wstring					  m_currentSearchEngine;


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

