#pragma once

class SceneLoader
{
public:
	void LoadScene(const std::string file, sol::state_view& lua);
private:
	void LoadObject(rapidjson::Document& scene, sol::state_view& lua);
	void LoadComponent(rapidjson::Document& scene, sol::state_view& lua);
};