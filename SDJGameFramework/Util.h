#pragma once

size_t GetHash(const std::string& str);

template <typename T>
size_t GetTypeHash();

template <typename T>
std::string GetTypeName();

template<typename T>
inline size_t GetTypeHash()
{
	return GetHash(GetTypeName<T>());
}

template<typename T>
inline std::string GetTypeName()
{
	std::string typeName = typeid(T).name();
	size_t pos = typeName.find(' ');
	return typeName.substr(pos + 1);
}

void LuaStateInitialize(sol::state_view& lua);
