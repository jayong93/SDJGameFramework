#include "stdafx.h"
#include "ObjectManager.h"

ObjectHandle ObjectManager::Add()
{
	objectList.emplace_back();

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

		ObjectHandle handle = Handle(index, entry.count);
		objectList.back().handle = handle;
		return handle;
	}
	else
	{
		handleList.emplace_back();
		HandleEntry& entry = handleList.back();

		entry.count = 1;
		entry.isActive = true;
		entry.index = objectList.size() - 1;

		ObjectHandle handle = Handle(handleList.size() - 1, entry.count);
		objectList.back().handle = handle;
		return handle;
	}
}

Object * ObjectManager::Get(const ObjectHandle & handle)
{
	if (handleList.size() <= handle.index)
		return nullptr;

	HandleEntry& entry = handleList[handle.index];

	if (entry.isActive == false || entry.count != handle.count)
		return nullptr;

	return &objectList[entry.index];
}

void ObjectManager::Delete(const ObjectHandle & handle)
{
	bool con = handleList.size() > handle.index;
	assert(con && "invalid index");

	freeIndexQueue.emplace_back(handle.index);
	HandleEntry& entry = handleList[handle.index];
	entry.isActive = false;

	if (objectList.size() > 1)
	{
		HandleEntry& lastEntry = handleList[objectList.back().handle.index];
		lastEntry.index = entry.index;

		std::swap(objectList[entry.index], objectList.back());
		objectList.pop_back();
	}
}
