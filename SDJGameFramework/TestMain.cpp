#include "stdafx.h"

#ifdef SDJ_TEST_BUILD

#include "gtest\gtest.h"

#pragma comment(lib, "gtestd.lib")

using namespace std;

#include "headers.h"

int main(int argc, wchar_t* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

TEST(HandleTest, UsingNumberAsHandle)
{
	FW.Init();
	auto handle = OM.Add("aaa");
	uint64_t numHandle = handle;
	EXPECT_TRUE(OM.Get(handle) == OM.Get(numHandle));

	OM.Clear();
	FW.CleanUp();
}

TEST(ObjectTest, Add)
{
	FW.Init();
	ObjectHandle aaaHandle = OM.Add("aaa");
	OM.Add("bbb");
	OM.Add("bbb");
	EXPECT_TRUE(OM.Size() == 2);

	ObjectHandle namedHandle = OM.GetByName("aaa")->handle;
	EXPECT_TRUE(aaaHandle == namedHandle);
	FW.CleanUp();
}

struct ObjectTestFixture : public testing::Test
{
	ObjectHandle hObj1;
	ObjectHandle hObj2;
	ObjectHandle hObj3;

	void SetUp()
	{
		FW.Init();
		hObj1 = OM.Add("obj1");
		hObj2 = OM.Add("obj2");
		hObj3 = OM.Add("obj3");
	}

	void TearDown()
	{
		FW.CleanUp();
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
	auto obj = OM.Get(hObj1);
	obj->AddComponent(Handle());
	EXPECT_TRUE(obj->compoList.size() == 0 && obj->compoIdxMap.size() == 0);

	auto compo = CM.Add<Shape>(hObj1);
	EXPECT_TRUE(obj->compoIdxMap.find(compo) != obj->compoIdxMap.end());
	EXPECT_TRUE(CM.Get(compo)->owner == hObj1);

	auto obj2 = OM.Get(hObj2);
	obj2->AddComponent(compo);
	EXPECT_TRUE(CM.Get(compo)->owner == hObj1);
	EXPECT_TRUE(obj2->compoList.size() == 0);
}

TEST_F(ObjectTestFixture, DelComponent)
{
	auto obj = OM.Get(hObj1);
	auto compo = CM.Add<Shape>(hObj1);
	auto compo2 = CM.Add<Shape>(hObj2);
	obj->DelComponent(compo);
	EXPECT_TRUE(obj->compoIdxMap.find(compo) == obj->compoIdxMap.end());
	EXPECT_TRUE(obj->compoList.size() == 0);

	obj = OM.Get(hObj2);
	obj->DelComponent(compo2);
	EXPECT_TRUE(obj->compoList.size() == 0 && obj->compoIdxMap.size() == 0);
}

TEST_F(ObjectTestFixture, HasComponent)
{
	auto obj = OM.Get(hObj1);
	auto obj2 = OM.Get(hObj2);
	auto compo = CM.Add<Shape>(hObj1);
	auto compo2 = CM.Add<Shape>(hObj2);

	EXPECT_TRUE(obj->HasComponent("Shape"));
	EXPECT_TRUE(obj2->HasComponent("Shape"));

	obj->DelComponent(compo);
	obj2->DelComponent(compo2);
	EXPECT_FALSE(obj->HasComponent("Shape"));
	EXPECT_FALSE(obj2->HasComponent("Shape"));
}

TEST(ComponentTest, Add)
{
	FW.Init();
	auto obj = OM.Add("test");
	auto obj2 = OM.Add("test2");

	auto handle = CM.Add<Shape>(obj);
	auto handle2 = CM.Add<Shape>(obj2);
	EXPECT_TRUE(handle.index == 0 && handle.count == 1);
	EXPECT_TRUE(handle2.index == 1 && handle2.count == 1);

	EXPECT_TRUE(CM.Get(handle)->handle == handle);
	EXPECT_TRUE(CM.Get(handle2)->handle == handle2);

	auto errHandle = CM.Add<Shape>(obj);
	EXPECT_FALSE(errHandle);
	EXPECT_TRUE(CM.Size<Shape>() == 2);

	auto errHandle2 = CM.Add("wrong", obj);
	EXPECT_FALSE(errHandle);
	EXPECT_TRUE(OM.Get(obj)->compoList.size() == 1);
	EXPECT_FALSE(OM.Get(obj)->HasComponent("wrong"));

	auto errHandle3 = CM.AddLuaComponent("abc", obj);
	EXPECT_FALSE(errHandle);
	EXPECT_TRUE(OM.Get(obj)->compoList.size() == 1);
	EXPECT_FALSE(OM.Get(obj)->HasComponent("abc"));

	FW.CleanUp();
}

struct ComponentTestFixture : testing::Test
{
	ComponentHandle hCompo[3];
	ObjectHandle hObj[3];

	void SetUp()
	{
		FW.Init();
		for (int i = 0; i < sizeof(hCompo) / sizeof(ComponentHandle); ++i)
		{
			std::string objName{ "obj" };
			objName += '0' + i + 1;
			hObj[i] = OM.Add(objName);
			hCompo[i] = CM.Add<Shape>(hObj[i]);
		}

	}

	void TearDown()
	{
		FW.CleanUp();
	}
};

TEST_F(ComponentTestFixture, Delete)
{
	auto objHandle = OM.Add("test");
	auto obj = OM.Get(objHandle);

	CM.Delete(hCompo[1]);
	CM.Delete(Handle());
	EXPECT_TRUE(obj->compoList.size() == 0);
	EXPECT_TRUE(CM.Get(hCompo[0]) != nullptr);
	EXPECT_TRUE(CM.Get(hCompo[2]) != nullptr);

	CM.Delete(hCompo[0]);
	CM.Delete(hCompo[2]);
	EXPECT_TRUE(CM.Size<Shape>() == 0);
}

TEST_F(ComponentTestFixture, ReuseFreeIndex)
{
	OM.Get(hObj[1])->DelComponent(hCompo[1]);
	auto newCompo = CM.Add<Shape>(hObj[1]);
	EXPECT_TRUE(hCompo[1].index == newCompo.index && hCompo[1].count != newCompo.count);
	EXPECT_TRUE(newCompo.count == 2);
	EXPECT_TRUE(CM.Get(newCompo) != nullptr);
}

TEST_F(ComponentTestFixture, Type)
{
	EXPECT_TRUE(CM.Type(hCompo[1]) == GetHash("Shape"));
}

struct LuaTestFixture : public testing::Test
{
	ObjectHandle obj1, obj2;
	ComponentHandle compo1, compo2;
	function<sol::protected_function_result(lua_State*, sol::protected_function_result)> errFn;

	void SetUp()
	{
		errFn = [](lua_State* state, sol::protected_function_result pfr) {
			if (!pfr.valid())
				cout << pfr.get<string>() << endl;
			return pfr;
		};
		FW.Init();
		obj1 = OM.Add("obj1");
		obj2 = OM.Add("obj2");
		compo1 = CM.Add<Shape>(obj1);
		compo2 = CM.Add<Shape>(obj2);
	}

	void TearDown()
	{
		FW.CleanUp();
	}
};

TEST_F(LuaTestFixture, SolTable)
{
	sol::state_view lua{ FW.lua };
	lua.safe_script("t = {}");
	auto t = lua["t"].get<sol::optional<sol::table>>();
	ASSERT_TRUE(t);

	lua.safe_script("t.a = 10");
	auto a = (*t)["a"].get<sol::optional<int>>();
	ASSERT_TRUE(a);
	EXPECT_TRUE(*a == 10);
}

TEST_F(LuaTestFixture, ControlObject)
{
	auto& lua = FW.lua;
	lua.safe_script(R"(
obj1 = objects.obj1
obj1.position = Vector.new(3,4,5)
	)", errFn
	);

	// GetObject 테스트
	ObjectHandle hObj1 = lua["obj1"]["handle"];
	EXPECT_TRUE(hObj1 == obj1);

	// MoveTo 테스트
	auto obj1Ptr = OM.Get(obj1);
	EXPECT_TRUE(obj1Ptr->position == Vector3D(3, 4, 5));

	// Move 테스트
	lua.safe_script(R"(obj1:Move(1,0,1))", errFn);
	EXPECT_TRUE(obj1Ptr->position == Vector3D(4, 4, 6));

	// Position 테스트
	Vector3D pos = lua["obj1"]["position"].get<Vector3D>();
	EXPECT_TRUE(pos == Vector3D(4.f, 4.f, 6.f));

	lua.safe_script(R"(ret = obj1.component.Shape)", errFn);
	EXPECT_TRUE(lua["ret"].get_type() == sol::type::userdata);
}

TEST_F(LuaTestFixture, LuaEnvironment)
{
	auto& lua = FW.lua;
	sol::environment env{ lua,sol::create,lua.globals() };
	lua["env"] = env;
	sol::table a = lua.create_named_table("a");
	sol::protected_function fn = lua.load(R"(function test()
b = 5
end
a.c=4
)");
	sol::set_environment(env, fn);
	fn();

	sol::object obj = a["c"];
	EXPECT_TRUE(obj.as<int>() == 4);

	sol::table mt = lua.create_table();
	mt["__newindex"] = mt;
	a[sol::metatable_key] = mt;

	a["d"] = 10;
	obj = mt["d"];
	EXPECT_TRUE(obj.as<int>() == 10);

	a.push();
	lua_pushstring(lua, "d");
	lua_pushinteger(lua, 10);
	lua_rawset(lua, -3);
	a.pop();
	obj = a["d"];
	EXPECT_TRUE(obj.as<int>() == 10);

	obj = lua["test"];
	EXPECT_TRUE(obj.get_type() == sol::type::nil);

	sol::protected_function testFn = lua["env"]["test"];
	EXPECT_TRUE(testFn.valid());
	testFn();
	obj = lua["b"];
	EXPECT_TRUE(obj.get_type() == sol::type::nil);
	obj = lua["env"]["b"];
	EXPECT_TRUE(obj.get_type() == sol::type::number);
}

TEST_F(LuaTestFixture, ControlComponentViaGetSet)
{
	auto& lua = FW.lua;
	Shape* c1 = CM.GetBy<Shape>(OM.Get(obj1)->compoList[0]);
	Shape* c2 = CM.GetBy<Shape>(OM.Get(obj2)->compoList[0]);
	c1->shapeType = Shape::CUBE;
	c1->cube.size = 4;
	c1->color.Set(0.4f, 0.4f, 0.2f);

	lua.safe_script(R"(
obj1 = objects.obj1
compo1 = obj1.component.Shape
size, color = compo1.cubeSize, compo1.color
c2 = objects.obj2.component.Shape
c2.type, c2.sphereRadius, c2.sphereSlice, c2.sphereStack = "sphere", 10, 20, 30
)", errFn);

	sol::optional<Vector3D> color = lua["color"];
	sol::object size = lua["size"];
	ASSERT_TRUE(color);
	EXPECT_TRUE(*color == Vector3D(0.4f, 0.4f, 0.2f));
	EXPECT_TRUE(size.get_type() == sol::type::number);
	EXPECT_TRUE(size.as<float>() == c1->cube.size);

	lua.safe_script("compo1.color = {1, 2, 3}");
	EXPECT_TRUE(c1->color == Vector3D(1, 2, 3));
	lua.safe_script("compo1.color = Vector.new(9,2,4)");
	EXPECT_TRUE(c1->color == Vector3D(9, 2, 4));

	EXPECT_TRUE(c2->shapeType == Shape::SPHERE);
	EXPECT_TRUE(c2->sphere.radius == 10);
	EXPECT_TRUE(c2->sphere.slice == 20);
	EXPECT_TRUE(c2->sphere.stack == 30);
}

TEST_F(LuaTestFixture, Vector3DUserType)
{
	sol::state_view lua{ FW.lua };

	Vector3D v1{ 9.f, 2.f, 4.f };
	lua["v1"] = v1;
	auto v1x = lua["v1"]["x"].get<sol::optional<float>>();
	auto v1y = lua["v1"]["y"].get<sol::optional<float>>();
	auto v1z = lua["v1"]["z"].get<sol::optional<float>>();
	ASSERT_TRUE(v1x && v1y && v1z);
	EXPECT_TRUE(*v1x == 9.f);
	EXPECT_TRUE(*v1y == 2.f);
	EXPECT_TRUE(*v1z == 4.f);

	lua.safe_script(R"(
v2 = Vector.new(2, 0, 6)
v2norm = v2.normal + Vector.new(0,1,0)
print(v2norm.x, v2norm.y, v2norm.z)
)");
	auto v2 = lua["v2"].get<sol::optional<Vector3D>>();
	ASSERT_TRUE(v2);
	EXPECT_TRUE(v2->x == 2.f);
	EXPECT_TRUE(v2->y == 0.f);
	EXPECT_TRUE(v2->z == 6.f);
}

struct MainFrameworkTestFixture : public testing::Test
{
	MainFrameworkTestFixture()
	{
		FW.Init();
	}

	~MainFrameworkTestFixture()
	{
		FW.CleanUp();
	}
};

TEST_F(MainFrameworkTestFixture, SceneLoading)
{
	FW.LoadScene("TestScene.json");

	EXPECT_TRUE(OM.Size() == 4);
	Object* objArr[4] = { nullptr, nullptr, nullptr, nullptr };
	for (int i = 0; i < 4; ++i)
	{
		string base{ "obj" };
		base += '1' + i;
		objArr[i] = OM.GetByName(base);
		ASSERT_TRUE(objArr[i]);
	}

	Vector3D expectedPos[4] = { {1,1},{1,2},{3,1},{1,4} };
	for (int i = 0; i < 4; ++i)
	{
		EXPECT_TRUE(objArr[i]->position == expectedPos[i]);
	}

	auto& compoTable = FW.lua["Component"].get<sol::table>();
	ASSERT_TRUE(compoTable);
	auto& instances = FW.lua["components"].get<sol::table>();
	ASSERT_TRUE(instances);

	int i = 0;
	instances.for_each([&i](auto& a, auto& b) {i++; });
	EXPECT_TRUE(i == 4);

	FW.lua["a"] = 10;
	FW.MainLoop();

	EXPECT_TRUE(FW.lua["a"].get<int>() == 10);
	for (auto& o : objArr)
	{
		LuaComponent* compo = CM.GetBy<LuaComponent>(o->GetComponent("Plus"));
		EXPECT_TRUE(compo->env["a"] == 30);
	}

	for (int i = 0; i < 4; ++i)
	{
		expectedPos[i].z -= 2;
		EXPECT_TRUE(objArr[i]->position == expectedPos[i]);
	}
}

TEST_F(MainFrameworkTestFixture, SceneLoadingWithCompoVar)
{
	FW.LoadScene("TestScene2.json");

	auto obj1 = OM.GetByName("obj1");
	ASSERT_TRUE(obj1);

	auto shape1 = CM.GetBy<Shape>(obj1->GetComponent("Shape"));
	ASSERT_TRUE(shape1);
	EXPECT_TRUE(shape1->shapeType == Shape::Type::CUBE);
	EXPECT_TRUE(shape1->cube.size == 5.f);
	EXPECT_TRUE(shape1->color == Vector3D(1.f, 0.f, 0.f));

	auto luaCompo = CM.GetBy<LuaComponent>(obj1->GetComponent("Plus"));
	sol::optional<int> testVar = luaCompo->env["testVar"];
	EXPECT_TRUE(testVar);
	EXPECT_TRUE(testVar.value() == 50);

	FW.lua.safe_script(R"(
obj1 = objects.obj1
compo1 = obj1.component.Plus
testVar, nilVar = compo1.testVar, compo1.fsf
)", FW.luaErrFunc);
	sol::optional<int> testVarInLua = FW.lua["testVar"];
	ASSERT_TRUE(testVarInLua);
	EXPECT_TRUE(testVarInLua.value() == 50);
	EXPECT_TRUE(FW.lua["nilVar"].get_type() == sol::type::none);

	auto obj2 = OM.GetByName("obj4");
	ASSERT_TRUE(obj2);

	auto shape2 = CM.GetBy<Shape>(obj2->GetComponent("Shape"));
	ASSERT_TRUE(shape2);
	EXPECT_TRUE(shape2->shapeType == Shape::Type::SPHERE);
	EXPECT_TRUE(shape2->sphere.radius == 3);
	EXPECT_TRUE(shape2->sphere.slice == 20);
	EXPECT_TRUE(shape2->sphere.stack == 20);
}
#endif