#pragma once

#include "Handles.h"
#include "Vector3D.h"
#include "typedef.h"

using GetSetMap = std::unordered_map<std::string, sol::protected_function>;

/*
모든 컴포넌트들의 조상
owner - 컴포넌트를 소유하는 오브젝트의 핸들
handle - 컴포넌트의 식별자
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
	union
	{
		double param[4] = { 0.,0.,0.,0. };
		struct { double size; } cube;
		struct { double size; } teapot;
		struct { double radius, slice, stack; } sphere;
		struct { double innerRadius, outerRadius, side, ring; } torus;
		struct { double base, height, slice, stack; } cone;
	};
	Vector3D color;

	static const char* typeNames[];
	const static StringHashMap<unsigned> typeMap;
	static MessageMap InitMsgMap() { return MessageMap(); }
	static void InitGetSetFunc();

private:
	static StringHashMap<unsigned> InitTypeMap();
};

struct LuaComponent : public Component
{
	virtual ~LuaComponent();
	bool SetScript(const std::string& name);
	virtual void SendMsg(sol::object& args);
	static void InitGetSetFunc() {}
	static MessageMap InitMsgMap() { return MessageMap(); }

	sol::environment env;
	std::string scriptName;
};