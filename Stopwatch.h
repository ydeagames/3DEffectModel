#pragma once
#include "StepTimer.h"

class Stopwatch
{
private:
	DX::StepTimer* m_timer;
	float lastTime;
	float time;

public:
	Stopwatch(DX::StepTimer& timer)
		: m_timer(&timer)
		, lastTime(m_timer->GetTotalSeconds())
		, time(m_timer->GetTotalSeconds())
	{
	}

public:
	void start()
	{
		lastTime = m_timer->GetTotalSeconds();
	}

	void stop()
	{
		time = m_timer->GetTotalSeconds();
	}

	operator float()
	{
		return time - lastTime;
	}
};