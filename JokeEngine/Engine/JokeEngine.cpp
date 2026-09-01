#include "JokeEngine.h"
#include "D3D11/Content/D3D11ContentManager.h"
#include "D3D11/Scene/D3D11Scene.h"
#include "D3D11/Renderer/D3D11Renderer.h"

// =======================================================================

void JokeEngine::Initialize(HWND hWnd, HINSTANCE hInstance)
{
	m_hWnd = hWnd; m_hInstance = hInstance;

	auto* p = JD2DTextEngine::GetInstance();

	m_InputThread = std::make_shared<JInputThread>(m_hWnd);
	m_InputThread->InputThreadStart();

	CreateSwapChain();
}

void JokeEngine::Resize(UINT width, UINT height, bool FullScreenState)
{
	// 렌더러 사이즈 변경 및 스왑체인 크기 변경
	// 12에선 앞단에 fence

	m_Renderer->ResizeTarget(width, height);
	// swapchain 사이즈 변경


}


// 단순히 Scene에게 전달만
void JokeEngine::MouseMessageReceiver(JMouseState& state)
{
	m_Scene->ProcessMouseMessage(state);
}

// =======================================================================

void JokeEngineDX11::Initialize(HWND hWnd, HINSTANCE hInstance)
{
	m_GlobalFactor = JD3D11GlobalFactor::GetInstance();
	auto* config = JEngineDefaultGlobalConfig::GetInstance()->GetConfigFactor();

	JokeEngine::Initialize(hWnd, hInstance);

	m_ContentManager = std::make_shared<JContentManagerDX11>();
	m_ContentManager->ReadyDefaultContent();
	

	m_Scene = std::make_shared<JEngineDefaultSceneDX11>();
	m_Scene->SetContentManager(m_ContentManager);
	m_Scene->SetInputThread(m_InputThread);
	m_Scene->BuildDefaultScene();

	m_Renderer = std::make_shared<JEngineRendererDX11>(config->WindowsWidth, config->WindowsHeight);
	m_Renderer->SetScene(m_Scene);

	// 키입력 시작
	m_InputThread->SetInputCheckState(true);
}


void JokeEngineDX11::CreateSwapChain()
{
	auto* factory = m_GlobalFactor->GetFactory();
	auto* device = m_GlobalFactor->GetDevice();

	auto* config = JEngineDefaultGlobalConfig::GetInstance()->GetConfigFactor();

	ComPtr<IDXGISwapChain1> swapchain{};

	DXGI_SWAP_CHAIN_DESC1 desc{
	.Width = config->WindowsWidth,
	.Height = config->WindowsHeight,
	.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
	.SampleDesc = {.Count = 1},
	.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT,
	.BufferCount = g_NumRenderTarget,
	.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
	.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING | DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	};

	ThrowIfFailed(factory->CreateSwapChainForHwnd(device, m_hWnd, &desc, nullptr, nullptr, swapchain.GetAddressOf()));

	ThrowIfFailed(swapchain->QueryInterface(IID_PPV_ARGS(m_SwapChain.GetAddressOf())));

#if defined(_DEBUG) || defined(DEBUG)
	spdlog::info("SwapChain Generate Success");
#endif

	// 전체 화면 전환
	if (config->WindowsFullscreenState) {

	}
}

void JokeEngineDX11::Update(float elapsedTime)
{

}

void JokeEngineDX11::Render()
{
	auto* gfactor = JEngineDefaultGlobalConfig::GetInstance()->GetConfigFactor();
	// update section
	// 
	float elapsedTime = m_Timer.Tick(gfactor->LimitFPS);

	m_Scene->Update(elapsedTime);       // CPU
	m_Renderer->RenderFrame();			// GPU(currnet그거) 이 안에서 FrameIndex도 넘어감
	
	
	ComPtr<ID3D11Texture2D> buffer;
	ThrowIfFailed(m_SwapChain->GetBuffer(0, IID_PPV_ARGS(buffer.GetAddressOf())));

	m_Renderer->CopyResult(buffer.Get());

	if (gfactor->VerticalSYNC)
		ThrowIfFailed(m_SwapChain->Present(1, 0));
	else {
		ThrowIfFailed(m_SwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING));
	}

}


// =========================================================================

