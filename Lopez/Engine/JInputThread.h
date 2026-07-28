#pragma once

#include <concurrent_queue.h>
#include "stdafx.h"


struct JKeyState {
	// 키보드인지 마우스인지
	// 무슨 키인지
	// 얼마나 누르고 있는지
	// 마우스이고 이동했으면 x, y이동량 기재
};


// 이건 Engine이 소유하고 Scene이 만들어질 때 Scene에게 &를 주고 그 Scene은 Update때 player 업데이트
class JInputThread {
public:
	JInputThread();
	~JInputThread();

	void GetInputs(std::vector<JKeyState>& outInputs);	// player가 키 입력을 받고 개별 로직으로 입력 처리

	void InputThreadStart();							// 입력 스레드 시작

	void SetInputCheckState(bool state);
	void InputQueueClear();

private:
	void InputQueueLoop();

private:
	std::atomic<bool> m_bRunning{};
	std::atomic<bool> m_bInputCheck{};
	concurrency::concurrent_queue<JKeyState> m_InputQueue;

	std::thread		m_InputLoopThread;
};