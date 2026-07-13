#include "JokeEngine.h"

// =======================================================================

void JokeEngine::Initialize(HWND hWnd, HINSTANCE hInstance)
{
	m_hWnd = hWnd; m_hInstance = hInstance;

	CreateSwapChain();
}

void JokeEngine::Resize(UINT width, UINT height, bool FullScreenState)
{
	// 렌더러 사이즈 변경 및 스왑체인 크기 변경
	// 12에선 앞단에 fence

	m_Renderer->ResizeTarget(width, height);
	// swapchain 사이즈 변경


}

// =======================================================================

void JokeEngineDX11::Initialize(HWND hWnd, HINSTANCE hInstance)
{
	m_GlobalFactor = JD3D11GlobalFactor::GetInstance();
	JokeEngine::Initialize(hWnd, hInstance);
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
	.BufferCount = 2,
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
	// float elapsedTime = Timer -> Tick();
	// Scene->Update(elapsedTime);
	// Renderer->AdvanceNextFrame();
	// Renderer->RenderFrame()
	
	
	ComPtr<ID3D11Texture2D> buffer;
	ThrowIfFailed(m_SwapChain->GetBuffer(0, IID_PPV_ARGS(buffer.GetAddressOf())));

	// Renderer->CopyFrame(buffer);

	if (gfactor->VerticalSYNC)
		ThrowIfFailed(m_SwapChain->Present(1, 0));
	else {
		ThrowIfFailed(m_SwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING));
	}

}


// =========================================================================

