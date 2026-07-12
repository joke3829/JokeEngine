#pragma once

#include "JokeEngineConfig.h"
#include "JContent/JContentManager.h"
#include "JRenderer/JEngineRenderer.h"
#include "D3D11/D3D11GlobalFactor.h"

// DirectX Engine

class JokeEngine {
public:
	virtual void Initialize(HWND hWnd, HINSTANCE hInstance);
	virtual void Resize(UINT width, UINT height, bool FullScreenState);
	
	virtual void Render() {}
protected:
	virtual void CreateSwapChain() {}
	virtual void Update(float elapsedTime) {}

protected:

	HWND m_hWnd{};
	HINSTANCE m_hInstance{};

	ComPtr<IDXGISwapChain4>			m_SwapChain{};

	std::shared_ptr<JContentManager>	m_ContentManager{};		

	std::shared_ptr<JEngineRenderer>	m_Renderer{};
	std::shared_ptr<JEngineScene>		m_Scene{};
};


class JokeEngineDX11 : public JokeEngine {
public:
	virtual void Initialize(HWND hWnd, HINSTANCE hInstance);
	virtual void Resize(UINT width, UINT height, bool FullScreenState);

	virtual void Render();

private:
	virtual void CreateSwapChain();
	virtual void Update(float elapsedTime);

	JD3D11GlobalFactor* m_GlobalFactor;
};


class JokeEngineDX12 final : public JokeEngine {

};