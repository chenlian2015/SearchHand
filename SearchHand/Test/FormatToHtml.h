#pragma once
#include <string>
#include <map>

#include "ParserHtml.h"
class FormatToHtml
{
public:
	FormatToHtml(void);
	~FormatToHtml(void);

	static std::string head;
	static std::string tail;
	static std::string itemTemplare;
	static std::string boldTemplate;

	int TransStructToHtml(std::multimap<int, SearchResItem>& m_page, std::string result);

	
};

