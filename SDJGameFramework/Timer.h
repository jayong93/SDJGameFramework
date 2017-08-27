#pragma once

struct Timer
{
	Timer() {}

	void Init() { prevTimePoint = std::chrono::system_clock::now(); }

	double GetElapsedTime()
	{
		currentTimePoint = std::chrono::system_clock::now();
		return std::chrono::duration<double>(currentTimePoint - prevTimePoint).count();
	}

	void UpdateTimePoint();

	double GetFPS() const
	{
		double timeSum = 0.;
		for (auto t : elapsedTimeQueue)
			timeSum += t;
		return timeSum / double(elapsedTimeQueue.size());
	}
	void Clear()
	{
		elapsedTimeQueue.clear();
		currentTimePoint = prevTimePoint = std::chrono::system_clock::time_point();
	}

private:
	std::chrono::system_clock::time_point prevTimePoint, currentTimePoint;
	const unsigned queuedTimeCount = 50;
	std::deque<double> elapsedTimeQueue;
};