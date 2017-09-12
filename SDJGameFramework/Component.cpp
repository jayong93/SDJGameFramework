#include "stdafx.h"
#include "HandleManagers.h"
#include "MessageManager.h"
#include "Framework.h"
#include "Util.h"
#include "LuaInterfaceType.h"

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

LuaComponent::~LuaComponent()
{
	sol::state_view lua{ FW.lua };
	sol::table table = lua["components"].get<sol::table>();
	if (table.valid())
		table[handle.ToUInt64()] = sol::nil;
}

bool LuaComponent::SetScript(const std::string & name)
{
	auto& lua = FW.lua;
	sol::protected_function fn = FW.componentTable["prototype"][name];
	if (fn.valid())
	{
		env = sol::environment{ lua, sol::create, lua.globals() };
		sol::set_environment(env, fn); fn();
		auto& ownerName = OM.Get(owner)->name;
		env["owner"] = lua["objects"][ownerName];
		env["handle"] = handle.ToUInt64();
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
