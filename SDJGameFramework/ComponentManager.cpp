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

void ComponentManager::Detele(const ComponentHandle & handle)
{
	bool con = handleList.size() > handle.index;
	assert(con && "invalid index");
	
	freeIndexQueue.emplace_back(handle.index);
	HandleEntry& entry = handleList[handle.index];
	entry.isActive = false;

	ICompoList* list = compoMap.at(entry.type);

	Object* owner = OM.Get(list->Get(entry.index)->owner);
	owner->DelComponent(handle);

	ComponentHandle lastHandle = list->Delete(entry.index);

	if (lastHandle)
	{
		HandleEntry& lastEntry = handleList[lastHandle.index];
		lastEntry.index = entry.index;
	}
}
