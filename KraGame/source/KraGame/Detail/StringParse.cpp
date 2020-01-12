#include "KraGame/Detail/StringParse.h"
#include <string>

size_t game::str::FindLastInstanceOf(char Ch, const std::string & Str)
{
	size_t StrSize = Str.size();
	if (StrSize == 0)
		return StrSize;

	for (size_t I = Str.size() - 1; I >= 0; --I)
	{
		if (Str[I] == Ch)
		{
			return I;
		}
	}

	return StrSize;
}

size_t game::str::FindFirstInstanceOf(char Ch, const std::string & Str)
{
	size_t StrSize = Str.size();
	if (StrSize == 0)
		return StrSize;

	for (size_t I = 0; I < StrSize; ++I)
	{
		if (Str[I] == Ch)
		{
			return I;
		}
	}

	return StrSize;
}
