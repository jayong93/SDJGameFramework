#include "stdafx.h"
#include "ObjectManager.h"
#include "Framework.h"

ObjectHandle ObjectManager::Add(const std::string & name, const Vector3D & pos)
{
	return Add(name, pos.x, pos.y, pos.z);
}

ObjectHandle ObjectManager::Add(const std::string& name, float x, float y, float z)
{
	bool con = objectNameMap.find(name) == objectNameMap.end();
	if (!con)
		return ObjectHandle();

	objectList.emplace_back();

	ObjectHandle handle;

	if (!freeIndexQueue.empty())
	{
		unsigned index = freeIndexQueue.front();
		freeIndexQueue.pop_front();

		HandleEntry& entry = handleList[index];
		++entry.count;
		if (entry.count == 0)
			entry.count = 1;
		entry.isActive = true;
		entry.index = objectList.size() - 1;

		handle = Handle(index, entry.count);
		objectList.back().handle = handle;
	}
	else
	{
		handleList.emplace_back();
		HandleEntry& entry = handleList.back();

		entry.count = 1;
		entry.isActive = true;
		entry.index = objectList.size() - 1;

		handle = Handle(handleList.size() - 1, entry.count);
		Object& obj = objectList.back();
		obj.handle = handle;
		obj.name = name;
		obj.position = Vector3D{ x,y,z };
		objectNameMap.emplace(name, handle);
	}

	return handle;
}

void ObjectManager::AddChild(const ObjectHandle & parent, const ObjectHandle & child)
{
	auto pObj = Get(parent);
	auto cObj = Get(child);
	if (!pObj || !cObj) return;

	pObj->childList.emplace_back(child);
	cObj->parent = parent;
}

Object * ObjectManager::Get(const ObjectHandle & handle)
{
	if (!IsValid(handle)) return nullptr;

	HandleEntry& entry = handleList[handle.index];

	return &objectList[entry.index];
}

Object * ObjectManager::GetByName(const std::string& name)
{
	auto it = objectNameMap.find(name);
	if (it == objectNameMap.end())
		return nullptr;
	return Get(it->second);
}

void ObjectManager::Delete(const ObjectHandle & handle)
{
	if (!IsValid(handle)) return;

	HandleEntry& entry = handleList[handle.index];

	auto& obj = objectList[entry.index];

	for (auto& c : obj.compoList)
	{
		CM.Delete(c);
	}

	freeIndexQueue.emplace_back(handle.index);
	entry.isActive = false;

	objectNameMap.erase(objectList[entry.index].name);

	if (objectList.size() > 1)
	{
		HandleEntry& lastEntry = handleList[objectList.back().handle.index];
		lastEntry.index = entry.index;

		std::swap(objectList[entry.index], objectList.back());
	}

	objectList.pop_back();
}

void ObjectManager::Clear()
{
	this->handleList.clear();
	this->freeIndexQueue.clear();
	this->objectList.clear();
	this->objectNameMap.clear();
}

bool ObjectManager::IsValid(const ObjectHandle & handle) const
{
	if (!handle)
		return false;

	if (handleList.size() <= handle.index)
		return false;

	const HandleEntry& entry = handleList[handle.index];
	if (entry.isActive == false || entry.count != handle.count)
		return false;

	return true;
}
