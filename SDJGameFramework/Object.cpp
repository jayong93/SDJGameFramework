#include "stdafx.h"
#include "Object.h"
#include "HandleManagers.h"

void Object::AddComponent(ComponentHandle handle)
{
	compoList.emplace_back(handle);
	compoIdxMap.emplace(handle, compoList.size() - 1);
	
	Component* compo = CM.Get(handle);
	compo->owner = this->handle;
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

		return true;
	}
	return false;
}
