#include "stdafx.h"
#include "Systems.h"
#include "Framework.h"

void Framework::Init()
{
	// 루아 초기화
	InitLuaFunc();

	// 시스템 초기화
	render.Init();

	ObjectHandle tObj = OM.Add("TestObj", 1.f, 1.f, 0.f);
	ObjectHandle tObj2 = OM.Add("TestObj2", 0.f, 0.f, 2.f);

	ComponentHandle shapeHandle = CM.Add<ShapeCompo>();
	ShapeCompo* shape = CM.GetBy<ShapeCompo>(shapeHandle);
	shape->shapeType = ShapeCompo::SPHERE;
	shape->drawParam[0] = 5.f;
	shape->drawParam[1] = 20.f;
	shape->drawParam[2] = 20.f;
	OM.Get(tObj)->AddComponent(shapeHandle);

	shapeHandle = CM.Add<ShapeCompo>();
	shape = CM.GetBy<ShapeCompo>(shapeHandle);
	shape->shapeType = ShapeCompo::CUBE;
	shape->drawParam[0] = 4.f;
	OM.Get(tObj2)->AddComponent(shapeHandle);
}

void Framework::InitLuaFunc()
{
	lua.open_libraries(sol::lib::io, sol::lib::string, sol::lib::math, sol::lib::base);
	lua["Object"] = lua.create_table();
	lua["Object"]["__index"] = lua["Object"];

	auto selfCheck = [](sol::table self) -> uint64_t {
		if (self.valid())
		{
			uint64_t handle = self["handle"];
			return handle;
		}
		return 0;
	};
	lua["Object"]["Move"] = [&](sol::table self, double x, double y, double z) {
		if (Object* obj = OM.Get(selfCheck(self)))
		{
			obj->position += Vector3D(x, y, z);
		}
	};
	lua["Object"]["MoveTo"] = [&](sol::table self, double x, double y, double z) {
		if (Object* obj = OM.Get(selfCheck(self)))
		{
			obj->position.Set(x, y, z);
		}
	};
	lua["Object"]["Position"] = [&](sol::table self) {
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
	lua["GetObjectByName"] = [&](std::string name) {
		auto obj = lua.create_table_with("handle", uint64_t(OM.GetByName(name.c_str())->handle));
		obj[sol::metatable_key] = lua["Object"];
		return obj;
	};

	//sol::table globalMeta = lua.create_table();
	//lua[sol::metatable_key] = globalMeta;

	//sol::table mt = lua[sol::metatable_key];
	//globalMeta["__newindex"] = []() {};
}

void Framework::Update(double time)
{
}

void Framework::Render()
{
	render.Render();
}

void Framework::MainLoop()
{
}
