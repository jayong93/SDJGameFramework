#pragma once

#include "Vector2D.h"
#include "Handles.h"

struct Object
{
	ObjectHandle handle;
	std::map<size_t, ComponentHandle> compoMap;
	std::string name;

	Vector2D position;

	template <typename T>
	ComponentHandle GetComponent()
	{
		auto it = compoMap.find(typeid(T).hash_code());
		if (it == compoMap.end())
			return ComponentHandle();
		else
			return it->second;
	}
};