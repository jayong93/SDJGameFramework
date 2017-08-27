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

	ICompoList* list = compoMap.at(entry.type);

	freeIndexQueue.emplace_back(handle.index);
	entry.isActive = false;

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
	MessageManager::Instance().Clear();
}

size_t ComponentManager::Type(const ComponentHandle& handle) const
{
	if (handle.index >= handleList.size())
		return 0;

	const HandleEntry& entry = handleList[handle.index];
	if (entry.isActive == false || entry.count != handle.count)
		return 0;

	return entry.type;
}

ComponentHandle ComponentManager::Add(size_t type, const ObjectHandle & owner)
{
	auto it = compoMap.find(type);
	bool con = it != compoMap.end();
	assert(con && "unregistered type");

	auto obj = OM.Get(owner);
	if (obj->HasComponent(type))
		return ComponentHandle();

	return AddComponent(type, it->second, owner);
}

ComponentHandle ComponentManager::AddComponent(size_t type, ICompoList * list, const ObjectHandle & owner)
{
	Component* compo = list->Add();

	HandleEntry* entry;
	unsigned index;

	if (!freeIndexQueue.empty())
	{
		index = freeIndexQueue.front();
		freeIndexQueue.pop_front();
		entry = &handleList[index];
		++entry->count;
		if (entry->count == 0)
			entry->count = 1;
	}
	else
	{
		handleList.emplace_back();
		index = handleList.size() - 1;
		entry = &handleList.back();
		entry->count = 1;
	}

	entry->isActive = true;
	entry->index = list->Size() - 1;
	entry->type = type;

	ComponentHandle handle = Handle(index, entry->count);
	compo->handle = handle;

	OM.Get(owner)->AddComponent(handle);

	return handle;
}
