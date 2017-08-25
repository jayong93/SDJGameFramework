#include "stdafx.h"
#include "ComponentManager.h"
#include "ObjectManager.h"

Component * ComponentManager::Get(const ComponentHandle & handle)
{
	if (handle.index >= handleList.size())
		return nullptr;

	HandleEntry& entry = handleList[handle.index];
	if (entry.isActive == false || entry.count != handle.count)
		return nullptr;

	return compoMap.at(entry.type)->Get(entry.index);
}

Component * ComponentManager::Get(uint64_t handle)
{
	return Get(Handle::ToHandle(handle));
}

void ComponentManager::Detele(const ComponentHandle & handle)
{
	bool con = handleList.size() > handle.index;
	if (!con) return;

	HandleEntry& entry = handleList[handle.index];
	if (entry.isActive == false || entry.count != handle.count)
		return;

	freeIndexQueue.emplace_back(handle.index);
	entry.isActive = false;

	ICompoList* list = compoMap.at(entry.type);

	Object* owner = OM.Get(list->Get(entry.index)->owner);
	if (owner)
		owner->DelComponent(handle);

	ComponentHandle lastHandle = list->Delete(entry.index);

	if (lastHandle)
	{
		HandleEntry& lastEntry = handleList[lastHandle.index];
		lastEntry.index = entry.index;
	}
}

void ComponentManager::Clear()
{
	for (auto& cList : compoMap)
	{
		cList.second->Clear();
	}

	handleList.clear();
	freeIndexQueue.clear();
}

void ComponentManager::ClearAndUnregister()
{
	Clear();
	compoMap.clear();
}
