#include "stdafx.h"

#ifdef SDJ_TEST_BUILD

#include "gtest\gtest.h"

#pragma comment(lib, "gtestd.lib")

using namespace std;

#include "Framework.h"

int main(int argc, wchar_t* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
	//auto pfr = lua.safe_script_file("test.lua", [](lua_State* state, sol::protected_function_result pfr) {return pfr; });
	//if (!pfr.valid())
	//{
	//	cout << pfr.get<string>() << endl;
	//}
}

TEST(HandleTest, UsingNumberAsHandle)
{
	auto handle = OM.Add("aaa");
	uint64_t numHandle = handle;
	EXPECT_TRUE(OM.Get(handle) == OM.Get(numHandle));

	OM.Clear();
}

TEST(ObjectTest, Add)
{
	ObjectHandle aaaHandle = OM.Add("aaa");
	OM.Add("bbb");
	OM.Add("bbb");
	EXPECT_TRUE(OM.Size() == 2);

	ObjectHandle namedHandle = OM.GetByName("aaa")->handle;
	EXPECT_TRUE(aaaHandle == namedHandle);
}

struct ObjectTestFixture : public testing::Test
{
	ObjectHandle hObj1;
	ObjectHandle hObj2;
	ObjectHandle hObj3;

	void SetUp()
	{
		hObj1 = OM.Add("obj1");
		hObj2 = OM.Add("obj2");
		hObj3 = OM.Add("obj3");
	}

	void TearDown()
	{
		OM.Clear();
	}
};

TEST_F(ObjectTestFixture, Get)
{
	auto handle = OM.Get(hObj2)->handle;
	EXPECT_TRUE(handle == hObj2);

	EXPECT_TRUE(OM.Get(1234) == nullptr);
}

TEST_F(ObjectTestFixture, GetByName)
{
	auto obj1 = OM.Get(hObj1);
	auto namedObj1 = OM.GetByName("obj1");
	EXPECT_TRUE(obj1 == namedObj1);
	EXPECT_TRUE(namedObj1->name == "obj1");

	EXPECT_TRUE(OM.GetByName("error") == nullptr);
}

TEST_F(ObjectTestFixture, Delete)
{
	OM.Delete(hObj2);
	EXPECT_EQ(OM.Get(hObj2), nullptr);
	EXPECT_EQ(OM.GetByName("obj2"), nullptr);

	EXPECT_NE(OM.GetByName("obj1"), nullptr);
	EXPECT_NE(OM.GetByName("obj3"), nullptr);
	EXPECT_TRUE(OM.GetByName("obj1")->handle == hObj1);
	EXPECT_TRUE(OM.GetByName("obj3")->handle == hObj3);

	OM.Delete(Handle());
	EXPECT_TRUE(OM.Size() == 2);

	OM.Delete(hObj1);
	OM.Delete(hObj3);
	EXPECT_TRUE(OM.Size() == 0);
}

TEST_F(ObjectTestFixture, ReuseFreeIndex)
{
	OM.Delete(hObj2);
	auto hNewObj2 = OM.Add("newObj2");

	EXPECT_FALSE(hObj2 == hNewObj2);
	EXPECT_TRUE(hNewObj2.count == 2);
	EXPECT_EQ(hObj2.index, hNewObj2.index);
	EXPECT_NE(OM.Get(hNewObj2), nullptr);
}

TEST_F(ObjectTestFixture, AddComponent)
{
	CompoList<Shape> sList;
	CM.RegisterComponentList(sList);
	auto compo = CM.Add<Shape>();

	auto obj = OM.Get(hObj1);
	obj->AddComponent(Handle());
	EXPECT_TRUE(obj->compoList.size() == 0 && obj->compoIdxMap.size() == 0);

	obj->AddComponent(compo);
	EXPECT_TRUE(obj->compoIdxMap.find(compo) != obj->compoIdxMap.end());
	EXPECT_TRUE(CM.Get(compo)->owner == hObj1);

	auto obj2 = OM.Get(hObj2);
	obj2->AddComponent(compo);
	EXPECT_TRUE(CM.Get(compo)->owner == hObj1);

	CM.ClearAndUnregister();
}

TEST_F(ObjectTestFixture, DelComponent)
{
	CompoList<Shape> sList;
	CM.RegisterComponentList(sList);
	auto compo = CM.Add<Shape>();
	auto compo2 = CM.Add<Shape>();

	auto obj = OM.Get(hObj1);
	obj->AddComponent(compo);
	obj->AddComponent(compo2);
	obj->DelComponent(compo);
	EXPECT_TRUE(obj->compoIdxMap.find(compo) == obj->compoIdxMap.end());
	EXPECT_TRUE(obj->compoList[0] == compo2);

	obj->DelComponent(compo2);
	EXPECT_TRUE(obj->compoList.size() == 0 && obj->compoIdxMap.size() == 0);

	CM.ClearAndUnregister();
}

TEST_F(ObjectTestFixture, HasComponent)
{
	CompoList<Shape> sList;
	CM.RegisterComponentList(sList);
	auto compo = CM.Add<Shape>();
	auto compo2 = CM.Add<Shape>();

	auto obj = OM.Get(hObj1);
	obj->AddComponent(compo);
	obj->AddComponent(compo2);

	EXPECT_TRUE(obj->HasComponent("Shape"));
	obj->DelComponent(compo);
	EXPECT_TRUE(obj->HasComponent("Shape"));
	obj->DelComponent(compo2);
	EXPECT_FALSE(obj->HasComponent("Shape"));

	CM.ClearAndUnregister();
}

TEST(ComponentTest, Add)
{
	CompoList<Shape> sList;
	CM.RegisterComponentList(sList);

	auto handle = CM.Add<Shape>();
	auto handle2 = CM.Add<Shape>();
	EXPECT_TRUE(handle.index == 0 && handle.count == 1);
	EXPECT_TRUE(handle2.index == 1 && handle2.count == 1);

	EXPECT_TRUE(CM.Get(handle)->handle == handle);
	EXPECT_TRUE(CM.Get(handle2)->handle == handle2);

	CM.ClearAndUnregister();
}

struct ComponentTestFixture : testing::Test
{
	CompoList<Shape> shapeList;
	ComponentHandle hCompo[3];

	void SetUp()
	{
		CM.RegisterComponentList(shapeList);
		for (int i = 0; i < sizeof(hCompo) / sizeof(ComponentHandle); ++i)
		{
			hCompo[i] = CM.Add<Shape>();
		}

	}

	void TearDown()
	{
		CM.ClearAndUnregister();
		OM.Clear();
	}
};

TEST_F(ComponentTestFixture, Delete)
{
	auto objHandle = OM.Add("test");
	auto obj = OM.Get(objHandle);

	obj->AddComponent(hCompo[1]);
	CM.Detele(hCompo[1]);
	CM.Detele(Handle());
	EXPECT_TRUE(obj->compoList.size() == 0);
	EXPECT_TRUE(CM.Get(hCompo[0]) != nullptr);
	EXPECT_TRUE(CM.Get(hCompo[2]) != nullptr);

	CM.Detele(hCompo[0]);
	CM.Detele(hCompo[2]);
	EXPECT_TRUE(CM.Size<Shape>() == 0);
}

TEST_F(ComponentTestFixture, ReuseFreeIndex)
{
	CM.Detele(hCompo[1]);
	auto newCompo = CM.Add<Shape>();
	EXPECT_TRUE(hCompo[1].index == newCompo.index && hCompo[1].count != newCompo.count);
	EXPECT_TRUE(newCompo.count == 2);
	EXPECT_TRUE(CM.Get(newCompo) != nullptr);
}

TEST_F(ComponentTestFixture, Type)
{
	EXPECT_TRUE(CM.Type(hCompo[1]) == GetHash("Shape"));
	auto c = CM.GetBy<Shape>(hCompo[1]);
}

struct LuaTestFixture : public testing::Test
{
	sol::state lua;
	CompoList<Shape> shapeList;
	ObjectHandle obj1, obj2;
	function<sol::protected_function_result(lua_State*, sol::protected_function_result)> errFn;

	void SetUp()
	{
		errFn = [](lua_State* state, sol::protected_function_result pfr) {
			if (!pfr.valid())
				cout << pfr.get<string>() << endl;
			return pfr;
		};
		LuaStateInitialize(lua);
		CM.RegisterComponentList(shapeList);
		obj1 = OM.Add("obj1");
		obj2 = OM.Add("obj2");
		OM.Get(obj1)->AddComponent(CM.Add<Shape>());
		OM.Get(obj2)->AddComponent(CM.Add<Shape>());
	}

	void TearDown()
	{
		OM.Clear();
		CM.ClearAndUnregister();
	}
};

TEST_F(LuaTestFixture, ControlObject)
{
	lua.safe_script(R"(
obj1 = GetObject("obj1")
obj1:MoveTo(3,4,5)
	)", errFn
	);

	// GetObject 테스트
	uint64_t hObj1 = lua["obj1"]["handle"];
	EXPECT_TRUE(hObj1 == uint64_t(obj1));

	// MoveTo 테스트
	auto obj1Ptr = OM.Get(obj1);
	EXPECT_TRUE(obj1Ptr->position == Vector3D(3, 4, 5));

	// Move 테스트
	lua.safe_script(R"(obj1:Move(1,0,1))", errFn);
	EXPECT_TRUE(obj1Ptr->position == Vector3D(4, 4, 6));

	// Position 테스트
	double x, y, z;
	sol::tie(x,y,z) = lua["obj1"]["Position"](lua["obj1"]);
	EXPECT_TRUE(x == 4.);
	EXPECT_TRUE(y == 4.);
	EXPECT_TRUE(z == 6.);

	lua.safe_script(R"(ret = obj1:Has("Shape"))", errFn);
	EXPECT_TRUE(lua["ret"].get<bool>());
}

TEST_F(LuaTestFixture, ControlComponentViaMessage)
{
	auto hc1 = OM.Get(obj1)->compoList[0];
	auto hc2 = OM.Get(obj2)->compoList[0];
	auto compo1 = CM.GetBy<Shape>(hc1);
	auto compo2 = CM.GetBy<Shape>(hc2);

	lua.safe_script(R"(
obj1 = GetObject("obj1")
obj1:SendMsg{"CHANGE_SHAPE",type="sphere",radius=5,slice=4,stack=3}
GetObject("obj2"):SendMsg{"CHANGE_SHAPE",type="cube",size=2}
)", errFn);

	EXPECT_TRUE(compo1->shapeType == Shape::SPHERE);
	EXPECT_TRUE(compo2->shapeType == Shape::CUBE);
	EXPECT_TRUE(compo1->drawParam[0] == 5 && compo1->drawParam[1] == 4 && compo1->drawParam[2] == 3);
	EXPECT_TRUE(compo2->drawParam[0] == 2);
}

#endif