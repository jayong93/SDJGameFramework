#include "stdafx.h"

#ifdef SDJ_TEST_BUILD

#pragma comment(lib, "UnitTest++.lib")
#include "UnitTest++\UnitTest++.h"
#include "Action.h"
#include "ActionList.h"
#include <string>
#include "HandleManagers.h"

TEST(ActionListTest)
{
	class TestAction : public Action
	{
	public:
		TestAction(int& val) : value{ val } {}

		virtual void Update(double time) { value += 5; }

	private:
		int& value;
	};

	class TestAction2 : public Action
	{
	public:
		TestAction2(int& val) : value{ val } {}

		virtual void Update(double time) { value -= 3; }

	private:
		int& value;
	};

	ActionList al;

	int testVal = 0;

	al.PushFront(new TestAction(testVal));
	al.PushFront(new TestAction2(testVal));

	al.Update(5.0);

	CHECK_EQUAL(2, testVal);
}

TEST(StringHashTest)
{
	std::hash<std::string> hashString;

	CHECK_EQUAL(hashString("Test"), hashString("Test"));
}

TEST(CompoListDefineTest)
{
	struct A : public Component
	{
		void Update(double t) {}
		void SendMessage(Message& msg) {}
	};
	CompoList<A> a;
}

TEST(ObjectHandleCreationTest)
{
	ObjectManager om;

	Object *a, *b;

	Handle ah, bh;
	
	ah = om.Add();
	bh = om.Add();

	a = om.Get(ah); b = om.Get(bh);

	CHECK_EQUAL(a->handle==ah, true);
	CHECK_EQUAL(b->handle==bh, true);
	CHECK_EQUAL(0, ah.index);
	CHECK_EQUAL(1, bh.index);
}

TEST(ObjectHandleDeleteTest)
{
	ObjectManager om;
	Handle hd[5];

	for (auto& h : hd)
	{
		h = om.Add();
	}

	om.Delete(hd[2]);

	CHECK_EQUAL(true, nullptr == om.Get(hd[2]));

	Handle nh = om.Add();

	CHECK_EQUAL(true, 2 == nh.index);
	CHECK_EQUAL(true, nullptr == om.Get(hd[2]));
}

TEST(ComponentManagerTest)
{
	class TestCompo : public Component
	{
	public:
		TestCompo() : val{ 0 } {}
		virtual void Update(double time) { val++; }
		virtual void SendMessage(Message& msg) {}
		int val;
	};

	ObjectManager om;
	ObjectHandle handle = om.Add();

	CompoList<TestCompo> clist;

	RegisterComponent(clist);
	ComponentHandle ch = AddComponent(TestCompo, handle);
	Component* compo = CM.Get(ch);
	clist.Update(0.);

	CHECK_EQUAL(true, (1==clist.arr.size()));
	CHECK_EQUAL(true, nullptr != compo);
	CHECK_EQUAL(1, clist.arr[0].val);

	// TODO: 삭제 테스트 해야함
	CM.Detele(ch);
	compo = CM.Get(ch);
	CHECK_EQUAL(true, nullptr == compo);

	ComponentHandle hArr[6];
	for (int i = 0; i < 5; ++i)
	{
		hArr[i] = AddComponent(TestCompo, handle);
	}

	CHECK_EQUAL(2, hArr[0].count);

	compo = CM.Get(hArr[4]);
	((TestCompo*)compo)->val = 100;
	CM.Detele(hArr[2]);
	auto nCompo = CM.GetBy<TestCompo>(hArr[4]);

	CHECK_EQUAL(100, nCompo->val);
}

int main()
{
	return UnitTest::RunAllTests();
}

#endif // TEST
