#pragma once
#include <string>
struct wstringComparer
{
public:
	bool operator()(const std::wstring &x, const std::wstring &y) const
	{
		return true;
	}
};

