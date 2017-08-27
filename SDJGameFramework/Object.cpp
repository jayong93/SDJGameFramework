#include "stdafx.h"
#include "Object.h"
#include "HandleManagers.h"

void Object::AddComponent(ComponentHandle handle)
{
	Component* compo = CM.Get(handle);
	if (compo == nullptr) return;
	if (compo->owner) return;

	compoList.emplace_back(handle);
	compoIdxMap.emplace(handle, compoList.size() - 1);

	auto type = CM.Type(handle);
	auto it = compoTypeMap.find(type);
	if (it == compoTypeMap.end())
		compoTypeMap.emplace(type, 1);
	else
		++it->second;

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

		auto it2 = compoTypeMap.find(CM.Type(handle));
		if (it2->second <= 1)
			compoTypeMap.erase(it2);
		else
			--it2->second;

		return true;
	}
	return false;
}

bool Object::HasComponent(std::string type)
{
	auto numType = GetHash(type);
	auto it = compoTypeMap.find(numType);
	if (it != compoTypeMap.end() && it->second > 0)
		return true;
	return false;
}

void Object::SendMsg(sol::object msg)
{
	for (auto& c : compoList)
	{
		CM.Get(c)->SendMsg(msg);
	}

	sol::state_view lua(msg.lua_state());

	auto luaCompos = lua["CompoList"][(uint64_t)handle];
	if (luaCompos.get_type() == sol::type::table)
	{
		auto& table = luaCompos.get<sol::table>();
		for (auto& c : table)
		{
			if (c.second.get_type() == sol::type::table)
			{
				c.second.as<sol::table>()["SendMsg"](msg);
			}
		}
	}
}
