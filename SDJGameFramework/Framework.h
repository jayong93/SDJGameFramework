#pragma once

#include "Systems.h"
#include "HandleManagers.h"
#include "Action.h"
#include "ActionList.h"
#include "Timer.h"

#define FW Framework::Instance()

struct Framework
{
	static Framework& Instance()
	{
		static Framework inst;
		return inst;
	}

	void Init();
	void Update(double time);
	void Render();
	void MainLoop();
	void SetView(int w, int h);
	auto RunLuaScript(const std::string& script) { return lua.safe_script(script, luaEnv, luaErrFunc); }
	void LoadScene(const std::string& fileName);
	void CleanUp();

	sol::state lua;
	sol::environment luaEnv;

	RenderSystem render;
	GameLogic logic;

	double limitedFrame = 60.;
	std::function<sol::protected_function_result(lua_State*, sol::protected_function_result)> luaErrFunc;
	Timer timer;

private:
	Framework();
	~Framework() {}
};