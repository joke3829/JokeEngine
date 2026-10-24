#include "JEngine.h"
#include "D3D11/Content/D3D11ContentManager.h"
#include "D3D11/Scene/D3D11Scene.h"
#include "D3D11/Renderer/D3D11Renderer.h"
#if DEV_MODE
#include "Engine/JObject/JCameraObject.h"
#endif

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
void JokeEngine::WindowMessageProcessor(UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_Scene->WindowMessageProcessor(msg, wParam, lParam);
}


#if DEV_MODE  // imgui
void JokeEngine::DockingLayoutSetup()
{
	ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr)
	{
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);
		ImGuiID ViewportScreen = dockspace_id;
		ImGuiID Right = 0;
		ImGui::DockBuilderSplitNode(ViewportScreen, ImGuiDir_Right, 0.25f, &Right, &ViewportScreen);
		ImGuiID RightDown = 0;
		ImGuiID RightUp = 0;
		ImGui::DockBuilderSplitNode(Right, ImGuiDir_Down, 0.7f, &RightDown, &RightUp);
		ImGuiID LeftUp = 0;
		ImGui::DockBuilderSplitNode(ViewportScreen, ImGuiDir_Up, 0.06f, &LeftUp, &ViewportScreen);
		ImGui::DockBuilderDockWindow("Scene", ViewportScreen);
		ImGui::DockBuilderDockWindow("Hierarchy", RightUp);
		ImGui::DockBuilderDockWindow("Inspector", RightDown);
		ImGui::DockBuilderDockWindow("Tools", LeftUp);
		ImGui::DockBuilderFinish(dockspace_id);
	}

	ImGui::DockSpaceOverViewport(dockspace_id, viewport, ImGuiDockNodeFlags_PassthruCentralNode);
}

void JokeEngine::RenderImGUI()
{
	RenderMenubar();
	DockingLayoutSetup();
	RenderToolBar();
	RenderSceneImage();
	RenderSceneHierarchy();
	RenderInspector();
	RenderProjectSettingPopup();
}

void JokeEngine::RenderMenubar()
{
	ImGuiIO& io = ImGui::GetIO();
	if (ImGui::BeginMainMenuBar()) {
		{
			// File Menu
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New Scene", "Ctrl+N")) { spdlog::info("누름"); }
				if (ImGui::MenuItem("Open Scene", "Ctrl+O")) { spdlog::info("누름"); }
				if (ImGui::MenuItem("Save", "Ctrl+S")) { spdlog::info("누름"); }
				if (ImGui::MenuItem("Save As", "Ctrl+Shitf+S")) { spdlog::info("누름"); }

				ImGui::Separator();

				if (ImGui::MenuItem("Project Setting", "Ctrl+O")) {
					m_ProjectSettingPopup = true;
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Quit")) {
					// 저장 묻기
					PostQuitMessage(0);
				}
				ImGui::EndMenu();
			}
		}
		{ // Debug Menu
			auto ConvertPlayMode = [&](bool mode) {
				m_PlayMode = mode;
				m_Scene->SetPlayMode(mode);
				if (!m_PlayMode) {
					m_Scene->Reset();
					m_Pause = false;
					m_Scene->SetPause(m_Pause);
					spdlog::info("PlayMode: Off");
				}
				else
					spdlog::info("PlayMode: On");
				
				};
			auto TogglePause = [&]() {
				m_Pause = !m_Pause;
				m_Scene->SetPause(m_Pause);
				if (m_Pause) spdlog::info("Pause: On");
				else spdlog::info("Pause: Off");
				};

			
			if (!m_PlayMode) {
				if (ImGui::IsKeyPressed(ImGuiKey_F5) && io.KeyCtrl) {
					ConvertPlayMode(true);
				}
			}
			else {
				if (ImGui::IsKeyPressed(ImGuiKey_P) && io.KeyCtrl && io.KeyAlt) {
					TogglePause();
				}
				if (ImGui::IsKeyPressed(ImGuiKey_F5) && io.KeyShift) {
					ConvertPlayMode(false);
				}
			}
			if (ImGui::BeginMenu("Debug")) {
				if (ImGui::MenuItem("Play", "Ctrl+F5", false, !m_PlayMode)) {
					ConvertPlayMode(true);
				}
				if (ImGui::MenuItem("Pause", "Ctrl+Alt+P", m_Pause, m_PlayMode)) {
					TogglePause();
				}
				if (ImGui::MenuItem("Stop", "Shift+F5", false, m_PlayMode)) {
					ConvertPlayMode(false);
				}
				ImGui::Separator();
				if (ImGui::MenuItem("ShowCollision", "Alt+C")) {

				}
				ImGui::EndMenu();
			}
		}
		{ // Create Menu
			if (ImGui::BeginMenu("Create")) {
				if (ImGui::MenuItem("Cube")) {

				}
				if (ImGui::MenuItem("Sphere")) {

				}
				if (ImGui::MenuItem("Plane")) {

				}
				if (ImGui::MenuItem("SpriteAnimationObject")) {

				}
				if (ImGui::MenuItem("Text")) {

				}
				ImGui::Separator();
				if (ImGui::MenuItem("ObjectWindow")) {

				}
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();
	}
}

void JokeEngine::RenderSceneHierarchy()
{
	auto& objects = m_Scene->GetObjects();

	ImGuiTreeNodeFlags flags =
		ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_DrawLinesFull |
		ImGuiTreeNodeFlags_DefaultOpen |
		ImGuiTreeNodeFlags_SpanFullWidth;

	auto DrawHierarchy = [&](auto& self, std::shared_ptr<JObject>& object) -> void{
		ImGuiTreeNodeFlags node_flag = flags;
		if (object.get() == m_SelectedObject)
			node_flag |= ImGuiTreeNodeFlags_Selected;
		if (object->GetLeafObjects().size() == 0) {
			node_flag |= ImGuiTreeNodeFlags_Leaf;
			if (ImGui::TreeNodeEx(object->GetName().c_str(), node_flag))
				if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
					m_SelectedObject = object.get();
				}
				ImGui::TreePop();
			return;
		}
		else {
			if (ImGui::TreeNodeEx(object->GetName().c_str(), node_flag)) {
				if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
					m_SelectedObject = object.get();
				}
				for (auto& child : object->GetLeafObjects())
					self(self, child);
				ImGui::TreePop();
			}
		}
		};
	if (ImGui::Begin("Hierarchy")) {
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) && ImGui::IsMouseClicked(0)) 
			m_SelectedObject = nullptr;
		if (ImGui::TreeNodeEx(m_Scene->GetName().c_str(), flags)) {
			for (auto& object : objects) {
				DrawHierarchy(DrawHierarchy, object);
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void JokeEngine::RenderInspector()
{
	if (ImGui::Begin("Inspector")) {
		if (m_SelectedObject) {
			m_SelectedObject->RenderInspector();
		}
		else
			ImGui::Text("None");
	}
	ImGui::End();
}

void JokeEngine::RenderProjectSettingPopup()
{
	if (m_ProjectSettingPopup) {
		ImGui::OpenPopup("Project Setting");
		m_ProjectSettingPopup = false;
	}
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Project Setting", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		if (ImGui::Button("Test", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}
}

void JokeEngine::RenderToolBar()
{
	if (!m_PlayMode && !m_ControlEditorScene) {
		if (ImGui::IsKeyPressed(ImGuiKey_Q, false)) { m_GizmoOperator = 0; }
		if (ImGui::IsKeyPressed(ImGuiKey_W, false)) { m_GizmoOperator = 1; }
		if (ImGui::IsKeyPressed(ImGuiKey_E, false)) { m_GizmoOperator = 2; }
		if (ImGui::IsKeyPressed(ImGuiKey_R, false)) { m_GizmoOperator = 3; }
	}
	
	if (m_PlayMode) ImGui::BeginDisabled(true);
	if (ImGui::Begin("Tools")) {
		// gizmo
		if (ImGui::RadioButton("Select", m_GizmoOperator == 0)) { m_GizmoOperator = 0; } ImGui::SameLine();
		if (ImGui::RadioButton("Move", m_GizmoOperator == 1)) { m_GizmoOperator = 1; } ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", m_GizmoOperator == 2)) { m_GizmoOperator = 2; } ImGui::SameLine();
		if (ImGui::RadioButton("Scale", m_GizmoOperator == 3)) { m_GizmoOperator = 3; } ImGui::SameLine();

		// snap
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50.f);
		if (ImGui::Checkbox("Use Snap", &m_UseSnap)); ImGui::SameLine();
		ImGui::SetNextItemWidth(70.f);
		ImGui::DragFloat("##MoveSnapDragFloat", &m_SnapFactor.x, 0.05f, 0.f, 100.f, "%.2f", ImGuiSliderFlags_AlwaysClamp); ImGui::SetItemTooltip("MoveFactor"); ImGui::SameLine();
		ImGui::SetNextItemWidth(70.f);
		ImGui::DragFloat("##RotateSnapDragFloat", &m_SnapFactor.y, 0.1f, 0.f, 90.f, "%.2f", ImGuiSliderFlags_AlwaysClamp); ImGui::SetItemTooltip("RotateFactor"); ImGui::SameLine();
		ImGui::SetNextItemWidth(70.f);
		ImGui::DragFloat("##ScaleSnapDragFloat", &m_SnapFactor.z, 0.05f, 0.f, 50.f, "%.2f", ImGuiSliderFlags_AlwaysClamp); ImGui::SetItemTooltip("ScaleFactor");
	}
	ImGui::End();
	if (m_PlayMode) ImGui::EndDisabled();
}

void JokeEngine::RenderImGuizo()
{
	if (m_SelectedObject) {
		auto* cam = m_Scene->GetEditorCamera();
		if (m_PlayMode)
			cam = m_Scene->GetCurrentMainCamera();
		ImGuizmo::SetOrthographic(cam->GetProjFactor().bOrtho);

		ImGuizmo::SetDrawlist();
		ImVec2 winpos = ImGui::GetWindowPos();
		ImVec2 winsize = ImGui::GetWindowSize();
		ImGuizmo::SetRect(winpos.x, winpos.y, winsize.x, winsize.y);
		XMFLOAT4X4 view = cam->GetViewMatrix();
		XMFLOAT4X4 proj = cam->GetProjMatrix();
		XMFLOAT4X4 identity;
		XMStoreFloat4x4(&identity, XMMatrixIdentity());
		XMFLOAT4X4 oMatrix = m_SelectedObject->GetWorldTransform();
		
		//ImGuizmo::DrawGrid(reinterpret_cast<float*>(&view), reinterpret_cast<float*>(&proj), reinterpret_cast<float*>(&identity), 100.f);
		if (m_GizmoOperator == 0 || m_PlayMode) {
			XMVECTOR S, R, T;
			XMMatrixDecompose(&S, &R, &T, XMLoadFloat4x4(&oMatrix));
			S = XMVectorSet(1.f, 1.f, 1.f, 1.f);
			XMStoreFloat4x4(&oMatrix, XMMatrixAffineTransformation(S, g_XMZero, R, T));
			ImGuizmo::DrawAxes(reinterpret_cast<float*>(&view), reinterpret_cast<float*>(&proj), reinterpret_cast<float*>(&oMatrix), 1);
		}
		else {
			ImGuizmo::OPERATION oper{};
			float sf[3]{};
			switch (m_GizmoOperator) {
			case 1:
				oper = ImGuizmo::OPERATION::TRANSLATE;
				sf[0] = sf[1] = sf[2] = m_SnapFactor.x;
				break;
			case 2:
				oper = ImGuizmo::OPERATION::ROTATE;
				sf[0] = sf[1] = sf[2] = m_SnapFactor.y;
				break;
			case 3:
				oper = ImGuizmo::OPERATION::SCALE;
				sf[0] = sf[1] = sf[2] = m_SnapFactor.z;
				break;
			}
			ImGuizmo::Manipulate(reinterpret_cast<float*>(&view), reinterpret_cast<float*>(&proj), oper, ImGuizmo::MODE::WORLD, reinterpret_cast<float*>(&oMatrix),
				nullptr, m_UseSnap ? sf : nullptr, nullptr, nullptr);
			if (ImGuizmo::IsUsing()) {
				XMMATRIX parentinverse = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_SelectedObject->GetParentWorldTransform()));
				XMMATRIX result = XMLoadFloat4x4(&oMatrix) * parentinverse;
				XMVECTOR S, R, T;
				XMMatrixDecompose(&S, &R, &T, result);

				XMFLOAT3 pos, scale;
				XMFLOAT4 quat;
				XMStoreFloat3(&pos, T); XMStoreFloat3(&scale, S); XMStoreFloat4(&quat, R);
				m_SelectedObject->SetPosition(pos, true);
				m_SelectedObject->SetRotation(quat, true);
				m_SelectedObject->SetScale(scale, true);
			}
		}
	}
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
	m_Scene->SetName("NewScene");
	m_Scene->SetContentManager(m_ContentManager);
	m_Scene->SetInputThread(m_InputThread);
#if DEV_MODE
	m_Scene->BuildDefaultEditorScene();
	m_Scene->SetPlayMode(m_PlayMode);
#else
	//m_Scene->BuildSceneFromFile(scene file path); or m_Scene->BuildScene(); // virtual로 상속받아 쓰는것도 되고
#endif
	m_Renderer = std::make_shared<JEngineRendererDX11>(config->WindowsWidth, config->WindowsHeight);
	m_Renderer->SetScene(m_Scene);

	// 키입력 시작
	m_InputThread->SetInputCheckState(true);

#if DEV_MODE
	{
		auto* device = m_GlobalFactor->GetDevice();
		
		// ImageDest 생성
		ResizeImageDest(config->WindowsWidth, config->WindowsHeight);
		m_NewImageDestSize.x = config->WindowsWidth; m_NewImageDestSize.y = config->WindowsHeight;

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
#if DEV_MODE
	.Width = 1920,
	.Height = 1080,
#else
	.Width = config->WindowsWidth,
	.Height = config->WindowsHeight,
#endif
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
	float clearcolor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	context->ClearRenderTargetView(m_SwapChainRTV.Get(), clearcolor);

	// 렌더링 전 렌더타겟 크기 바꾸기
	XMUINT2 renderer_size = m_Renderer->GetTargetSize();
	if (m_PlayMode) {
		if(renderer_size.x != gfactor->WindowsWidth || renderer_size.y != gfactor->WindowsHeight)
		m_Renderer->ResizeTarget(gfactor->WindowsWidth, gfactor->WindowsHeight);
		ResizeImageDest(gfactor->WindowsWidth, gfactor->WindowsHeight);
	}
	else {
		if (renderer_size.x != m_NewImageDestSize.x || renderer_size.y != m_NewImageDestSize.y) {
			m_Renderer->ResizeTarget(m_NewImageDestSize.x, m_NewImageDestSize.y);
			m_Scene->EditorCameraResize(static_cast<float>(m_NewImageDestSize.x), static_cast<float>(m_NewImageDestSize.y));
			ResizeImageDest(m_NewImageDestSize.x, m_NewImageDestSize.y);
		}
		// Scene 창 클릭 관련
		auto& io = ImGui::GetIO();
		if (m_ControlEditorScene) {
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
				m_ControlEditorScene = false;
				m_Scene->SetControlEditorScene(m_ControlEditorScene);
			}
		}
		else {
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
				float mx = io.MousePos.x; float my = io.MousePos.y;
				bool isInside = mx > m_OldEditorSceneSP.x && mx < (m_OldEditorSceneSP.x + m_OldEditorSceneAvail.x)
					&& my > m_OldEditorSceneSP.y && my < (m_OldEditorSceneSP.y + m_OldEditorSceneAvail.y);
				if (isInside) {
					m_ControlEditorScene = true;
					m_Scene->SetControlEditorScene(m_ControlEditorScene);
				}
			}
		}
	}

	if (!m_PlayMode || m_Pause) elapsedTime = 0.f;
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
}

void JokeEngineDX11::StartImGuiRender()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	// ImGuizmo
	ImGuizmo::BeginFrame();
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
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	if (ImGui::Begin("Scene", nullptr)) {
		// Imgui::Image 할 때 넘겨주는 사이즈를 맞춰줘야한다.
		ImVec2 ImageSize = ImGui::GetContentRegionAvail();
		ImVec2 avail = ImageSize;
		m_OldEditorSceneAvail = avail;
		m_NewImageDestSize.x = static_cast<UINT>(ImageSize.x);
		m_NewImageDestSize.y = static_cast<UINT>(ImageSize.y);
		ImVec2 InitCursorPos = ImGui::GetCursorPos();
		m_OldEditorSceneSP = ImGui::GetCursorScreenPos();

		if (m_PlayMode) {
			XMUINT2 cnSize = m_Renderer->GetTargetSize();
			// avil size랑 현재 배율 보고
			XMFLOAT2 fsize = { static_cast<float>(cnSize.x), static_cast<float>(cnSize.y) };

			float scaleX = avail.x / fsize.x;
			float scaleY = avail.y / fsize.y;
			float targetscale = (scaleX < scaleY) ? scaleX : scaleY;

			ImageSize.x = fsize.x * targetscale; ImageSize.y = fsize.y * targetscale;
			// 중앙 정렬을 위한 오프셋 계산 (contentStartPos 기준)

			float offsetX = (avail.x - ImageSize.x) * 0.5f;
			float offsetY = (avail.y - ImageSize.y) * 0.5f;

			ImGui::SetCursorPos(ImVec2(InitCursorPos.x + offsetX, InitCursorPos.y + offsetY));
		}
		ImGui::Image(m_ImageDestSRV.Get(), ImageSize);

		JokeEngine::RenderImGuizo();

		ImGui::SetCursorPos(InitCursorPos);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.f, 1.f, 0.f, 1.f));
		auto& io = ImGui::GetIO();
		ImGui::Text("%.2fFPS", io.Framerate);
		ImGui::Text("%.1f, %.1f", io.MousePos.x, io.MousePos.y);
		ImGui::Text("GetCursorPos: %.1f, %.1f", InitCursorPos.x, InitCursorPos.y);
		ImGui::Text("GetCursorScreenPos: %.1f, %.1f", m_OldEditorSceneSP.x, m_OldEditorSceneSP.y);
		ImGui::Text("%.1f, %.1f", avail.x, avail.y);
		ImGui::PopStyleColor();
	}
	ImGui::PopStyleVar();
	ImGui::End();
}

void JokeEngineDX11::ResizeImageDest(UINT width, UINT height)
{
	auto* device = m_GlobalFactor->GetDevice();
	// imgui용 타겟 생성
	D3D11_TEXTURE2D_DESC desc{
	.Width = width,
	.Height = height,
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
}
#endif

// =========================================================================

