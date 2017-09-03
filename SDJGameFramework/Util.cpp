#include "stdafx.h"
#include "Util.h"
#include "HandleManagers.h"

size_t GetHash(const std::string& str)
{
	static std::hash<std::string> hash;
	return hash(str);
}

void LuaStateInitialize(sol::state_view& lua)
{
	lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);
	lua.create_named_table("objects");
	lua.create_named_table("Component", "prototype", sol::create);
	lua.create_named_table("components");
}