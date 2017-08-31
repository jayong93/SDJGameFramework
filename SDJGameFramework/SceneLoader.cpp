#include "stdafx.h"
#include "SceneLoader.h"
#include "HandleManagers.h"

using namespace rapidjson;

void SceneLoader::LoadScene(const std::string file, sol::state_view& lua)
{
	std::ifstream ifs{ file };
	if (ifs.bad()) return;

	IStreamWrapper isw{ ifs };
	Document doc;
	doc.ParseStream(isw);
	if (doc.HasParseError()) return;

	LoadComponentScript(doc, lua);
	LoadObject(doc, lua);
}

static void SetArgsInTable(sol::table& args, const GenericValue<UTF8<>>& d)
{
	switch (d.GetType())
	{
		case Type::kNumberType:
			args.add(d.GetFloat());
			break;
		case Type::kStringType:
			args.add(d.GetString());
			break;
	}
}

static void SetNamedArgsInTable(sol::table& args, const std::string& name, const GenericValue<UTF8<>>& d)
{
	sol::state_view lua(args.lua_state());
	switch (d.GetType())
	{
		case Type::kNumberType:
			args[name] = d.GetFloat();
			break;
		case Type::kStringType:
			args[name] = d.GetString();
			break;
		case Type::kArrayType:
		{
			sol::table list = lua.create_table();
			for (auto& item : d.GetArray())
				SetArgsInTable(list, item);
			args[name] = list;
		}
		break;
	}
}

void SceneLoader::LoadObject(rapidjson::Document & scene, sol::state_view& lua)
{
	if (scene.HasMember("object"))
	{
		auto& objs = scene["object"];
		for (auto& o : objs.GetArray())
		{
			auto& data = o.GetObject();

			auto& posData = data["position"].GetArray();
			Vector3D pos;
			for (int i = 0; i < 3; ++i)
				pos.data[i] = posData[i].GetDouble();

			const char* objName = data["name"].GetString();
			auto hObj = OM.Add(objName, pos);

			LoadComponentOfObject(data, hObj, lua);
		}
	}
}

void SceneLoader::LoadComponentScript(rapidjson::Document & scene, sol::state_view& lua)
{
	if (scene.HasMember("component"))
	{
		auto& compos = scene["component"];
		if (compos.IsArray())
		{
			std::string compoName;
			for (auto& c : compos.GetArray())
			{
				compoName = c.GetString();
				std::string fullName = compoName; fullName += ".lua";
				std::ifstream compoScript{ fullName };
				if (compoScript.bad()) continue;

				sol::protected_function fn = lua.load_file(fullName);
				if (!fn.valid()) continue;
				lua["Component"]["prototype"][compoName] = fn;

				size_t type = GetHash(compoName);
				lua["Component"]["get"][type] = lua.create_table();
				lua["Component"]["set"][type] = lua.create_table();
			}
		}
	}
}

void SceneLoader::LoadComponentOfObject(rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<>>>& objData, ObjectHandle obj, sol::state_view & lua)
{
	bool hasVar = objData.HasMember("var");
	auto& compoList = objData["component"].GetArray();
	const std::string& objName = OM.Get(obj)->name;
	for (auto& c : compoList)
	{
		auto compoName = c.GetString();
		ComponentHandle compoHandle;
		if (CM.IsRegistered(compoName))
			compoHandle = CM.Add(compoName, obj);
		else
		{
			compoHandle = CM.AddLuaComponent(c.GetString(), obj);
			if (!compoHandle)
			{
				fprintf(stderr, "Invalid Component %s On %s", compoName, objName.c_str());
				continue;
			}
		}

		if (hasVar && objData["var"].HasMember(compoName))
		{
			sol::table args = lua.create_table();
			auto varData = objData["var"][compoName].GetObject();
			for (auto& d : varData)
			{
				SetNamedArgsInTable(args, d.name.GetString(), d.value);
			}
			sol::table compo = lua["Component"]["Get"](compoHandle.ToUInt64());
			compo["Set"](compo, args);
		}
	}
}
