#pragma once

#include "Vector3D.h"
#include "Handles.h"
#include "Util.h"

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
	std::set<size_t> compoTypeSet;
	std::string name;

	Vector3D position;

	bool AddComponent(ComponentHandle handle);
	bool DelComponent(ComponentHandle handle);
	bool HasComponent(const std::string& type) const { return HasComponent(GetHash(type)); }
	bool HasComponent(size_t type) const;
	template <typename T>
	bool HasComponent() const;

	void SendMsg(sol::object msg);
};

template<typename T>
inline bool Object::HasComponent() const
{
	HasComponent(GetTypeHash<T>());
}
