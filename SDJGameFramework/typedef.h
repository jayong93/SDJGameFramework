#pragma once

template <typename T>
using StringHashMap = std::unordered_map<std::string, T>;
using MessageMap = StringHashMap<std::function<bool(void*, sol::table&)>>;