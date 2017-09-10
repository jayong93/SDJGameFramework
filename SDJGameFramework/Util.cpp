#include "stdafx.h"
#include "Util.h"
#include "HandleManagers.h"
#include "Framework.h"

size_t GetHash(const std::string& str)
{
	static std::hash<std::string> hash;
	return hash(str);
}

void LuaStateInitialize(sol::state_view& lua)
{
	lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);
	lua.create_named_table("objects");
	FW.componentTable = sol::table(FW.lua, sol::create);
	FW.componentTable.set("prototype", sol::create);
	lua.create_named_table("components");
}