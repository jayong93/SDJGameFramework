#include "stdafx.h"
#include "Action.h"
#include "ActionList.h"

void ActionList::Update(double time)
{
	unsigned lane = 0;
	for (int i = 0; i < actions.size(); ++i) {
		if (actions[i]->lane & lane)
			continue;

		if (actions[i]->repeatCount == 0)
			actions[i]->OnStart();

		actions[i]->Update(time);
		actions[i]->repeatCount += 1;

		if (actions[i]->isBlocking)
			break;
		if (actions[i]->isFinished)
		{
			actions[i]->OnEnd();
			this->Delete(actions[i].get());
		}
	}
}

void ActionList::InsertBefore(Action * target, Action * action)
{
	auto it = std::find_if(actions.begin(), actions.end(), [target](const auto& a) {return a.get() == target; });
	actions.emplace(it, action);
}

void ActionList::InsertAfter(Action * target, Action * action)
{
	auto it = std::find_if(actions.begin(), actions.end(), [target](const auto& a) {return a.get() == target; });
	actions.emplace(it + 1, action);
}

void ActionList::Delete(Action * action)
{
	actions.erase(std::find_if(actions.begin(), actions.end(), [action](const auto& a) {return a.get() == action; }));
}
