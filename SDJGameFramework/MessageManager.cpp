#include "stdafx.h"
#include "ComponentManager.h"
#include "MessageManager.h"

void MessageManager::SendMsg(ComponentHandle reciver, Message& msg)
{
	CM.Get(reciver)->SendMsg(msg);
}

void MessageManager::BroadcastMsg(Message & msg)
{
	auto its = interestedReceivers.equal_range(msg.type);
	for (auto it = its.first; it != its.second; ++it)
	{
		CM.Get(it->second)->SendMsg(msg);
	}
}
