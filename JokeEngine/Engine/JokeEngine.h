#pragma once

#include "JokeEngineConfig.h"
#include "JContent/JContentManager.h"
#include "JRenderer/JEngineRenderer.h"
#include "D3D11/D3D11GlobalFactor.h"
#include "D2D/JD2DTextEngine.h"
#include "JInputThread.h"
#include "Timer.h"

// DirectX Engine

class JokeEngine {
public:
	virtual void Initialize(HWND hWnd, HINSTANCE hInstance);
	virtual void ResizeSwapChain(UINT width, UINT height, bool FullScreenState);
	virtual void ResizeRenderer(UINT width, UINT height);
	
	virtual void MouseMessageReceiver(JMouseState& state);

	virtual void Render() {};
protected:
	virtual void CreateSwapChain() {}
	virtual void Update(float elapsedTime) {}

#if DEV_MODE
protected:
	// imgui
	virtual void ImGuiInitialize() { DockingLayoutSetup(); }
	virtual void StartImGuiRender() {}
	virtual void EndImGuiRender() {}
	virtual void ImGuiShutDown() {}


	void DockingLayoutSetup();
	void RenderImGUI();
	void RenderMenubar();			
	void RenderSceneHierarchy();	// scene이 해줄것
	void RenderInspector();			// scene::object가 해줄것
	virtual void RenderSceneImange() {};
#endif
protected:

	HWND m_hWnd{};
	HINSTANCE m_hInstance{};

	ComPtr<IDXGISwapChain4>			m_SwapChain{};

	std::shared_ptr<JContentManager>	m_ContentManager{};		

	std::shared_ptr<JEngineRenderer>	m_Renderer{};
	std::shared_ptr<JEngineScene>		m_Scene{};
	
	std::shared_ptr<JInputThread>		m_InputThread{};

	CTimer								m_Timer{};
};


class JokeEngineDX11 : public JokeEngine {
public:
	virtual void Initialize(HWND hWnd, HINSTANCE hInstance);
	void ResizeSwapChain(UINT width, UINT height, bool FullScreenState);

	virtual void Render();

private:
	virtual void CreateSwapChain();
	virtual void Update(float elapsedTime);

	JD3D11GlobalFactor* m_GlobalFactor;

#if DEV_MODE
private:
	void ImGuiInitialize();
	void StartImGuiRender();
	void EndImGuiRender();
	void ImGuiShutDown();
	void RenderSceneImage();

	// imgui Image용 resource와 srv
	ComPtr<ID3D11Texture2D> m_ImageDest{};
	ComPtr<ID3D11ShaderResourceView> m_ImageDestSRV{};
	ComPtr<ID3D11RenderTargetView> m_SwapChainRTV{};
#endif
};


class JokeEngineDX12 final : public JokeEngine {

};