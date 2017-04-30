#pragma once

#include "Component.h"

#define CM ComponentManager::Instance()
#define RegisterComponent(list) \
	ComponentManager::Instance().RegisterComponentList((typeid(decltype(list)::CompoType)).hash_code(), &(list))
#define AddComponent(CType, OHandle) \
	ComponentManager::Instance().Add((typeid(CType)).hash_code(), OHandle)

class ICompoList
{
public:
	ICompoList() {}
	virtual ~ICompoList() {};

	virtual Component* Get(size_t i) = 0;
	virtual size_t Size() const = 0;
	virtual ComponentHandle Delete(size_t i) = 0;
	virtual Component* Add() = 0;
};

template <class T>
class CompoList : public ICompoList
{
public:
	typedef T CompoType;

	CompoList() : arr()
	{
		static_assert(std::is_base_of<Component, T>::value, "T isn't derived class of Component");
		arr.reserve(20);
	}
	virtual ~CompoList() {}

	virtual Component* Get(size_t i)
	{
		if (i >= arr.size())
			return nullptr;
		return (Component*)&arr[i];
	}
	virtual size_t Size() const { return arr.size(); }

	// 이동된 객체의 Handle 반환
	virtual ComponentHandle Delete(size_t i)
	{
		if (arr.size() == 0)
			return ComponentHandle();
		if (arr.size() == 1)
		{
			arr.pop_back();
			return ComponentHandle();
		}

		std::swap(arr[i], arr.back());
		arr.pop_back();
		return arr[i].handle;
	}

	virtual Component* Add()
	{
		arr.emplace_back();
		return &arr.back();
	}

	std::vector<CompoType> arr;
};

class ComponentManager
{
public:
	static ComponentManager& Instance()
	{
		static ComponentManager inst;
		return inst;
	}

	template <typename T>
	T* GetBy(const ComponentHandle& handle);

	Component* Get(const ComponentHandle& handle);
	void Detele(const ComponentHandle& handle);

	ComponentHandle Add(size_t type, const ObjectHandle& obj);

	void RegisterComponentList(size_t type, ICompoList* list);

private:
	ComponentManager() {}
	~ComponentManager() {}
	std::map<size_t, ICompoList*> compoLists;

	struct HandleEntry
	{
		size_t type = 0;
		ObjectHandle ownerHandle;
		unsigned count = 0;
		unsigned index = 0;
		bool isActive = false;
	};

	std::vector<HandleEntry> handleList;
	std::deque<unsigned> freeIndexQueue;
};

template<typename T>
inline T * ComponentManager::GetBy(const ComponentHandle & handle)
{
	Component* compo = this->Get(handle);

	if (compo && handleList[handle.index].type != typeid(T).hash_code())
		return nullptr;

	return (T*)compo;
}
