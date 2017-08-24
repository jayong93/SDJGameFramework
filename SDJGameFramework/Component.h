#pragma once

#include "Handles.h"
#include "Vector3D.h"

struct Message;

/*
모든 컴포넌트들의 조상
owner - 컴포넌트를 소유하는 오브젝트의 핸들
handle - 컴포넌트의 식별자
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