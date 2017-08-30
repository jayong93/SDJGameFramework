#include "stdafx.h"
#include "ComponentManager.h"
#include "MessageManager.h"
#include "Framework.h"
#include "Util.h"
#include "ComponentGetSetMacro.h"

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

void Shape::InitGetSetFunc()
{
	START_GETSET_DEF(Shape);
	SIMPLE_GETSET_DEF(sphereRadius, sphere.radius);
	SIMPLE_GETSET_DEF(coneBase, cone.base);
	SIMPLE_GETSET_DEF(coneHeight, cone.height);
	SIMPLE_GETSET_DEF(torusSide, torus.side);
	SIMPLE_GETSET_DEF(torusRing, torus.ring);
	SIMPLE_GETSET_DEF(torusInnerRadius, torus.innerRadius);
	SIMPLE_GETSET_DEF(torusOuterRadius, torus.outerRadius);
	SIMPLE_GETSET_DEF(cubeSize, cube.size);
	SIMPLE_GETSET_DEF(teapotSize, teapot.size);
	SIMPLE_GETSET_DEF(sphereSlice, sphere.slice);
	SIMPLE_GETSET_DEF(coneSlice, cone.slice);
	SIMPLE_GETSET_DEF(sphereStack, sphere.stack);
	SIMPLE_GETSET_DEF(coneStack, cone.stack);
	START_COMPLEX_GET_DEF(type, obj);
	{
		return typeNames[obj->shapeType];
	}
	END_COMPLEX_GET_DEF();
	START_COMPLEX_SET_DEF(type, obj, val);
	{
		if (val.is<std::string>())
		{
			auto it = obj->typeMap.find(val.as<std::string>());
			if (it != obj->typeMap.end())
				obj->shapeType = Shape::Type(it->second);
		}
	}
	END_COMPLEX_SET_DEF();
	START_MULTI_GET_DEF(color, obj, vr);
	{
		for (int i = 0; i < 3; ++i)
			vr.add(obj->color.data[i]);
	}
	END_MULTI_GET_DEF();
	START_COMPLEX_SET_DEF(color, obj, val);
	{
		sol::table v = val;
		if (v.valid())
		{
			for (int i = 0; i < 3; ++i)
			{
				sol::optional<double> val = v[i + 1];
				if (val)
					obj->color.data[i] = val.value();
			}
		}
	}
	END_COMPLEX_SET_DEF();
	END_GETSET_DEF();
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
	auto& lua = FW.lua;
	sol::table table = lua["Component"]["instance"][handle.ToUInt64()];
	if (table.valid())
		lua["Component"]["instance"][handle.ToUInt64()] = sol::nil;
}

bool LuaComponent::SetScript(const std::string & name)
{
	auto& lua = FW.lua;
	sol::protected_function fn = lua["Component"]["prototype"][name];
	if (fn.valid())
	{
		env = sol::environment{ lua, sol::create, lua.globals() };
		sol::set_environment(env, fn); fn();
		env["owner"] = lua["Object"]["Get"](handle.ToUInt64());
		lua["Component"]["instance"][handle.ToUInt64()] = env.as<sol::table>();
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
