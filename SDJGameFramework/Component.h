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

/*
컴포넌트 공통
InitMsgMap() - 처리할 메세지와 핸들러를 묶어놓은 MessageMap을 초기화
InitGetSetFunc() - 루아에서 컴포넌트의 변수로 접근할 이름과 핸들러를 묶음
*/

/*
GLUT에서 제공하는 기본 3차원 도형들을 나타내는 컴포넌트
변수		-	설명		-	루아에서의 변수 이름
shapeType - 도형 종류 - "type"
param - GLUT 그리기 함수의 인자들 - "typeParam" (ex. cubeSize, torusInnerRadius)
color - 도형의 색 - "color"
typeNames, typeMap - 타입 enum과 string의 변환 데이터
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
루아로 작성된 스크립트 컴포넌트
env - 스크립트에서 접근할 환경 테이블. 전역 환경을 더럽히는걸 막는다.
scriptName - 실제 컴포넌트 종류
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