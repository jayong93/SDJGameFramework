#pragma once

#include "Vector2D.h"
#include "Handles.h"

struct Object
{
	ObjectHandle handle;
	std::vector<ComponentHandle> compoList;
	std::map<size_t, ComponentHandle> compoMap;

	Vector2D position;
};