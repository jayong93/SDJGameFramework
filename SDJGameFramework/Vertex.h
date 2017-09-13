#pragma once

#include "Vector3D.h"

struct Vertex
{
	Vector3D position;
	Vector3D color;

	static void RegisterInLua(sol::table typeTable)
	{
		typeTable.new_usertype<Vertex>("Vertex",
			"position", &Vertex::position,
			"color", &Vertex::color
			);
	}
};