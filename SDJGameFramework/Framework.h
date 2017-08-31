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

	sol::state lua;
	double limitedFrame = 60.;

	std::function<sol::protected_function_result(lua_State*, sol::protected_function_result)> luaErrFunc;

private:
	Framework();
	~Framework() {}

	std::unique_ptr<RenderSystem> render;
	std::unique_ptr<GameLogic> logic;
	SceneLoader sceneLoader;
	Timer timer;
};