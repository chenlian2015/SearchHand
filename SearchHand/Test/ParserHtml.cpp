#include "StdAfx.h"
#include "ParserHtml.h"


ParserHtml::ParserHtml(void)
{
	
}


ParserHtml::~ParserHtml(void)
{
}

#define COM_R(H,P,R)  if(FAILED(H) || NULL == (P)) return R;

int ParserHtml::Parse(const wchar_t * szHtmlDom)
{
	if (NULL == m_pHTMLDocument)
	{
		CreateDom();
	}

	HRESULT             hr; 

	SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0, 1);
	VARIANT *param;
	hr = SafeArrayAccessData(psa, (LPVOID*)&param);

	param->vt = VT_BSTR;
	param->bstrVal = SysAllocString(szHtmlDom);


	hr = m_pHTMLDocument->write(psa);
	COM_R(hr, m_pHTMLDocument, -1);

	hr = m_pHTMLDocument->close();

	ParseGoogleDom();
	return 0;
}


int ParserHtml::CreateDom(void)
{
	HRESULT             hr; 
	::CoInitialize(NULL);
	hr = ::CoCreateInstance(CLSID_HTMLDocument, NULL, CLSCTX_INPROC, IID_IUnknown, (void**)&m_punk);
	COM_R(hr, m_punk, -1);

	hr = m_punk->QueryInterface(&m_pHTMLDocument);
	COM_R(hr, m_pHTMLDocument, -1);

	return 0;
}

bool checkTag(CComQIPtr<IHTMLElement> &spEle, const std::wstring &className, const std::wstring &tagName)
{
	bool bRet = false;
	
	CComBSTR bstrTagName;
	spEle->get_tagName(&bstrTagName);
	CComBSTR bstrClassName;
	spEle->get_className(&bstrClassName);
	
	std::wstring eleClassName, eleTagName;
	
	if (NULL != bstrTagName)
	{
		eleTagName = bstrTagName;
	}

	if(NULL != bstrClassName)
	{
		eleClassName = bstrClassName;
	}
	
	
	if ((_wcsicmp(eleClassName.c_str(), className.c_str()) ==0) && (_wcsicmp(tagName.c_str(), eleTagName.c_str()) == 0))
	{
		bRet = true;
	}


	return bRet;
}

int GetOneChildElement(const std::wstring& szClassName, const std::wstring& szTagName, CComQIPtr<IHTMLElement> &spEleParent, std::vector<CComQIPtr<IHTMLElement>> &vecSpEleOut, const bool &bAll = false)
{

	CComQIPtr<IDispatch> spDisTmp;
	CComQIPtr<IHTMLElementCollection> spCollect;
	HRESULT hr = S_FALSE;

	hr = spEleParent->get_children(&spDisTmp);
	COM_R(hr, spDisTmp, -1)

		spCollect = spDisTmp;
	COM_R(S_OK, spCollect, -1)

	long len = 0;
	spCollect->get_length(&len);
	
	for (long i=0; i<len; i++)
	{
		CComQIPtr<IDispatch> spDisTmpEle;
		spCollect->item(CComVariant(i),CComVariant(i), &spDisTmpEle);

		if (NULL == spDisTmpEle)
		{
			continue;
		}

		CComQIPtr<IHTMLElement> spEle = spDisTmpEle;

		if(checkTag(spEle, szClassName, szTagName))
		{
			
			vecSpEleOut.push_back(spEle);
			if (!bAll)
			{
				break;
			}
			
		}

	}
	return 0;
}



int ParserHtml::ParseGoogleDom(void)
{
	CComQIPtr<IHTMLDocument3> spDoc3(m_pHTMLDocument);
	if (NULL == spDoc3)
	{
		return -1;
	}
	m_pageItems.clear();
	
	CComBSTR google_items_container_id(L"rso");
	CComQIPtr<IHTMLElement> spgoogle_items_container_ol;
	HRESULT hr = spDoc3->getElementById(google_items_container_id, &spgoogle_items_container_ol);
	COM_R(hr, spgoogle_items_container_ol, -1);

	CComQIPtr<IHTMLElement> spEleDiv_srg;
	 std::vector<CComQIPtr<IHTMLElement>> spEleVec;
	GetOneChildElement(L"srg", L"div", spgoogle_items_container_ol, spEleVec,  false);
	if (spEleVec.size() == 0)
	{
		return -1;
	}
	spEleDiv_srg = spEleVec[0];

	COM_R(hr, spEleDiv_srg, -1);

	//<li class="g"> collention 
	CComQIPtr<IDispatch> spDisRsoChild;
	spEleDiv_srg->get_children(&spDisRsoChild);

	CComQIPtr<IHTMLElementCollection> spDisRsoChildCollection;
	spDisRsoChildCollection = spDisRsoChild;
	long lenDisRsoChildsCnt = 0;
	COM_R(S_OK, spDisRsoChildCollection, -1);

	spDisRsoChildCollection->get_length(&lenDisRsoChildsCnt);
	for (long l=0; l<lenDisRsoChildsCnt; l++)
	{
		//deal one <li class="g">
		SearchResItem sri;

		CComQIPtr<IHTMLElement> spLi_G;
		CComQIPtr<IDispatch> spTmp;
		spDisRsoChildCollection->item(CComVariant(l), CComVariant(l), &spTmp);
		if(NULL == spTmp)
		{
			continue;
		}

		spLi_G = spTmp;
		if(!checkTag(spLi_G, L"g", L"li"))
		{
			continue;
		}


		CComQIPtr<IHTMLElementCollection> spColAll;
		CComQIPtr<IDispatch> spDisTmpX;
		hr = spLi_G->get_all(&spDisTmpX);
		spColAll = spDisTmpX;

		COM_R(hr, spColAll, -1);

		long lenLiGAllChild = 0;
		hr = spColAll->get_length(&lenLiGAllChild);

		CComQIPtr<IHTMLElement> h3R;
		CComQIPtr<IHTMLElement> spanSt;
		for (long y=0; y<lenLiGAllChild; y++)
		{
			CComQIPtr<IHTMLElement> spEleTmp;
			CComQIPtr<IDispatch> spDisTmp;
			spColAll->item(CComVariant(y), CComVariant(y), &spDisTmp);
			if (NULL == spDisTmp)
			{
				continue;
			}
			spEleTmp = spDisTmp;

			if(checkTag(spEleTmp, L"st", L"span"))
			{
				
				spanSt = spEleTmp;
				if (NULL != h3R)
				{
					break;
				}

				continue;
			}

			if(checkTag(spEleTmp, L"r", L"h3"))
			{
				
				h3R = spEleTmp;
				if (NULL != spanSt)
				{
					break;
				}
				continue;
			}

		}

		if (NULL == h3R || NULL == spanSt)
		{
			continue;
		}

		std::wstring strTagName(L"a");
		std::wstring strClassName(L"");
		CComQIPtr<IHTMLElement> spEleA;
		spEleVec.clear();
		GetOneChildElement(strClassName, strTagName, h3R, spEleVec, false);
		if (spEleVec.size()==0)
		{
			continue;
		}
		spEleA = spEleVec[0];

		CComBSTR bstrTitle;
		if (NULL != spEleA)
		{
			CComQIPtr<IHTMLAnchorElement> spAnchor;
			spAnchor = spEleA;

			if (NULL == spAnchor)
			{
				continue;
			}
			
			CComBSTR bstrHref;
			spAnchor->get_href(&bstrHref);

			if (NULL != bstrHref)
			{
				sri.httplink=bstrHref;
			}
			
			spEleA->get_innerText(&bstrTitle);
			if(NULL != bstrTitle)
			{
				sri.title = bstrTitle;
			}

		}


		if (NULL != spanSt)
		{
		
			CComQIPtr<IHTMLElement> spanTime;
			spEleVec.clear();
			GetOneChildElement(L"f", L"span", spanSt, spEleVec, false);
			if (spEleVec.size() == 0)
			{
				continue;
			}
			spanTime = spEleVec[0];

			if (NULL != spanTime)
			{
				CComBSTR time;
				spanTime->get_innerText(&time);
				if (NULL != time)
				{
					sri.time = time;
				}
			}

			CComBSTR bstrContentTxt;
			spanSt->get_innerText(&bstrContentTxt);
			if (NULL != bstrContentTxt)
			{
				sri.aabstract = bstrContentTxt;
			}

			int nPos = sri.aabstract.find(sri.time);
			if (nPos != std::wstring::npos)
			{
				sri.aabstract.erase(nPos, sri.time.length());
			}

			spEleVec.clear();
			GetOneChildElement(L"", L"em", spanSt, spEleVec, true);
			for (std::vector<CComQIPtr<IHTMLElement>>::iterator it =  spEleVec.begin();
				it != spEleVec.end();
				it++)
			{
				CComQIPtr<IHTMLElement> spEm = *it;
				CComBSTR bstrEm;
				spEm->get_innerText(&bstrEm);

				if (NULL != bstrEm)
				{
					
					sri.em.insert(bstrEm.m_str);
				}
			}
			
		}

	}

	return 0;
}
