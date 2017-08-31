#include "stdafx.h"
#include "Systems.h"
#include "Framework.h"
#include "HandleManagers.h"
#include "MessageManager.h"

using namespace rapidjson;

void Framework::Init()
{
	// 시스템 초기화
	LuaStateInitialize(lua);

	render = std::make_unique<RenderSystem>();
	render->Init();
	logic = std::make_unique<GameLogic>();
	logic->Init();
	timer.Init();
}

void Framework::Update(double time)
{
	logic->Update(time);
}

void Framework::Render()
{
	render->Render();
}

void Framework::MainLoop()
{
	double timeElapsed = timer.GetElapsedTime();
#ifdef SDJ_TEST_BUILD
	timer.UpdateTimePoint();
	Update(timeElapsed);
#else
	if (limitedTime <= 0 || timeElapsed >= limitedTime)
	{
		timer.UpdateTimePoint();
		Update(timeElapsed);
		glutPostRedisplay();
	}
	else if (limitedTime <= 0 || timeElapsed < limitedTime - 0.005)
		lua.collect_garbage();
#endif
}

void Framework::SetView(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10., 10., -10., 10., -100., 100.);
}

void Framework::LoadScene(const std::string & fileName)
{
	sceneLoader.LoadScene(fileName, lua);
}

void Framework::CleanUp()
{
	render.reset();
	logic.reset();

	OM.Clear();
	CM.ClearAndUnregister();
	MM.Clear();
	timer.Clear();
#ifdef SDJ_TEST_BUILD
	lua = sol::state{};
#endif
}

Framework::Framework()
{
	luaErrFunc = [](lua_State* state, sol::protected_function_result pfr) {
		if (!pfr.valid())
		{
			std::cout << pfr.get<std::string>() << std::endl;
		}
		return pfr;
	};
}
