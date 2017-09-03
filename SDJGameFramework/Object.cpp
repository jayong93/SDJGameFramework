#include "stdafx.h"
#include "Object.h"
#include "HandleManagers.h"
#include "Framework.h"
#include "Component.h"
#include "LuaInterfaceType.h"

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
	FW.lua.new_usertype<ObjectInLua>(
		"Object",
		"new", sol::constructors<ObjectInLua(ObjectHandle)>(),
		"handle", &ObjectInLua::handle,
		"position", sol::property(&ObjectInLua::GetPosition, &ObjectInLua::SetPosition),
		"valid", sol::property(&ObjectInLua::IsValid),
		"Move", sol::overload(sol::resolve<void(const Vector3D&)>(&ObjectInLua::Move), sol::resolve<void(float,float,float)>(&ObjectInLua::Move)),
		"component", &ObjectInLua::component,
		"child", &ObjectInLua::child
		);
}
