#pragma once

#include "Object.h"
#include "Vector3D.h"

struct ObjectInLua {
	ObjectHandle handle;
	sol::table component;
	sol::table child;

	ObjectInLua(const ObjectHandle& h);
	sol::object GetPosition(sol::this_state lua) const;
	void SetPosition(const Vector3D& v);
	bool IsValid() const;
	void Move(const Vector3D& offset);
	void Move(float x, float y, float z);
};

struct ComponentInLua
{
	ComponentHandle handle;
	ObjectInLua* owner;

	ComponentInLua(ComponentHandle h, ObjectHandle o);
};
