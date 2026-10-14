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

void JokeEngine::ResizeSwapChain(UINT width, UINT height, bool FullScreenState)
{
	// 렌더러 사이즈 변경 및 스왑체인 크기 변경
	// 12에선 앞단에 fence

	// swapchain 사이즈 변경

	ThrowIfFailed(m_SwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING | DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));
}

void JokeEngine::ResizeRenderer(UINT width, UINT height)
{
	m_Renderer->ResizeTarget(width, height);
}


// 단순히 Scene에게 전달만
void JokeEngine::MouseMessageReceiver(JMouseState& state)
{
	m_Scene->ProcessMouseMessage(state);
}


#if DEV_MODE  // imgui
void JokeEngine::DockingLayoutSetup()
{
	ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr)
	{

	}

	ImGui::DockSpaceOverViewport(dockspace_id, viewport, ImGuiDockNodeFlags_PassthruCentralNode);
}

void JokeEngine::RenderImGUI()
{
	RenderMenubar();
	DockingLayoutSetup();
	RenderSceneHierarchy();
	RenderInspector();
	RenderSceneImange();
}

void JokeEngine::RenderMenubar()
{

}

void JokeEngine::RenderSceneHierarchy()
{

}

void JokeEngine::RenderInspector()
{

}
#endif


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

#if DEV_MODE
	{
		auto* device = m_GlobalFactor->GetDevice();
		// imgui용 타겟 생성
		D3D11_TEXTURE2D_DESC desc{
		.Width = config->WindowsWidth,
		.Height = config->WindowsHeight,
		.MipLevels = 1,
		.ArraySize = 1,
		.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
		.SampleDesc = {.Count = 1},
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_SHADER_RESOURCE
		};
		ThrowIfFailed(device->CreateTexture2D(&desc, nullptr, m_ImageDest.ReleaseAndGetAddressOf()));

		// imgui용 srv
		ThrowIfFailed(device->CreateShaderResourceView(m_ImageDest.Get(), nullptr, m_ImageDestSRV.ReleaseAndGetAddressOf()));


		// swapchain rendertarget
		ComPtr<ID3D11Texture2D> buffer{};
		ThrowIfFailed(m_SwapChain->GetBuffer(0, IID_PPV_ARGS(buffer.ReleaseAndGetAddressOf())));
		device->CreateRenderTargetView(buffer.Get(), nullptr, m_SwapChainRTV.ReleaseAndGetAddressOf());
	}
	// imgui 초기화
	ImGuiInitialize();
#endif
}

void JokeEngineDX11::ResizeSwapChain(UINT width, UINT height, bool FullScreenState)
{
	auto* context = m_GlobalFactor->GetDeviceContext();
	context->ClearState();
#if DEV_MODE
	m_SwapChainRTV.Reset();
#endif
	JokeEngine::ResizeSwapChain(width, height, FullScreenState);

#if DEV_MODE
	auto* device = m_GlobalFactor->GetDevice();
	// swapchain rendertarget
	ComPtr<ID3D11Texture2D> buffer{};
	ThrowIfFailed(m_SwapChain->GetBuffer(0, IID_PPV_ARGS(buffer.ReleaseAndGetAddressOf())));
	device->CreateRenderTargetView(buffer.Get(), nullptr, m_SwapChainRTV.ReleaseAndGetAddressOf());
#endif
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
	.BufferCount = kNumRenderTarget,
	.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
	.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING | DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	};

	ThrowIfFailed(factory->CreateSwapChainForHwnd(device, m_hWnd, &desc, nullptr, nullptr, swapchain.GetAddressOf()));

	ThrowIfFailed(swapchain->QueryInterface(IID_PPV_ARGS(m_SwapChain.GetAddressOf())));

#if DEV_MODE
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

#if DEV_MODE
	StartImGuiRender();
	auto* context = m_GlobalFactor->GetDeviceContext();
	float clearcolor[4] = { 0.0f, 0.5f, 0.5f, 1.0f };
	context->ClearRenderTargetView(m_SwapChainRTV.Get(), clearcolor);
#endif

	m_Scene->Update(elapsedTime);       // CPU
	m_Renderer->RenderFrame();			// GPU(currnet그거) 이 안에서 FrameIndex도 넘어감
	
	
	ComPtr<ID3D11Texture2D> buffer;
	ThrowIfFailed(m_SwapChain->GetBuffer(0, IID_PPV_ARGS(buffer.GetAddressOf())));

	// DEV_MODE인 땐 ImGui::Image로 사용할 SRV에 복사
#if DEV_MODE
	m_Renderer->CopyResult(m_ImageDest.Get());
#else
	// SwapChain에 복사
	m_Renderer->CopyResult(buffer.Get());
#endif

#if DEV_MODE
	
	context->OMSetRenderTargets(1, m_SwapChainRTV.GetAddressOf(), nullptr);
	RenderImGUI();
	EndImGuiRender();

#endif


	if (gfactor->VerticalSYNC)
		ThrowIfFailed(m_SwapChain->Present(1, 0));
	else {
		ThrowIfFailed(m_SwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING));
	}

}


#if DEV_MODE
void JokeEngineDX11::ImGuiInitialize()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	ImGui_ImplWin32_Init(m_hWnd);
	ImGui_ImplDX11_Init(m_GlobalFactor->GetDevice(), m_GlobalFactor->GetDeviceContext());

	JokeEngine::ImGuiInitialize();
}

void JokeEngineDX11::StartImGuiRender()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//ImGui::ShowDemoWindow();
}

void JokeEngineDX11::EndImGuiRender()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void JokeEngineDX11::ImGuiShutDown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void JokeEngineDX11::RenderSceneImage()
{

}
#endif

// =========================================================================

