#pragma once

#include "Handles.h"
#include "Vector3D.h"

struct Message;

/*
��� ������Ʈ���� ����
owner - ������Ʈ�� �����ϴ� ������Ʈ�� �ڵ�
handle - ������Ʈ�� �ĺ���
*/
struct Component
{
	virtual ~Component() {}

	virtual void SendMsg(Message& msg) {};

	ObjectHandle owner;
	ComponentHandle handle;
};

struct ShapeCompo : public Component
{
	virtual ~ShapeCompo() {}

	enum {NONE, CUBE, SPHERE, CONE, TORUS, TEAPOT} shapeType;
	double drawParam[4] = { 0.f, 0.f, 0.f, 0.f };
	Vector3D color;
};

struct LuaCompo : public Component
{
	virtual ~LuaCompo() {}

	std::string scriptName;
};