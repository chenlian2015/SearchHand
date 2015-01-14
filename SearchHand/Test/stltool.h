#pragma once
#include <string>
struct wstringComparer
{
public:
	bool operator()(const std::wstring &x, const std::wstring &y) const
	{
		return _wcsicmp(x.c_str(), y.c_str()) <0;
	}
};

