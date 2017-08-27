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
	lua["Object"]["GetObject"] = [&, objectMt](std::string name) {
		auto obj = lua.create_table_with("handle", uint64_t(OM.GetByName(name.c_str())->handle));
		obj[sol::metatable_key] = objectMt;
		return obj;
	};
}

static void LuaComponentInitialize(sol::state_view& lua)
{
	auto& compoTable = lua["Component"].get<sol::table>();
	compoTable.create_named("prototype");
	compoTable.create_named("instance");
	lua.safe_script(R"(
function Component:Loop(time)
	for i, v in pairs(self.instance) do
		v.Update(time)
	end
end
)");
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