#include "stdafx.h"



class CTimer {
public:
	CTimer();

	float Tick(float fps = 0.f);
private:
	std::chrono::steady_clock::time_point		m_StartTime{};
	std::chrono::steady_clock::time_point		m_EndTime{};
};