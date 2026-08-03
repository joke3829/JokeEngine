#pragma once

#include <concurrent_queue.h>
#include "stdafx.h"


// 지금 눌린게 확인되면 elapsedTime동안 누르고 있었다고 판단
struct JKeyState {
	SHORT KeyState[256]{};
	POINT CursorPos{};		// 이 포즈는 client screen space pos
	int deltaX{};
	int deltaY{};
	float elapsedTime{};
};


// 이건 Engine이 소유하고 Scene이 만들어질 때 Scene에게 &를 주고 그 Scene은 Update때 player 업데이트
class JInputThread {
public:
	JInputThread();
	JInputThread(HWND hwnd);

	~JInputThread();

	void GetInputs(std::vector<JKeyState>& outInputs);	// player가 키 입력을 받고 개별 로직으로 입력 처리

	void InputThreadStart();							// 입력 스레드 시작

	void SetWindowHandle(HWND hwnd);

	//  커서 움직임을 한곳에 고정 (카메라 같은 것)
	void CursorLocked(bool locked);

	void SetRefreshHz(float hz);
	void SetInputCheckState(bool state);
	void InputQueueClear();

private:
	void InputQueueLoop();

private:
	HWND m_hWnd{};

	std::atomic<bool> m_bRunning{};
	std::atomic<bool> m_bInputCheck{};
	std::atomic<bool> m_CursorLock{};

	std::atomic<float> m_RefreshHz{};
	concurrency::concurrent_queue<JKeyState> m_InputQueue;

	std::thread		m_InputLoopThread;
};