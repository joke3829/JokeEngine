#include "stdafx.h"



class CTimer {
public:
	CTimer();

	float Tick(float fps = 0.f);
private:
	LARGE_INTEGER		m_Frequency{};
	LARGE_INTEGER		m_StartTime{};
	LARGE_INTEGER		m_EndTime{};
};