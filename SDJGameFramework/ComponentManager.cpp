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

ComponentHandle ComponentManager::AddLuaComponent(const std::string & scriptName, const ObjectHandle & owner)
{
	auto handle = Add_(GetTypeName<LuaComponent>(), scriptName, owner);
	if (!handle)
		return ComponentHandle();

	auto compo = GetBy<LuaComponent>(handle);
	bool ret = compo->SetScript(scriptName);
	if (!ret)
	{
		OM.Get(compo->owner)->DelComponent(handle);
		return ComponentHandle();
	}

	auto obj = OM.Get(owner);
	FW.lua["components"][handle.ToUInt64()] = compo->env;
	auto compoTable = FW.lua["objects"][obj->name]["component"].get<sol::optional<sol::table>>();
	if (compoTable)
	{
		compoTable->set(scriptName, FW.lua["components"][handle.ToUInt64()]);
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
	if (!handle)
		return false;

	if (handle.index >= handleList.size())
		return false;

	const HandleEntry& entry = handleList[handle.index];
	if (entry.isActive == false || entry.count != handle.count)
		return false;

	return true;
}

ComponentHandle ComponentManager::Add_(const std::string& type, const std::string& realType, const ObjectHandle & owner)
{
	size_t hashedType = GetHash(type);
	size_t hashedRealType = GetHash(realType);

	auto it = compoMap.find(hashedType);
	if (it == compoMap.end())
		return ComponentHandle();

	ICompoList* list = compoMap[hashedType];
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
	entry->type = hashedType;
	entry->realType = hashedRealType;

	ComponentHandle handle = Handle(index, entry->count);
	compo->handle = handle;

	Object* obj = OM.Get(owner);
	bool ret = obj->AddComponent(handle);
	if (!ret)
	{
		Delete(handle);
		return ComponentHandle();
	}

	if (hashedType != GetTypeHash<LuaComponent>())
	{
		FW.lua["components"][handle.ToUInt64()] = FW.componentTable[realType]["new"](handle, owner);
		FW.lua["objects"][obj->name]["component"][type] = FW.lua["components"][handle.ToUInt64()];
	}

	return handle;
}

