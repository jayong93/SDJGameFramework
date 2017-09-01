#pragma once

#include "Handles.h"

class SceneLoader
{
public:
	void LoadScene(const std::string file, sol::state_view& lua) const;
private:
	void LoadObject(const rapidjson::Document & scene, sol::state_view& lua) const;
	void LoadComponentScript(const rapidjson::Document & scene, sol::state_view& lua) const;
	void LoadComponentOfObject(rapidjson::GenericObject<true,rapidjson::GenericValue<rapidjson::UTF8<>>>& objData, ObjectHandle obj, sol::state_view& lua) const;
};