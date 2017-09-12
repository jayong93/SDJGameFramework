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
	auto it = compoTypeMap.find(type);
	if (it == compoTypeMap.end())
		compoTypeMap.emplace(type, handle);
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

		auto it2 = compoTypeMap.find(CM.Type(handle));
		if (it2 != compoTypeMap.end())
			compoTypeMap.erase(it2);

		CM.Delete(handle);

		return true;
	}
	return false;
}

bool Object::HasComponent(size_t type) const
{
	auto it = compoTypeMap.find(type);
	if (it != compoTypeMap.end())
		return true;
	return false;
}

ComponentHandle Object::GetComponent(size_t type)
{
	auto it = compoTypeMap.find(type);
	if (it != compoTypeMap.end())
		return it->second;
	return ComponentHandle();
}

ComponentHandle Object::GetComponent(const std::string & type)
{
	return GetComponent(GetHash(type));
}

void Object::SendMsg(sol::object msg)
{
	for (auto& c : compoList)
	{
		CM.Get(c)->SendMsg(msg);
	}
}

void Object::RegisterInLua()
{
	FW.lua.new_usertype<Object>(
		"Object",
		"new", sol::no_constructor,
		"get_by_id", [](uint64_t id) {return OM.Get(id);},
		"get_by_name", [](const std::string& name) {return OM.GetByName(name); },
		"id", sol::property([](Object& obj) {return obj.handle.ToUInt64();}),
		"position", &Object::position,
		"move", sol::overload(sol::resolve<void(const Vector3D&)>(&Object::Move), sol::resolve<void(float,float,float)>(&Object::Move)),
		"get_component", [](Object& obj, const std::string& type) -> sol::object {
		auto hCompo = obj.GetComponent(type); 
		return FW.lua["Component"]["get"](hCompo.ToUInt64());
	},
		"child_list", sol::readonly(&Object::childList)
		);
}
