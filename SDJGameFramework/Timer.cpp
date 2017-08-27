#include "stdafx.h"
#include "Timer.h"

void Timer::UpdateTimePoint()
{
	auto elapsedTime = std::chrono::duration<double>(currentTimePoint - prevTimePoint).count();
	if (elapsedTimeQueue.size() >= queuedTimeCount)
		elapsedTimeQueue.pop_front();
	elapsedTimeQueue.emplace_back(elapsedTime);
	prevTimePoint = currentTimePoint;
}
