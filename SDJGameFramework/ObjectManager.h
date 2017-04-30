#pragma once

#include "Object.h"

#define OM ObjectManager::Instance()

class ObjectManager
{
public:
	static ObjectManager& Instance()
	{
		static ObjectManager inst;
		return inst;
	}

	ObjectHandle Add();
	Object* Get(const ObjectHandle & handle);
	void Delete(const ObjectHandle & handle);

private:
	ObjectManager() {}

	struct HandleEntry
	{
		unsigned index = 0;
		unsigned count = 0;
		bool isActive = false;
	};

	std::vector<HandleEntry> handleList;
	std::deque<unsigned> freeIndexQueue;
	std::vector<Object> objectList;
};