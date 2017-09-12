#include "stdafx.h"
#include "HandleManagers.h"
#include "MessageManager.h"
#include "Framework.h"
#include "Util.h"

Component::~Component() {}

void Component::SendMsg(sol::object& args)
{
	auto msgMap = MM.GetMsgMap(CM.Type(handle));
	if (msgMap)
	{
		auto& msg = args.as<sol::table>();
		if (msg[1].get_type() == sol::type::string)
		{
			auto it = msgMap->find(msg[1].get<std::string>());
			if (it != msgMap->end())
			{
				it->second(this, msg);
			}
		}
	}
}

void Component::RegisterInLua()
{
	FW.lua.new_usertype<Component>("Component",
		"new", sol::no_constructor,
		"get", [](uint64_t id) -> sol::object {
		if (CM.IsLuaComponent(id))
		{
			auto compo = CM.GetBy<LuaComponent>(id);
			return compo->env;
		}

		auto typeName = CM.TypeName(id);
		sol::table type = FW.typeTable[typeName].get<sol::table>();
		sol::protected_function getFn = type["get"].get<sol::protected_function>();
		return getFn(id).get<sol::object>();
	}
		);
}

LuaComponent::~LuaComponent()
{
}

bool LuaComponent::SetScript(const std::string & name)
{
	auto& lua = FW.lua;
	sol::protected_function fn = FW.luaScriptTable[name];
	if (fn.valid())
	{
		env = sol::environment{ lua, sol::create, lua.globals() };
		sol::set_environment(env, fn); fn();
		env["owner_id"] = owner.ToUInt64();
		env["id"] = handle.ToUInt64();
		scriptName = name;
		return true;
	}
	return false;
}

void LuaComponent::SendMsg(sol::object & args)
{
	auto& lua = FW.lua;
	sol::protected_function msgHandler = env["SendMsg"];
	if (msgHandler.valid())
	{
		msgHandler(args);
	}
}
