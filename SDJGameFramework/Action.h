#pragma once

class ActionList;

class Action
{
public:
	Action() :
		isFinished{ false },
		isBlocking{ false },
		timeElapsed{ 0. },
		duration{ 0. },
		lane{ 0 },
		repeatCount{ 0 }
	{}
	virtual	~Action() {}

	virtual void Update(double time) = 0;
	virtual void OnStart() {};
	virtual void OnEnd() {};

	bool isFinished;
	bool isBlocking;
	double timeElapsed;
	double duration;
	unsigned lane;
	unsigned repeatCount;

protected:
	ActionList* owner;
};