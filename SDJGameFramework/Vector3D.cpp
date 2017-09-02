#include "stdafx.h"
#include "Vector3D.h"

void Vector3D::RegisterInLua(sol::state_view & lua)
{
	lua.new_usertype<Vector3D>(
		"Vector",
		sol::constructors<Vector3D(), Vector3D(float, float, float)>(),
		"x", &Vector3D::x,
		"y", &Vector3D::y,
		"z", &Vector3D::z,
		sol::meta_function::addition, &Vector3D::operator+,
		sol::meta_function::subtraction, &Vector3D::operator-,
		sol::meta_function::multiplication, &Vector3D::operator*,
		"dot", sol::property(&Vector3D::Dot),
		"length", sol::property(&Vector3D::Length),
		"normal", sol::property(&Vector3D::Normal)
		);
}
