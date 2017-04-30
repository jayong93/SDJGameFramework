#pragma once

#include "Handles.h"
#include "Message.h"

class MessageManager
{
public:
	static MessageManager& Instance()
	{
		static MessageManager inst;
		return inst;
	}

	void SendMsg(ComponentHandle receiver, Message& msg);
	void BroadcastMsg(Message& msg);

private:
	MessageManager() {}

	std::multimap<size_t, ComponentHandle> interestedReceivers;
};