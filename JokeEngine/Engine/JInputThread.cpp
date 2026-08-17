#include "JInputThread.h"

JInputThread::JInputThread()
{
	m_RefreshHz.store(1000.f, std::memory_order_release);
}

JInputThread::JInputThread(HWND hwnd)
{
	SetWindowHandle(hwnd);
	m_RefreshHz.store(1000.f, std::memory_order_release);
}

JInputThread::~JInputThread()
{
	m_bRunning.store(false, std::memory_order_release);
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
	m_bRunning.store(true, std::memory_order_release);
	m_InputLoopThread = std::thread{ &JInputThread::InputQueueLoop, this };
}

// 스레드 안전 확인이 안됨 - 되도록이면 loop 시작 전에 설정
void JInputThread::SetWindowHandle(HWND hwnd)
{
	m_hWnd = hwnd;
}

void JInputThread::CursorLocked(bool locked)
{
	m_CursorLock.store(locked, std::memory_order_release);
}

void JInputThread::SetRefreshHz(float hz)
{
	m_RefreshHz.store(hz, std::memory_order_release);
}

void JInputThread::SetInputCheckState(bool state)
{
	m_bInputCheck.store(state, std::memory_order_release);
	std::this_thread::sleep_for(std::chrono::milliseconds(5));	// 필요한가? 의도는 안전하게 5ms동안 추가 중이던 queue를 기다리고 Clear를 하려는 의도였는데
}

void JInputThread::InputQueueClear()
{
	JKeyState dummy{};
	while (m_InputQueue.try_pop(dummy));
}


// 현재 문제가 마우스를 여기서 같이 안받으면 키보드와 마우스의 동기가 안됨
void JInputThread::InputQueueLoop()
{
	POINT previousCursorPos{};
	POINT lockedCursorPos{};
	bool inputcheck = false, previousicheck = false;
	bool cursorlock = false, preCursorlock = false;
	auto start = std::chrono::steady_clock::now();



	while (m_bRunning.load(std::memory_order_acquire)) {
		inputcheck = m_bInputCheck.load(std::memory_order_acquire);
		if (inputcheck) {
			if (not previousicheck) start = std::chrono::steady_clock::now();
			
			auto end = std::chrono::steady_clock::now();
			float elapsed = std::chrono::duration<float>(end - start).count();
			float limitfps = 1.f / m_RefreshHz.load(std::memory_order_acquire);
			if (elapsed >= limitfps) {
				JKeyState state{};
				if (GetForegroundWindow() == m_hWnd) {
					for (int i = 0; i < 256; ++i) {
						// 0x0000 = 이전에 누른적 없고 현재도 안눌린 상태	(누른적 없음)
						// 0x8000 = 이전에 누른적 없고 현재 눌린 상태		(지금 눌림)
						// 0x8001 = 이전에 눌렀고 현재도 눌려있는 상태		(계속 눌림)
						// 0x0001 = 이전에 눌렀고 현재 안눌린 상태			(지금 뗌)
						state.KeyState[i] = GetAsyncKeyState(i);
					}
					GetCursorPos(&state.CursorPos);
					ScreenToClient(m_hWnd, &state.CursorPos);

					// delta
					state.deltaX = state.CursorPos.x - previousCursorPos.x;
					state.deltaY = state.CursorPos.y - previousCursorPos.y;

					previousCursorPos = state.CursorPos;

					
					cursorlock = m_CursorLock.load(std::memory_order_acquire);
					if (cursorlock) {
						if (!preCursorlock) {	// alt tab - 클라이언트 화면 사이즈 조절 시 문제 있을 수 있음(있을 것 같음 확실히)
							RECT winSize{};
							GetClientRect(m_hWnd, &winSize);
							lockedCursorPos.x = (winSize.right - winSize.left) / 2;
							lockedCursorPos.y = (winSize.bottom - winSize.top) / 2;
						}
						previousCursorPos = lockedCursorPos;
						POINT screenpos = lockedCursorPos;
						ClientToScreen(m_hWnd, &screenpos);
						SetCursorPos(screenpos.x, screenpos.y);
					}
					preCursorlock = cursorlock;

				}
					state.elapsedTime = elapsed;
//#if defined(_DEBUG) || defined(DEBUG)
//				if (state.KeyState['A'])
//					spdlog::info("Input hz - {:.3f}, {:d}, {:d}", elapsed, state.deltaX, state.deltaY);
//#endif

				m_InputQueue.push(state);

				start = std::chrono::steady_clock::now();
			}

			previousicheck = inputcheck;
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
	}
}
