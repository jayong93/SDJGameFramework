#include "stdafx.h"
#include "Vertex.h"
#include "Systems.h"
#include "Framework.h"
#include "HandleManagers.h"
#include "MessageManager.h"

using namespace rapidjson;

void Framework::Init()
{
	// 시스템 초기화
	LuaInit();
	TypeInit();
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
#ifdef SDJ_TEST_BUILD
	render.reset();
	logic.reset();

	OM.Clear();
	CM.ClearAndUnregister();
	MM.Clear();
	timer.Clear();

	luaScriptTable = sol::table();
	typeTable = sol::table();
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
			luaScriptTable.set(fData.cFileName, fn);
		} while (FindNextFileA(ret, &fData));

		FindClose(ret);
	}
}

void Framework::LuaInit()
{
	lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);
	luaScriptTable = sol::table(lua, sol::create);
	typeTable = sol::table(lua, sol::create);
}

void Framework::TypeInit()
{
	Vector3D::RegisterInLua(lua);
	Vertex::RegisterInLua(typeTable);
	Object::RegisterInLua();
	Component::RegisterInLua();
	Shape::RegisterInLua();
}
