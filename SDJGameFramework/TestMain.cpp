#include "stdafx.h"

#ifdef SDJ_TEST_BUILD

#include "gtest\gtest.h"

#pragma comment(lib, "gtestd.lib")

using namespace std;

#include "Framework.h"

int main(int argc, wchar_t* argv[])
{
	OM.Add("test");
	OM.Add("Test");
	OM.Add("tEST");

	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
	//auto pfr = lua.safe_script_file("test.lua", [](lua_State* state, sol::protected_function_result pfr) {return pfr; });
	//if (!pfr.valid())
	//{
	//	cout << pfr.get<string>() << endl;
	//}
}

TEST(ObjectTest, ObjectManagerAdd)
{
	ObjectHandle aaaHandle = OM.Add("aaa");
	EXPECT_TRUE(OM.Size() == 4);

	ObjectHandle namedHandle = OM.GetByName("aaa")->handle;
	EXPECT_TRUE(aaaHandle == namedHandle);
}

#endif