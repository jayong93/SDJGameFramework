#pragma once

#include "Handles.h"
#include "Vector3D.h"
#include "typedef.h"

/*
모든 컴포넌트들의 조상
owner - 컴포넌트를 소유하는 오브젝트의 핸들
handle - 컴포넌트의 식별자
*/
struct Component
{
	virtual ~Component() {}

	void SendMsg(sol::object& args);
	static MessageMap InitMsgMap() { return MessageMap(); }

	ObjectHandle owner;
	ComponentHandle handle;
};

struct Shape : public Component
{
	virtual ~Shape() {}

	enum Type { NONE, CUBE, SPHERE, CONE, TORUS, TEAPOT } shapeType = NONE;
	double drawParam[4] = { 0.f, 0.f, 0.f, 0.f };
	Vector3D color;

	const static StringHashMap<unsigned> typeMap;
	static MessageMap InitMsgMap();

private:
	static StringHashMap<unsigned> InitTypeMap();
};

struct LuaCompo : public Component
{
	virtual ~LuaCompo() {}

	std::string scriptName;
};