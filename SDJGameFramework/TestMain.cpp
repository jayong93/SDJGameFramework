#include "stdafx.h"

#ifdef SDJ_TEST_BUILD

using namespace std;

#include "Framework.h"

int main()
{
	Framework::Instance().Init();
	sol::state& lua = Framework::Instance().lua;
	lua.do_file("test.lua");
	auto t = lua["t"];
	cout << t.valid() << endl;
	//lua.create_named_table("ObjectManager", "GetByName", [](std::string name) {});
}

#endif