#include "stdafx.h"
#include "ComponentManager.h"
#include "Framework.h"
#include "ObjectManager.h"
#include "MessageManager.h"

Component * ComponentManager::Get(const ComponentHandle & handle)
{
	if (!IsValid(handle)) return nullptr;

	HandleEntry& entry = handleList[handle.index];

	return compoMap.at(entry.type)->Get(entry.index);
}

void ComponentManager::Delete(const ComponentHandle & handle)
{
	if (!IsValid(handle)) return;

	HandleEntry& entry = handleList[handle.index];
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
	if (!IsValid(handle))
		return 0;

	const HandleEntry& entry = handleList[handle.index];

	return entry.realType;
}

ComponentHandle ComponentManager::AddLuaComponent(size_t type, const std::string & scriptName, const ObjectHandle & owner)
{
	auto obj = OM.Get(owner);
	if (obj->HasComponent(type))
		return ComponentHandle();

	size_t luaType = GetTypeHash<LuaComponent>();
	auto handle = Add_(luaType, type, compoMap[luaType], owner);
	auto compo = GetBy<LuaComponent>(handle);
	bool ret = compo->SetScript(scriptName);
	if (!ret)
	{
		OM.Get(compo->owner)->DelComponent(handle);
		return ComponentHandle();
	}
	return handle;
}

bool ComponentManager::IsLuaComponent(const ComponentHandle & handle) const
{
	if (!IsValid(handle)) return false;

	const HandleEntry& entry = handleList[handle.index];

	if (entry.type == GetTypeHash<LuaComponent>())
		return true;
	return false;
}

bool ComponentManager::IsValid(const ComponentHandle & handle) const
{
	if (handle.index >= handleList.size())
		return false;

	const HandleEntry& entry = handleList[handle.index];
	if (entry.isActive == false || entry.count != handle.count)
		return false;

	return true;
}

ComponentHandle ComponentManager::Add_(size_t type, const ObjectHandle & owner)
{
	auto it = compoMap.find(type);
	if (it == compoMap.end())
		return ComponentHandle();

	auto obj = OM.Get(owner);
	if (obj->HasComponent(type))
		return ComponentHandle();

	return Add_(type, type, it->second, owner);
}

ComponentHandle ComponentManager::Add_(size_t type, size_t realType, ICompoList * list, const ObjectHandle & owner)
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
	entry->realType = realType;

	ComponentHandle handle = Handle(index, entry->count);
	compo->handle = handle;

	OM.Get(owner)->AddComponent(handle);

	return handle;
}

void ComponentManager::RegisterComponentList_(const std::string& type, size_t numType, MessageMap & msgMap)
{
	MM.RegisterComponentMessageMap(numType, msgMap);
	FW.lua["Component"]["get"][numType] = FW.lua.create_table();
	FW.lua["Component"]["set"][numType] = FW.lua.create_table();
}
