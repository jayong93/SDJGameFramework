#pragma once

#include "Systems.h"
#include "HandleManagers.h"
#include "Message.h"
#include "Action.h"
#include "ActionList.h"

struct Framework
{
	static Framework& Instance()
	{
		static Framework inst;
		return inst;
	}

	void Init();
	void InitLuaFunc();
	void Update(double time);
	void Render();
	void MainLoop();

	sol::state lua;
	std::chrono::system_clock::time_point prevTimePoint;
	RenderSystem render;

private:
	Framework() {}
	~Framework() {}
};