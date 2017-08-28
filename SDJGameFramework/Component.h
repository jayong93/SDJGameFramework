#pragma once

#include "Handles.h"
#include "Vector3D.h"
#include "typedef.h"

using GetSetMap = std::unordered_map<std::string, sol::protected_function>;

/*
��� ������Ʈ���� ����
owner - ������Ʈ�� �����ϴ� ������Ʈ�� �ڵ�
handle - ������Ʈ�� �ĺ���
*/
struct Component
{
	virtual ~Component() = 0;

	virtual void SendMsg(sol::object& args);

	ObjectHandle owner;
	ComponentHandle handle;
};

struct Shape : public Component
{
	virtual ~Shape() {}

	enum Type { NONE, CUBE, SPHERE, CONE, TORUS, TEAPOT } shapeType = NONE;
	double drawParam[4] = { 0.f, 0.f, 0.f, 0.f };
	Vector3D color;
	int test;

	const static StringHashMap<unsigned> typeMap;
	static MessageMap InitMsgMap();
	static void GetSetFunc();

private:
	static StringHashMap<unsigned> InitTypeMap();
};

struct LuaComponent : public Component
{
	virtual ~LuaComponent();
	bool SetScript(const std::string& name);
	virtual void SendMsg(sol::object& args);
	static void GetSetFunc() {}
	static MessageMap InitMsgMap() { return MessageMap(); }

	sol::environment env;
	std::string scriptName;
};