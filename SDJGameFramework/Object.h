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
	ObjectHandle handle, parent;
	std::vector<ComponentHandle> compoList;
	std::vector<uint64_t> childList;
	std::map<uint64_t, size_t> compoIdxMap;
	std::map<size_t, ComponentHandle> compoTypeMap;
	std::string name;

	Vector3D position;

	void Move(const Vector3D& offset) { position += offset; }
	void Move(float x, float y, float z) { Move({ x,y,z }); }

	bool AddComponent(ComponentHandle handle);
	bool DelComponent(ComponentHandle handle);

	template <typename T>
	bool HasComponent() const;
	bool HasComponent(const std::string& type) const { return HasComponent(GetHash(type)); }
	bool HasComponent(size_t type) const;

	template<typename T>
	ComponentHandle GetComponent();
	ComponentHandle GetComponent(const std::string& type);
	ComponentHandle GetComponent(size_t type);

	void SendMsg(sol::object msg);
	static void RegisterInLua();
};

template<typename T>
inline bool Object::HasComponent() const
{
	return HasComponent(GetTypeHash<T>());
}

template<typename T>
inline ComponentHandle Object::GetComponent()
{
	return GetComponent(GetTypeHash<T>());
}
