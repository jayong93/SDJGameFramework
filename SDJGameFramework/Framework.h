#pragma once

#include "Systems.h"
#include "HandleManagers.h"
#include "Message.h"
#include "Action.h"
#include "ActionList.h"

struct Framework
{
	void Init();
	void Update(double time);
	void Render();
	void MainLoop();

	std::chrono::system_clock::time_point prevTimePoint;
	RenderSystem render;
};