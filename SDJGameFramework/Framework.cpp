#include "stdafx.h"
#include "Systems.h"
#include "Framework.h"

void Framework::Init()
{
	// 시스템 초기화
	render.Init();

	ObjectHandle tObj = OM.Add("TestObj", 1.f, 1.f, 0.f);
	ObjectHandle tObj2 = OM.Add("TestObj2", 0.f, 0.f, 2.f);

	ComponentHandle shapeHandle = CM.Add<Shape>();
	Shape* shape = CM.GetBy<Shape>(shapeHandle);
	shape->shapeType = Shape::SPHERE;
	shape->drawParam[0] = 5.f;
	shape->drawParam[1] = 20.f;
	shape->drawParam[2] = 20.f;
	OM.Get(tObj)->AddComponent(shapeHandle);

	shapeHandle = CM.Add<Shape>();
	shape = CM.GetBy<Shape>(shapeHandle);
	shape->shapeType = Shape::CUBE;
	shape->drawParam[0] = 4.f;
	OM.Get(tObj2)->AddComponent(shapeHandle);
}

void Framework::Update(double time)
{
}

void Framework::Render()
{
	render.Render();
}

void Framework::MainLoop()
{
}
