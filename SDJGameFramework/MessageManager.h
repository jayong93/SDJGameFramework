#pragma once

#include "Handles.h"
#include "typedef.h"

#define MM MessageManager::Instance()

class MessageManager
{
public:
	static MessageManager& Instance()
	{
		static MessageManager inst;
		return inst;
	}

	void BroadcastMsg(sol::object& msg);
	void RegisterComponentMessageMap(size_t type, MessageMap& msgMap)
	{
		componentsMsgMap.emplace(type, msgMap);
	}
	void RegisterInterestedComponent(size_t msgType, uint64_t handle)
	{
		interestedComponents[msgType].emplace(handle);
	}
	void Clear() { componentsMsgMap.clear(); interestedComponents.clear(); }
	const MessageMap* GetMsgMap(size_t type) const
	{
		auto it = componentsMsgMap.find(type);
		if (it != componentsMsgMap.end())
			return &it->second;
		return nullptr;
	}

private:
	MessageManager() = default;

	std::map<size_t, MessageMap> componentsMsgMap;
	std::map<size_t, std::set<uint64_t>> interestedComponents;
};