#pragma once

#include "Component.h"
#include "Util.h"
#include "MessageManager.h"

#define CM ComponentManager::Instance()

class ICompoList
{
public:
	ICompoList() {}
	virtual ~ICompoList() {};

	virtual Component* Get(size_t i) = 0;
	virtual size_t Size() const = 0;
	virtual ComponentHandle Delete(size_t i) = 0;
	virtual Component* Add() = 0;
	virtual void Clear() = 0;
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

	// �̵��� ��ü�� Handle ��ȯ
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

	virtual void Clear()
	{
		arr.clear();
	}

	virtual Component* Add()
	{
		arr.emplace_back();
		return &arr.back();
	}

	auto begin() const
	{
		return arr.begin();
	}

	auto end() const
	{
		return arr.end();
	}

	std::vector<CompoType> arr;
};

/*
���� ������ ������Ʈ���� �����ϴ� ��ü
compoMap - ������Ʈ�� ������ �ش� ������Ʈ �迭�� ����
handleList - �ڵ�� ���� ���� ������ �����͵��� ����
freeIndexQueue - ������Ʈ�� ���ŵǾ��� ��, �ڵ�� ����� ��Ʈ���� �ε����� ��Ȱ���ϱ� ���ؼ� ����
*/
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
	Component* Get(uint64_t handle);
	void Detele(const ComponentHandle& handle);
	void Clear();
	void ClearAndUnregister();
	size_t Type(const ComponentHandle& handle) const;

	template <typename T>
	ComponentHandle Add();

	template <typename T>
	size_t Size() const;

	template <typename T>
	void RegisterComponentList(T& list);

private:
	ComponentManager() {}
	~ComponentManager() {}
	std::map<size_t, ICompoList*> compoMap;

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

	if (compo && handleList[handle.index].type != GetTypeHash<T>())
		return nullptr;

	return (T*)compo;
}

template<typename T>
inline ComponentHandle ComponentManager::Add()
{
	size_t type = GetTypeHash<T>();
	auto it = compoMap.find(type);
	bool con = it != compoMap.end();
	assert(con && "unregistered type");

	Component* compo = it->second->Add();

	HandleEntry* entry;
	unsigned index;

	if (!freeIndexQueue.empty())
	{
		index = freeIndexQueue.front();
		freeIndexQueue.pop_front();
		entry = &handleList[index];
		++entry->count;
		if (entry->count == 0)
			entry->count = 1;
	}
	else
	{
		handleList.emplace_back();
		index = handleList.size() - 1;
		entry = &handleList.back();
		entry->count = 1;
	}

	entry->isActive = true;
	entry->index = it->second->Size() - 1;
	entry->type = type;

	ComponentHandle handle = Handle(index, entry->count);
	compo->handle = handle;

	return handle;
}

template<typename T>
inline size_t ComponentManager::Size() const
{
	size_t type = GetTypeHash<T>();
	auto it = compoMap.find(type);
	if (it != compoMap.end())
	{
		return it->second->Size();
	}
	return 0;
}

template<typename T>
inline void ComponentManager::RegisterComponentList(T& list)
{
	size_t type = GetTypeHash<T::CompoType>();
	bool con = compoMap.end() == compoMap.find(type);
	assert(con && "this type registered already");
	compoMap[type] = &list;
	MessageManager::Instance().RegisterComponentMessageMap(type, T::CompoType::InitMsgMap());
}
