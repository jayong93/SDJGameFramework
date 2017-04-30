#pragma once

class Action;

class ActionList
{
public:
	void Update(double time);

	void PushFront(Action* action) { actions.emplace_front(action); }
	void PushBack(Action* action) { actions.emplace_back(action); }
	void InsertBefore(Action* target, Action* action);
	void InsertAfter(Action* target, Action* action);
	void Delete(Action* action);

	Action* Begin() { return actions.front().get(); }
	Action* End() { return actions.back().get(); }

	bool IsEmpty() const { return actions.empty(); }

private:
	std::deque<std::unique_ptr<Action>> actions;
};