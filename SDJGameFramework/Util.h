#pragma once

size_t GetHash(const std::string& str);

template <typename T>
size_t GetTypeHash();

template<typename T>
inline size_t GetTypeHash()
{
	std::string typeName = typeid(T).name();
	size_t pos = typeName.find(' ');
	std::string subName = typeName.substr(pos+1);
	return GetHash(subName);
}
