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
	static void RegisterInLua();

	ObjectHandle owner;
	ComponentHandle handle;
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