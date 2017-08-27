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

	ObjectHandle Add(const std::string& name, float x = 0.f, float y = 0.f, float z = 0.f);
	ObjectHandle Add(const std::string& name, const Vector3D& pos);
	Object* Get(const ObjectHandle & handle);
	Object* Get(uint64_t handle);
	Object* GetByName(const std::string& name);
	void Delete(const ObjectHandle & handle);
	size_t Size() const { return objectList.size(); }
	void Clear();

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
	std::unordered_map<std::string, ObjectHandle> objectNameMap;
};