#include "stdafx.h"
#include "ComponentManager.h"
#include "MessageManager.h"
#include "Framework.h"
#include "Util.h"

#define MSG_HANDLER(Component, Table) [](void* Component, sol::table& Table) -> bool

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

const StringHashMap<unsigned> Shape::typeMap = Shape::InitTypeMap();

MessageMap Shape::InitMsgMap()
{
	MessageMap map;

	map["CHANGE_SHAPE"] = MSG_HANDLER(c, t)
	{
		Shape& compo = *static_cast<Shape*>(c);

		sol::object type = t["type"];
		if (!type.valid()) return false;

		auto it = Shape::typeMap.find(type.as<std::string>());
		if (it == Shape::typeMap.end()) return false;

		compo.shapeType = Type(it->second);

		sol::object args[4];
		auto tieObj = sol::tie(args[0], args[1], args[2], args[3]);
		switch (it->second)
		{
			case 1:
				tieObj = t.get<sol::object, sol::object, sol::object, sol::object>("size", "nil", "nil", "nil");
				break;
			case 2:
				tieObj = t.get<sol::object, sol::object, sol::object, sol::object>("radius", "slice", "stack", "nil");
				break;
			case 3:
				tieObj = t.get<sol::object, sol::object, sol::object, sol::object>("base", "height", "slice", "stack");
				break;
			case 4:
				tieObj = t.get<sol::object, sol::object, sol::object, sol::object>("innerRadius", "outerRadius", "side", "ring");
				break;
		}
		for (int i = 0; i < 4; ++i)
		{
			if (args[i].valid())
				compo.drawParam[i] = args[i].as<double>();
		}

		return true;
	};

	return map;
}

StringHashMap<unsigned> Shape::InitTypeMap()
{
	std::unordered_map<std::string, unsigned> map;
	const char* typeNames[] = { "cube", "sphere", "cone", "torus", "teapot" };

	int i = 1;
	for (auto n : typeNames)
	{
		map[n] = i++;
	}

	return map;
}

bool LuaComponent::SetScript(const std::string & name)
{
	// 환경을 만드는데 문제가 있음.
	// 아래대로 하면 환경에 함수들이 등록이 안됨
	sol::protected_function fn = FW.lua["Component"]["prototype"][name];
	if (fn.valid())
	{
		auto mt = FW.lua.create_table_with("__index", FW.luaEnv);
		sol::environment env{ FW.lua, sol::create };
		sol::set_environment(env, fn);
		env[sol::metatable_key] = mt;
		FW.lua["Component"]["instance"][uint64_t(handle)] = fn();
	}
	return false;
}
