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

	return compoLists.at(entry.type)->Get(entry.index);
}

void ComponentManager::Detele(const ComponentHandle & handle)
{
	bool con = handleList.size() > handle.index;
	assert(con && "invalid index");

	freeIndexQueue.emplace_back(handle.index);
	HandleEntry& entry = handleList[handle.index];
	entry.isActive = false;

	ICompoList* list = compoLists.at(entry.type);

	Object* owner = OM.Get(list->Get(entry.index)->owner);
	owner->compoMap.erase(entry.type);

	ComponentHandle lastHandle = list->Delete(entry.index);

	if (lastHandle)
	{
		HandleEntry& lastEntry = handleList[lastHandle.index];
		lastEntry.index = entry.index;
	}
}

ComponentHandle ComponentManager::Add(size_t type, const ObjectHandle & obj)
{
	auto it = compoLists.find(type);
	bool con = it != compoLists.end();
	assert(con && "unregistered type");

	Object* owner = OM.Get(obj);
	assert(owner && "invalid owner handle");

	con = owner->compoMap.find(type) == owner->compoMap.end();
	assert(con && "same type component already exists");

	Component* compo = it->second->Add();
	compo->owner = obj;

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
	entry->index = it->second->Size() - 1;
	entry->type = type;
	entry->ownerHandle = obj;

	ComponentHandle handle = Handle(index, entry->count);
	compo->handle = handle;

	owner->compoMap.emplace(type, handle);

	return handle;
}

void ComponentManager::RegisterComponentList(size_t type, ICompoList * list)
{
	bool con = compoLists.end() == compoLists.find(type);
	assert(con && "this type registered already");
	compoLists[type] = list;
}
