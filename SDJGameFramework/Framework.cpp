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
	LoadScripts();

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

void Framework::LoadScripts()
{
	WIN32_FIND_DATAA fData;
	HANDLE ret = FindFirstFileA(R"(.\script\*.lua)", &fData);

	if (ret != INVALID_HANDLE_VALUE)
	{
		const char* path = R"(.\script\)";
		do
		{
			char fullName[256];
			strcpy_s(fullName, path);
			strcat_s(fullName, fData.cFileName);
			std::ifstream compoScript{ fullName };
			if (compoScript.bad()) continue;

			sol::protected_function fn = lua.load_file(fullName);
			if (!fn.valid()) continue;

			size_t extStartIdx = strlen(fData.cFileName) - 4;
			fData.cFileName[extStartIdx] = 0;
			lua["Component"]["prototype"][fData.cFileName] = fn;

			size_t type = GetHash(fData.cFileName);
			lua["Component"]["get"][type] = lua.create_table();
			lua["Component"]["set"][type] = lua.create_table();
		} while (FindNextFileA(ret, &fData));
	}
}
