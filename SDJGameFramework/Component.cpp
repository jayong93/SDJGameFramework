#include "stdafx.h"
#include "HandleManagers.h"
#include "MessageManager.h"
#include "Framework.h"
#include "Util.h"
#include "LuaInterfaceType.h"

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
const char* Shape::typeNames[] = { "none", "cube", "sphere", "cone", "torus", "teapot" };

void Shape::RegisterInLua()
{
	auto GetType = [](ComponentInLua& c) { auto& s = *CM.GetBy<Shape>(c.handle); return s.typeNames[s.shapeType]; };
	auto SetType = [](ComponentInLua& c, const std::string& t) {auto& s = *CM.GetBy<Shape>(c.handle); s.shapeType = Shape::Type(Shape::typeMap.at(t)); };

	std::function<float(ComponentInLua&)> GetParam[4];
	for (int i = 0; i < 4; ++i)
		GetParam[i] = [i](ComponentInLua& c) {auto& s = *CM.GetBy<Shape>(c.handle); return s.param[i]; };
	std::function<void(ComponentInLua&, float)> SetParam[4];
	for (int i = 0; i < 4; ++i)
		SetParam[i] = [i](ComponentInLua& c, float v) {auto& s = *CM.GetBy<Shape>(c.handle); s.param[i] = v; };

	auto GetColor = [](ComponentInLua& c) {auto& s = *CM.GetBy<Shape>(c.handle); return s.color; };
	auto SetColor = [](ComponentInLua& c, sol::object v)
	{
		auto& s = *CM.GetBy<Shape>(c.handle);
		v.get_type();
		if (v.is<Vector3D>())
		{
			s.color = v.as<Vector3D>();
		}
		else if (v.is<sol::table>())
		{
			sol::table t = v.as<sol::table>();
			int limit = t.size() > 3 ? 3 : t.size();
			for (int i = 1; i <= limit; ++i)
			{
				s.color[i - 1] = t[i];
			}
		}
	};

	sol::table t = FW.lua["Component"].get<sol::table>();
	t.new_usertype<ComponentInLua>(
		GetTypeName<Shape>(),
		"new", sol::constructors<ComponentInLua(ComponentHandle, ObjectHandle)>(),
		"handle", &ComponentInLua::handle,
		"owner", &ComponentInLua::owner,
		"type", sol::property(GetType, SetType),
		"cubeSize", sol::property(GetParam[0], SetParam[0]),
		"teapotSize", sol::property(GetParam[0], SetParam[0]),
		"sphereRadius", sol::property(GetParam[0], SetParam[0]),
		"torusInnerRadius", sol::property(GetParam[0], SetParam[0]),
		"coneBase", sol::property(GetParam[0], SetParam[0]),
		"sphereSlice", sol::property(GetParam[1], SetParam[1]),
		"torusOuterRadius", sol::property(GetParam[1], SetParam[1]),
		"coneHeight", sol::property(GetParam[1], SetParam[1]),
		"sphereStack", sol::property(GetParam[2], SetParam[2]),
		"torusSide", sol::property(GetParam[2], SetParam[2]),
		"coneSlice", sol::property(GetParam[2], SetParam[2]),
		"torusRing", sol::property(GetParam[3], SetParam[3]),
		"coneStack", sol::property(GetParam[3], SetParam[3]),
		"color", sol::property(GetColor, SetColor)
		);
}

StringHashMap<unsigned> Shape::InitTypeMap()
{
	std::unordered_map<std::string, unsigned> map;

	int i = 0;
	for (auto n : typeNames)
	{
		map[n] = i++;
	}

	return map;
}

LuaComponent::~LuaComponent()
{
	sol::state_view lua{ env.lua_state() };
	sol::table table = lua["components"].get<sol::table>();
	if (table.valid())
		table[handle.ToUInt64()] = sol::nil;
}

bool LuaComponent::SetScript(const std::string & name)
{
	auto& lua = FW.lua;
	sol::protected_function fn = lua["Component"]["prototype"][name];
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
