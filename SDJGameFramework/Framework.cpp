#include "stdafx.h"
#include "Systems.h"
#include "Framework.h"

void Framework::Init(HWND hwnd, HINSTANCE hinstance, HDC& memDC)
{
	this->hWnd = hwnd;
	this->hInst = hinstance;

	graphic.Init(hWnd, memDC);

	ObjectHandle tempObj = OM.Add("tempObj");
	ObjectHandle tempObj2 = OM.Add("tempObj2");
	ComponentHandle tempCompo = AddComponent(EllipseComponent, tempObj);
	ComponentHandle tempCompo2 = AddComponent(EllipseComponent, tempObj2);

	auto obj = OM.Get(tempObj);
	auto obj2 = OM.Get(tempObj2);
	auto compo = CM.GetBy<EllipseComponent>(tempCompo);
	auto compo2 = CM.GetBy<EllipseComponent>(tempCompo2);

	obj->position.Set(100, 100);
	obj2->position.Set(110, 100);
	compo->penWidth = 5;
	compo->radius = 20;
	compo->brushColor.Set(255, 0, 0);
	compo->penColor.Set(0, 0, 255);
	compo->order = 20;

	compo2->penWidth = 2;
	compo2->radius = 20;
	compo2->brushColor.Set(155, 0, 0);
	compo2->penColor.Set(0, 0, 155);
	compo2->order = 10;

	prevTimePoint = std::chrono::system_clock::now();
}

void Framework::Update(double time)
{
	graphic.Update();
}

void Framework::MainLoop()
{
	auto timePoint = std::chrono::system_clock::now();

	std::chrono::duration<double> timeElapsed = timePoint - prevTimePoint;

	if (timeElapsed.count() >= 1 / 60.)
	{
		Update(timeElapsed.count());

		prevTimePoint = timePoint;
	}
}
