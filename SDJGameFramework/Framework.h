#pragma once

#include "Systems.h"
#include "HandleManagers.h"
#include "Message.h"
#include "Action.h"
#include "ActionList.h"

class Framework
{
public:
	void Init(HWND hwnd, HINSTANCE hinstance, HDC& memDC);
	void Update(double time);
	void MainLoop();

private:
	std::chrono::system_clock::time_point prevTimePoint;

	HWND hWnd;
	HINSTANCE hInst;

	Graphic graphic;
};