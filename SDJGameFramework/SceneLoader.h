#pragma once

#include "Handles.h"

class SceneLoader
{
public:
	void LoadScene(const std::string file, sol::state_view& lua);
private:
	void LoadObject(rapidjson::Document& scene, sol::state_view& lua);
	void LoadComponentScript(rapidjson::Document& scene, sol::state_view& lua);
	void LoadComponentOfObject(rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<>>>& objData, ObjectHandle obj, sol::state_view& lua);
};