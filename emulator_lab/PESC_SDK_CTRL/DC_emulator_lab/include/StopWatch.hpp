#pragma once

#include <types.hpp>
#include <xtime_l.h>

class StopWatch
{
public:
	StopWatch() = default;

	inline void Start() { XTime_GetTime( &m_timeStart ); }
	inline void Stop() { XTime_GetTime( &m_timeStop ); }

	inline uint32_t Elapsed() { return static_cast<uint32_t>( m_timeStop - m_timeStart ); }
	inline uint32_t ElpasedNow() 
	{
		uint64_t now;
		XTime_GetTime(&now);
		return static_cast<uint32_t>(now - m_timeStart);
	}

private:
	uint64_t m_timeStart = 0;
	uint64_t m_timeStop = 0;
};
