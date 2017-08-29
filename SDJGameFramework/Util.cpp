#include "stdafx.h"
#include "Util.h"
#include "HandleManagers.h"

size_t GetHash(const std::string& str)
{
	static std::hash<std::string> hash;
	return hash(str);
}

static void LuaObjectInitialize(sol::state_view& lua)
{
	auto objectMt = lua.create_table();
	objectMt["__index"] = objectMt;

	auto selfCheck = [](sol::table self) -> uint64_t {
		if (self.valid())
		{
			uint64_t handle = self["handle"];
			return handle;
		}
		return 0;
	};
	objectMt["Move"] = [&](sol::table self, double x, double y, double z) {
		if (Object* obj = OM.Get(selfCheck(self)))
		{
			obj->position += Vector3D(x, y, z);
		}
	};
	objectMt["MoveTo"] = [&](sol::table self, double x, double y, double z) {
		if (Object* obj = OM.Get(selfCheck(self)))
		{
			obj->position.Set(x, y, z);
		}
	};
	objectMt["Position"] = [&](sol::table self) {
		Object* obj = OM.Get(selfCheck(self));
		if (!obj)
		{
			sol::object nil = sol::make_object(lua, sol::nil);
			return std::make_tuple(nil, nil, nil);
		}

		auto pos = obj->position;
		sol::object p[3];
		for (int i = 0; i < 3; ++i)
			p[i] = sol::make_object(lua, pos.data[i]);
		return std::make_tuple(p[0], p[1], p[2]);
	};
	objectMt["Has"] = [&](sol::table self, std::string type) {
		if (Object* obj = OM.Get(selfCheck(self)))
		{
			return obj->HasComponent(type);
		}
		return false;
	};
	objectMt["SendMsg"] = [&](sol::table self, sol::table msg) {
		if (Object* obj = OM.Get(selfCheck(self)))
		{
			if (msg.valid())
			{
				obj->SendMsg(msg);
			}
		}
	};
	objectMt["GetComponent"] = [&](sol::table self, std::string type) -> sol::object
	{
		if (Object* obj = OM.Get(selfCheck(self)))
		{
			auto compo = obj->GetComponent(type);
			if (CM.Get(compo))
			{
				return lua["Component"]["Get"](compo.ToUInt64());
			}
		}
		return sol::nil;
	};
	lua["Object"]["Get"] = [&, objectMt](sol::object args) -> sol::object {
		uint64_t handle = 0;

		if (args.is<std::string>())
		{
			auto name = args.as<std::string>();
			Object* obj = OM.GetByName(name.c_str());
			if (!obj) return sol::nil;
			handle = obj->handle.ToUInt64();
		}
		else if (args.is<uint64_t>())
		{
			handle = args.as<uint64_t>();
			Object* obj = OM.Get(handle);
			if (!obj) return sol::nil;
		}

		auto objTable = lua.create_table_with("handle", handle);
		objTable[sol::metatable_key] = objectMt;
		return objTable;
	};
}

static void LuaComponentInitialize(sol::state_view& lua)
{
	auto compoTable = lua["Component"].get<sol::table>();
	compoTable.create_named("prototype");
	compoTable.create_named("instance");
	compoTable.create_named("get");
	compoTable.create_named("set");

	// Get/Set 함수 설정
	sol::table compoMt = lua.create_table();
	compoMt["__index"] = compoMt;
	auto selfCheck = [](sol::table self) -> uint64_t {
		if (self.valid())
		{
			uint64_t handle = self["handle"];
			return handle;
		}
		return 0;
	};
	compoMt["Get"] = [&, selfCheck](sol::table self, sol::object arg) -> sol::object
	{
		auto handle = selfCheck(self);
		if (CM.Get(handle))
		{
			auto type = CM.Type(handle);
			sol::table get = lua["Component"]["get"][type];
			if (arg.is<sol::table>())
			{
				sol::table argTable = arg.as<sol::table>();
				sol::table t = lua.create_table();
				for (int i = 1; i <= argTable.size(); ++i)
				{
					if (argTable[i].get_type() == sol::type::string)
					{
						std::string name = argTable[i];
						sol::protected_function fn = get[name];
						if (fn.valid())
							t[i] = fn(handle);
						else
							t[i] = sol::nil;
					}
					else
						t[i] = sol::nil;
				}
				return t;
			}
			else if (arg.is<std::string>())
			{
				return get[arg](handle);
			}
		}
		return sol::nil;
	};
	compoMt["Set"] = [&, selfCheck](sol::table self, sol::object arg)
	{
		auto handle = selfCheck(self);
		if (CM.Get(handle))
		{
			auto type = CM.Type(handle);
			sol::table set = lua["Component"]["set"][type];
			if (arg.is<sol::table>())
			{
				sol::table argTable = arg.as<sol::table>();
				for (auto& i : argTable)
				{
					if (i.first.is<std::string>())
					{
						sol::protected_function fn = set[i.first];
						if (fn.valid())
							set[i.first](handle, i.second);
					}
				}
			}
		}
	};
	compoTable["Get"] = [&, compoMt](uint64_t handle) -> sol::object
	{
		auto type = CM.Type(handle);
		sol::table table = lua.create_table_with("handle", handle, "type", type);
		table[sol::metatable_key] = compoMt;
		return table;
	};
}

static void LuaGlobalInitialize(sol::state_view& lua)
{
	lua.create_named_table("Component");
	lua.create_named_table("Object");
}

void LuaStateInitialize(sol::state_view& lua)
{
	lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);
	LuaGlobalInitialize(lua);
	LuaObjectInitialize(lua);
	LuaComponentInitialize(lua);
}