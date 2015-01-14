#include "StdAfx.h"
#include "FormatToHtml.h"
#include "StringProcess.h"
#include <fstream>


std::string FormatToHtml::head = "<html><head>\
	<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\
	\
	<style>\
	<!--\
	/* Style Definitions */\
	p.MsoNormal, li.MsoNormal, div.MsoNormal\
{mso-style-unhide:no;\
mso-style-qformat:yes;\
mso-style-parent:\"\";\
margin:0cm;\
margin-bottom:.0001pt;\
text-align:justify;\
text-justify:inter-ideograph;\
mso-pagination:none;\
font-size:10.5pt;\
-->\
	</style>\
	\
	</head>\
	\
	<body  link=\"blue\" vlink=\"#954F72\" style=\"tab-interval:21.0pt;\
	text-justify-trim:punctuation\">\
	<div style=\"width:40%;margin-left:8%;\">";

std::string FormatToHtml::tail = "</div></body></html>";

std::string FormatToHtml::itemTemplare = "<div style=\"margin-top:15%;\">\
	<p class=\"MsoNormal\" align=\"left\" style=\"margin-left:18.0pt;text-align:left;\
	text-indent:-18.0pt;mso-pagination:widow-orphan;mso-outline-level:3;mso-list:\
l0 level1 lfo1;tab-stops:list 36.0pt;\"><span lang=\"EN-US\" style=\"font-size:13.5pt;font-family:Arial,sans-serif;mso-fareast-font-family:\
	arial,sans-serif;color:#222222;mso-font-kerning:0pt\"><a href=\"%s\"><span style=\"color:#660099;\
	text-decoration:none;text-underline:none\">%s</span></a></span></p>\
	\
	<p class=\"MsoNormal\" align=\"left\" style=\"text-align:left;line-height:12.0pt;\
	mso-pagination:widow-orphan;\"><span lang=\"EN-US\" style=\"mso-bidi-font-size:10.5pt;font-family:Arial,sans-serif;mso-fareast-font-family:\
	arial,sans-serif;color:#006621;mso-font-kerning:0pt\">%s</span></p>\
	\
	\
	<p class=\"MsoNormal\" align=\"left\" style=\"text-align:left;line-height:13.5pt;\
	mso-pagination:widow-orphan;\"><span lang=\"EN-US\" style=\"font-size:12.0pt;font-family:Arial,sans-serif;mso-fareast-font-family:\
	arial,sans-serif;color:#545454;mso-font-kerning:0pt\">%s</span></p>\
	</div>";


FormatToHtml::FormatToHtml(void)
{
}


FormatToHtml::~FormatToHtml(void)
{
}


int FormatToHtml::TransStructToHtml( std::multimap<int, SearchResItem>& m_page, std::string strHtml )
{
	std::multimap<int, SearchResItem> page = m_page;


	strHtml = head;

	CHAR *szTmp = NULL;
	int nLen = 0;
	for (std::multimap<int, SearchResItem>::iterator it = page.begin(); it != page.end(); it++)
	{
		
		for (std::set<std::wstring, wstringComparer>::iterator item = it->second.em.begin(); item != it->second.em.end(); item++)
		{
			std::wstring strBold=L"<font color=\"red\">";
			strBold.append(*item);
			strBold.append(L"</font>");

			it->second.aabstract = ReplaceString(it->second.aabstract, *item, strBold);
		}

		setlocale(LC_ALL, "en-US");
		std::string httplink,title,aabstract;
		WideCharToMultiByteCP(CP_UTF8, it->second.httplink.c_str(),httplink);
		WideCharToMultiByteCP(CP_UTF8, it->second.title.c_str(), title);
		WideCharToMultiByteCP(CP_UTF8, it->second.aabstract.c_str(), aabstract);

		int nLenCur = itemTemplare.length() + httplink.length()*2 + title.length() + aabstract.length();

		if (NULL == szTmp)
		{
			nLen = nLenCur+1024;
			szTmp = new CHAR[nLen];
		}

		if (NULL != szTmp  && (nLenCur > nLen))
		{
			delete []szTmp;
			szTmp = NULL;
			nLen = nLenCur;
			szTmp = new CHAR[nLen];
		}

		sprintf(szTmp, "%s", httplink.c_str());
		sprintf(szTmp, "%s", title.c_str());
		sprintf(szTmp, "%s", aabstract.c_str());
		sprintf(szTmp, itemTemplare.c_str(), httplink.c_str(), title.c_str(), httplink.c_str(), aabstract.c_str());
		strHtml +=szTmp;

	}


	if (NULL != szTmp)
	{
		delete []szTmp;
		szTmp = NULL;
	}

	strHtml += tail;
	
	//DEBUG
	{
		FILE * fp = fopen("E:\\TMP\\Xy.html", "w+");
		if (NULL != fp)
		{
			std::string strResult;

			fwrite(strHtml.c_str(), 1, strHtml.length(), fp);
			fclose(fp);
		}
	}
	
	
	return 0;
}


