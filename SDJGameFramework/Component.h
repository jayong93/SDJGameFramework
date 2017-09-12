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
	static void RegisterInLua();

	ObjectHandle owner;
	ComponentHandle handle;
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