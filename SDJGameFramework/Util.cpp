#include "stdafx.h"
#include "Util.h"

size_t GetHash(const std::string& str)
{
	static std::hash<std::string> hash;
	return hash(str);
}
