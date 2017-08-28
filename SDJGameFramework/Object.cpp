#include "stdafx.h"
#include "Object.h"
#include "HandleManagers.h"
#include "Framework.h"
#include "Component.h"

bool Object::AddComponent(ComponentHandle handle)
{
	Component* compo = CM.Get(handle);
	if (compo == nullptr) return false;
	if (compo->owner) return false;

	auto type = CM.Type(handle);
	auto it = compoTypeSet.find(type);
	if (it == compoTypeSet.end())
		compoTypeSet.emplace(type);
	else return false;

	compoList.emplace_back(handle);
	compoIdxMap.emplace(handle, compoList.size() - 1);
	compo->owner = this->handle;

	return true;
}

bool Object::DelComponent(ComponentHandle handle)
{
	auto it = compoIdxMap.find(handle);
	if (it != compoIdxMap.end())
	{
		size_t prevIndex = it->second;
		ComponentHandle& lastCompo = compoList.back();

		compoIdxMap[lastCompo] = prevIndex;
		std::swap(lastCompo, compoList[it->second]);

		compoList.pop_back();
		compoIdxMap.erase(it);

		auto it2 = compoTypeSet.find(CM.Type(handle));
		if (it2 != compoTypeSet.end())
			compoTypeSet.erase(it2);

		CM.Delete(handle);

		return true;
	}
	return false;
}

bool Object::HasComponent(size_t type) const
{
	auto it = compoTypeSet.find(type);
	if (it != compoTypeSet.end())
		return true;
	return false;
}

void Object::SendMsg(sol::object msg)
{
	for (auto& c : compoList)
	{
		CM.Get(c)->SendMsg(msg);
	}
}
