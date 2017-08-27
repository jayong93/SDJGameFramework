#pragma once

#include "Vector3D.h"
#include "Handles.h"

/*
���ӿ��� �ٷ����� �ּ� ��ü
handle - ������Ʈ �ĺ���
compoList - ������Ʈ�� ���� ������Ʈ���� ���
compoIdxMap - compoList������ ������Ʈ �ڵ��� ��ġ ����
name - ������Ʈ �̸�
position - ���忡�� ������Ʈ�� �̸�(UI ���� ���� ȭ�鿡��)
*/
struct Object
{
	ObjectHandle handle;
	std::vector<ComponentHandle> compoList;
	std::map<uint64_t, size_t> compoIdxMap;
	std::map<size_t, unsigned> compoTypeMap;
	std::string name;

	Vector3D position;

	void AddComponent(ComponentHandle handle);
	bool DelComponent(ComponentHandle handle);
	bool HasComponent(std::string type);
	void SendMsg(sol::object msg);
};