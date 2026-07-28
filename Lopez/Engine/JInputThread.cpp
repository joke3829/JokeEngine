#include "JInputThread.h"

JInputThread::JInputThread()
{

}

JInputThread::~JInputThread()
{
	m_bRunning = false;
	if (m_InputLoopThread.joinable())
		m_InputLoopThread.join();
}

void JInputThread::GetInputs(std::vector<JKeyState>& outInputs)
{
	outInputs.clear();

	JKeyState state{};
	while (m_InputQueue.try_pop(state)) {
		outInputs.emplace_back(state);
	}
}

void JInputThread::InputThreadStart()
{
	m_bRunning = true;
	m_InputLoopThread = std::thread{ JInputThread::InputQueueLoop, this };
}

void JInputThread::SetInputCheckState(bool state)
{
	m_bInputCheck = state;
}

void JInputThread::InputQueueClear()
{

	// 동시성 안전 X 반드시 m_bInputCheck를 끄고 Scene이 시작 전에 클리어 하자
	m_InputQueue.clear();
}


void JInputThread::InputQueueLoop()
{

}
