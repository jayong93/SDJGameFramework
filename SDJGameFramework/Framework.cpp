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
	render.Init();
	logic.Init();
	timer.Init();
}

void Framework::Update(double time)
{
	logic.Update(time);
}

void Framework::Render()
{
	render.Render();
}

void Framework::MainLoop()
{
	double timeElapsed = timer.GetElapsedTime();
	double limitedTime = (1 / limitedFrame);
#ifdef SDJ_TEST_BUILD
	timer.UpdateTimePoint();
	Update(timeElapsed);
#else
	if (timeElapsed >= limitedTime)
	{
		timer.UpdateTimePoint();
		Update(timeElapsed);
		glutPostRedisplay();
	}
	else if (timeElapsed < limitedTime - 0.005)
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
	std::ifstream ifs{ fileName };
	if (ifs.bad()) return;

	IStreamWrapper isw{ ifs };
	Document doc;
	doc.ParseStream(isw);
	if (doc.HasParseError()) return;

	// Component 초기화
	auto& compos = doc["component"];
	if (compos.IsArray())
	{
		std::string compoName;
		for (auto& c : compos.GetArray())
		{
			compoName = c.GetString();
			std::string fullName = compoName; fullName += ".lua";
			std::ifstream compoScript{ fullName };
			if (compoScript.bad()) continue;

			sol::protected_function fn = lua.load_file(fullName);
			if (!fn.valid()) continue;
			lua["Component"]["prototype"][compoName] = fn;
		}
	}

	// Object 초기화
	auto& objs = doc["object"];
	for (auto& o : objs.GetArray())
	{
		auto& data = o.GetObject();

		auto& posData = data["position"].GetArray();
		Vector3D pos;
		for (int i = 0; i < 3; ++i)
			pos.data[i] = posData[i].GetDouble();

		auto hObj = OM.Add(data["name"].GetString(), pos);

		auto& compoList = data["component"].GetArray();
		for (auto& c : compoList)
		{
			auto compoName = c.GetString();
			if (CM.IsRegistered(compoName))
				CM.Add(compoName, hObj);
			else
				CM.AddLuaComponent(c.GetString(), hObj);
		}
	}
}

void Framework::CleanUp()
{
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
