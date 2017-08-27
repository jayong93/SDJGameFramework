#pragma once

#include "Vector3D.h"
#include "Handles.h"
#include "Util.h"

/*
게임에서 다뤄지는 최소 객체
handle - 오브젝트 식별자
compoList - 오브젝트가 가진 컴포넌트들의 목록
compoIdxMap - compoList에서의 컴포넌트 핸들의 위치 저장
name - 오브젝트 이름
position - 월드에서 오브젝트의 이름(UI 같은 경우는 화면에서)
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
