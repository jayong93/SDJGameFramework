#include "stdafx.h"
#include "LuaInterfaceType.h"
#include "HandleManagers.h"
#include "Framework.h"

ObjectInLua::ObjectInLua(const ObjectHandle & h) : handle{ h }, component{ FW.lua, sol::create }, child{ FW.lua, sol::create }
{
}

sol::object ObjectInLua::GetPosition(sol::this_state lua) const
{
	if (OM.IsValid(handle))
	{
		return sol::make_object(lua, OM.Get(handle)->position);
	}
	return sol::nil;
}

void ObjectInLua::SetPosition(const Vector3D & v)
{
	if (OM.IsValid(handle))
		OM.Get(handle)->position = v;
}

bool ObjectInLua::IsValid() const
{
	return OM.IsValid(handle);
}

void ObjectInLua::Move(const Vector3D & offset)
{
	if (OM.IsValid(handle))
		OM.Get(handle)->Move(offset);
}

void ObjectInLua::Move(float x, float y, float z)
{
	if (OM.IsValid(handle))
		OM.Get(handle)->Move(Vector3D(x, y, z));
}

ComponentInLua::ComponentInLua(ComponentHandle h, ObjectHandle o) : handle{ h }
{
	if (CM.IsValid(h) && OM.IsValid(o))
	{
		auto obj = OM.Get(o);
		owner = &FW.lua["objects"][obj->name].get<ObjectInLua>();
	}
}
