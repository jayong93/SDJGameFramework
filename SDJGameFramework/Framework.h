#pragma once

#include "Systems.h"
#include "Action.h"
#include "ActionList.h"
#include "Timer.h"
#include "SceneLoader.h"

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
	void LoadScene(const std::string& fileName);
	void CleanUp();
	Timer& GetTimer() { return timer; }
	void EnableFrameLimit(double targetFps) { if (targetFps > 0) limitedTime = 1 / targetFps; }
	void DisableFrameLimit() { limitedTime = 0.; }

	sol::state lua;
	sol::table typeTable, luaScriptTable;
	std::function<sol::protected_function_result(lua_State*, sol::protected_function_result)> luaErrFunc;

private:
	Framework();
	~Framework() {}
	void LoadScripts();
	void LuaInit();
	void TypeInit();

	std::unique_ptr<RenderSystem> render;
	std::unique_ptr<GameLogic> logic;
	SceneLoader sceneLoader;
	Timer timer;
	double limitedTime = 1 / 60.;
};