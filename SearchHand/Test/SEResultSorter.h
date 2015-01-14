#pragma once

#include "ParserHtml.h"
//SearchEngine Result Sort
#include "stltool.h"
#include <map>
#include <set>
class SEResultSorter
{
public:
	SEResultSorter(void);
	~SEResultSorter(void);


	std::multimap<int, SearchResItem> m_page;
	std::set<std::wstring, wstringComparer> httpLink;

public:

	int AddData(std::vector<SearchResItem> & data)
	{
		for (std::vector<SearchResItem>::iterator it = data.begin(); it != data.end(); it++)
		{
			
			if(httpLink.find(it->httplink) != httpLink.end())
			{
				continue;
			}

			m_page.insert(std::pair<int, SearchResItem>(-it->em.size(), *it));
		}
		
		return 0;
	}

	//�Ѿ����򣬲���Ҫ������,��Ԥ��������ӿ����ɣ���Ϊ�Ժ����򷽷����ܻ�ı�
	int sort()
	{
		return 0;
	}


};

