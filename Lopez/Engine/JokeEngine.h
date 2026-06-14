#pragma once

#include "JokeEngineConfig.h"
#include "D3D11/D3D11GlobalFactor.h"

// DirectX Engine

class JokeEngine {
public:
	virtual void Initialize(HWND hWnd, HINSTANCE hInstance) {}
	virtual void Resize(UINT width, UINT height) {}
	
	virtual void Tick() {}
protected:
	virtual void CreateSwapChain() {}

	ComPtr<IDXGISwapChain4>			m_SwapChain{};
};


class JokeEngineDX11 final : public JokeEngine {
public:
	void Initialize(HWND hWnd, HINSTANCE hInstance);

private:
	void CreateSwapChain();

	JD3D11GlobalFactor* globalFactor;
};

class JokeEngineDX12 final : public JokeEngine {

};