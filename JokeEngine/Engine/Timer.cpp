#include "Timer.h"

CTimer::CTimer()
{
	m_StartTime = std::chrono::steady_clock::now();
}

float CTimer::Tick(float fps)
{
	m_EndTime = std::chrono::steady_clock::now();

	float elapsedTime = std::chrono::duration<float>(m_EndTime - m_StartTime).count();

	if (fps != 0.f) {
		while (elapsedTime < 1.f / fps) {
			m_EndTime = std::chrono::steady_clock::now();
			elapsedTime = std::chrono::duration<float>(m_EndTime - m_StartTime).count();
		}
	}
	m_StartTime = m_EndTime;

	return elapsedTime;
}