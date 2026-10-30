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
	
	virtual void WindowMessageProcessor(UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void Render() {};
protected:
	virtual void CreateSwapChain() {}
	virtual void Update(float elapsedTime) {}

#if DEV_MODE
protected:
	// imgui
	virtual void ImGuiInitialize() {}
	virtual void StartImGuiRender() {}
	virtual void EndImGuiRender() {}
	virtual void ImGuiShutDown() {}


	void DockingLayoutSetup();
	void RenderImGUI();
	void RenderMenubar();			
	void RenderSceneHierarchy();
	void RenderInspector();			// scene::object가 해줄것
	void RenderProjectSettingPopup();
	void RenderCreateWindowPopup();
	void RenderToolBar();
	void RenderImGuizo();
	virtual void RenderSceneImage() {};
	
	// ImageDest ReSize
	virtual void ResizeImageDest(UINT width, UINT height) {}

	bool m_PlayMode{ false };		// false = editmode, true gamemode
	bool m_Pause{ false };
	bool m_ControlEditorScene{ false };	// 우클릭으로 Scene window를 잡고 있나를 판단
	bool m_ProjectSettingPopup{ false };
	bool m_CreateWindowPopup{ false };
	unsigned short m_SelectedObjectInCreatePopup{};

	ImVec2 m_OldEditorSceneSP{}; // GetCursorScreenPos
	ImVec2 m_OldEditorSceneAvail{};
	XMUINT2 m_NewImageDestSize{};
	char m_GizmoOperator{};
	bool m_UseSnap{};
	XMFLOAT3 m_SnapFactor{ 1.f, 5.f, 1.f };		// x: move, y: rotate, z:scale
	bool m_LocalGizmo{};
	
	JObject* m_SelectedObject = nullptr;
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
	void ResizeImageDest(UINT width, UINT height);
#endif
};


class JokeEngineDX12 final : public JokeEngine {

};