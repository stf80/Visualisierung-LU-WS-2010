#pragma once

#include "common.h"
#include <windows.h>
#include <time.h>

// Timer class using the high-resolution performance counter
class Timer
{
public:

	Timer() : m_dStart(0.0), m_dCurrent(0.0), m_bRunning(false)
	{
		if (s_dSecondsPerTick <= 0.0)
		{
			LARGE_INTEGER frequency;
			
			if (QueryPerformanceFrequency(&frequency))
				s_dSecondsPerTick = 1.0 / double(frequency.QuadPart);
			else
				s_dSecondsPerTick = 1.0 / double(CLOCKS_PER_SEC);
		}

		start();
	};

	~Timer()
	{
	};

	void start()
	{
		m_bRunning = true;
		m_dStart = m_dCurrent = GetCurrentTime();
	};

	void stop()
	{
		m_dCurrent = GetCurrentTime();
		m_bRunning = false;
	};

	operator double() const
	{
		return GetCurrentTime() - GetStartTime();
	};

	operator float() const
	{
		return float(GetCurrentTime() - GetStartTime());
	};

	const bool IsRunning() const
	{
		return m_bRunning;
	};

private:

	const double GetCurrentClock() const
	{
		LARGE_INTEGER now;

		if (QueryPerformanceCounter(&now))
			return double(now.QuadPart);
		
		return double(clock());
	};

	const double GetStartTime() const
	{
		return m_dStart;
	};

	const double GetCurrentTime() const
	{
		if (m_bRunning)
			return GetCurrentClock() * s_dSecondsPerTick;
		
		return m_dCurrent;
	};

private:

	double m_dStart;
	double m_dCurrent;
	bool m_bRunning;
	static double s_dSecondsPerTick;
};
