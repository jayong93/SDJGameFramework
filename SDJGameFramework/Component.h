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

/*
������Ʈ ����
InitMsgMap() - ó���� �޼����� �ڵ鷯�� ������� MessageMap�� �ʱ�ȭ
InitGetSetFunc() - ��ƿ��� ������Ʈ�� ������ ������ �̸��� �ڵ鷯�� ����
*/

/*
GLUT���� �����ϴ� �⺻ 3���� �������� ��Ÿ���� ������Ʈ
����		-	����		-	��ƿ����� ���� �̸�
shapeType - ���� ���� - "type"
param - GLUT �׸��� �Լ��� ���ڵ� - "typeParam" (ex. cubeSize, torusInnerRadius)
color - ������ �� - "color"
typeNames, typeMap - Ÿ�� enum�� string�� ��ȯ ������
*/
struct Shape : public Component
{
	virtual ~Shape() {}

	enum Type { NONE, CUBE, SPHERE, CONE, TORUS, TEAPOT } shapeType = NONE;
	union
	{
		float param[4] = { 0.,0.,0.,0. };
		struct { float size; } cube;
		struct { float size; } teapot;
		struct { float radius, slice, stack; } sphere;
		struct { float innerRadius, outerRadius, side, ring; } torus;
		struct { float base, height, slice, stack; } cone;
	};
	Vector3D color;

	static const char* typeNames[];
	const static StringHashMap<unsigned> typeMap;
	static MessageMap InitMsgMap() { return MessageMap(); }
	static void RegisterInLua();

private:
	static StringHashMap<unsigned> InitTypeMap();
};

/*
��Ʒ� �ۼ��� ��ũ��Ʈ ������Ʈ
env - ��ũ��Ʈ���� ������ ȯ�� ���̺�. ���� ȯ���� �������°� ���´�.
scriptName - ���� ������Ʈ ����
*/
struct LuaComponent : public Component
{
	virtual ~LuaComponent();
	bool SetScript(const std::string& name);
	virtual void SendMsg(sol::object& args);
	static void RegisterInLua() {}
	static MessageMap InitMsgMap() { return MessageMap(); }

	sol::environment env;
	std::string scriptName;
};