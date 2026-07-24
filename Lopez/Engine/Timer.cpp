#include "Timer.h"


CTimer::CTimer()
{
	if (!QueryPerformanceFrequency(&m_Frequency)) {
		MessageBoxA(0, "This Hardware not supports 'QueryPerformance...'", "Fatal Error", MB_OK);
		PostQuitMessage(0);
		return;
	}

	QueryPerformanceCounter(&m_StartTime);
}

float CTimer::Tick(float fps)
{
	QueryPerformanceCounter(&m_EndTime);

	float elapsedTime = static_cast<float>(m_EndTime.QuadPart - m_StartTime.QuadPart) * 1.0f / m_Frequency.QuadPart;

	if (fps != 0.f) {
		while (elapsedTime < 1.f / fps) {
			QueryPerformanceCounter(&m_EndTime);
			elapsedTime = static_cast<float>(m_EndTime.QuadPart - m_StartTime.QuadPart) * 1.0f / m_Frequency.QuadPart;
		}
	}
	m_StartTime = m_EndTime;

	return elapsedTime;
}