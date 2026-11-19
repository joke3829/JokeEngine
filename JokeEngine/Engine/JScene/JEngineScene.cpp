#include "JEngineScene.h"
#include "Engine/JContent/JContentManager.h"
#include "Engine/JObject/JCameraObject.h"
#include "Engine/JObject/JStaticMeshObject.h"
#include "Engine/JObject/JPlayerObject.h"
#include "Engine/JObject/JSpriteAnimationObject.h"
#include "Engine/JObject/JTextObject.h"
#include "Engine/JObject/JObjectRegistry.h"
#if DEV_MODE
#include "Engine/JObject/JEditorPlayer.h"

extern TempGlobalSceneInfo g_GlobalSceneInfo;
#endif


JEngineScene::JEngineScene()
{
	m_name = "NewScene";
}

void JEngineScene::ClearScene()
{
	// 모든 오브젝트 제거 + 행렬 모음 제거
	m_CameraMatrices.clear();
	m_CameraMatricesTP.clear();
	m_Cameras.clear();

	m_WorldMatrices.clear();
	m_WorldMatricesTP.clear();
	m_Players.clear();

	m_CurrentPlayer = -1;
	m_InitialPlayerIdx = -1;
	m_CurrentMainCamera = 0;
	m_InitialCameraIdx = 0;

	m_Objects.clear();
}

JObject* JEngineScene::CreateObject(const char* ObjectType)
{
	m_WorldMatrices.emplace_back();
	m_WorldMatricesTP.emplace_back();
	std::string name{};
	std::unique_ptr<JObject> object;
	object.reset(JObjectRegistry::CreateObject(
		ObjectType,
		m_WorldMatrices,
		m_WorldMatricesTP,
		static_cast<UINT>(m_WorldMatrices.size() - 1),
		ObjectType));


	if (!object) {
#if DEV_MODE
		spdlog::error("{} Create Failed.", ObjectType);
#endif
		return nullptr;
	}

	JObject* ret = object.get();
	m_Objects.try_emplace(object->GetUID(), std::move(object));

	// player냐?
	if (JPlayerObject* player = dynamic_cast<JPlayerObject*>(ret)) {
		if (m_CurrentPlayer < 0) { m_CurrentPlayer = 0; }
		m_Players.emplace_back(player);
	}
	else if (JCameraObject* camera = dynamic_cast<JCameraObject*>(ret)) {
#if DEV_MODE
		if (m_TempCameraIndex < 0) m_TempCameraIndex = 0;
#endif
		if (m_CurrentMainCamera < 0) { m_CurrentMainCamera = 0; }
		m_Cameras.emplace_back(camera);
	}


	// 추가 요청
	CDObjectInfo info{ ret->GetShaderName(), ret->GetUID(), ret};
	m_CDQueue.push(info);
	SetDirty();
	return ret;
}

void JEngineScene::DeleteObject(UINT uid)
{
	// 실존하는지 찾고 
	if (not m_Objects.contains(uid)) return;
	auto& obj = m_Objects[uid];

	if (JCameraObject* cp = dynamic_cast<JCameraObject*>(m_Objects[uid].get())) {
#if DEV_MODE
		for (int i = 1; i < m_Cameras.size(); ++i) {
			if (cp == m_Cameras[i]) {
				m_Cameras.erase(m_Cameras.begin() + i);
				m_CameraMatrices.erase(m_CameraMatrices.begin() + i);
				m_CameraMatricesTP.erase(m_CameraMatricesTP.begin() + i);
				if (m_CurrentMainCamera >= m_Cameras.size() - 1) {
					m_CurrentMainCamera = m_Cameras.size() - 2;
				}
				if (m_TempCameraIndex >= m_Cameras.size() - 1)
					m_TempCameraIndex = m_Cameras.size() - 2;
			}
		}
#else
		for (int i = 0; i < m_Cameras.size(); ++i) {
			if (cp == m_Cameras[i]) {
				m_Cameras.erase(m_Cameras.begin() + i);
				m_CameraMatrices.erase(m_CameraMatrices.begin() + i);
				m_CameraMatricesTP.erase(m_CameraMatricesTP.begin() + i);
				if (m_CurrentMainCamera >= m_Cameras.size()) {
					m_CurrentMainCamera = m_Cameras.size() - 1;
				}
			}
		}
#endif	
	}
	else if (JPlayerObject* pp = dynamic_cast<JPlayerObject*>(m_Objects[uid].get())) {
		for (int i = 0; i < m_Players.size(); ++i) {
			if (pp == m_Players[i]) {
				m_Players.erase(m_Players.begin() + i);
				if (m_CurrentPlayer >= m_Players.size())
					m_CurrentPlayer = m_Players.size() - 1;
			}
		}
	}

	CDObjectInfo info{};
	info.shadername = obj->GetShaderName();
	info.uid = obj->GetUID();
	m_CDQueue.push(info);
	obj->Delete();
	m_Objects.erase(info.uid);

	SetDirty();
}

void JEngineScene::Update(float elapsedTime)
{
	// 키입력에 따른 업데이트 
	std::vector<JKeyState> inputs{};
	m_InputThread->GetInputs(inputs);

#if DEV_MODE
	g_GlobalSceneInfo.camera = m_Cameras[m_CurrentMainCamera];

	if (m_PlayMode && !m_Pause) {
		if (m_CurrentPlayer > -1) {
			for (auto& state : inputs) {
				m_Players[m_CurrentPlayer]->UpdateInput(state);
			}
		}
	}
	else {
		if (m_ControlEditorScene)
			for (auto& state : inputs)
				m_EditorPlayer->UpdateInput(state);
	}
#else
	// player, ui 업데이트
	if(m_CurrentPlayer > -1)
		for (auto& state : inputs) {
			m_Players[m_CurrentPlayer]->UpdateInput(state);
		}
#endif

	for (auto& object : m_Objects)
		if(not object.second->IsHasParent())
			object.second->Update(elapsedTime);
}

void JEngineScene::Reset()
{
	m_CurrentPlayer = m_InitialPlayerIdx;

#if DEV_MODE
	m_TempCameraIndex = m_InitialCameraIdx;
#else
	m_CurrentMainCamera = m_InitialCameraIdx;
#endif

	for (auto& object : m_Objects)
		object.second->Reset();
}

void JEngineScene::WindowMessageProcessor(UINT msg, WPARAM wParam, LPARAM lParam)
{
#if DEV_MODE
	if (m_ControlEditorScene)
		m_EditorPlayer->UpdateInputWindowMessage(msg, wParam, lParam);
	else {
		if (m_PlayMode && !m_Pause) {
			if (m_CurrentPlayer > -1)
				m_Players[m_CurrentPlayer]->UpdateInputWindowMessage(msg, wParam, lParam);
		}
		// ui update
	}
#else

#endif
}

#if DEV_MODE
void JEngineScene::BuildDefaultEditorScene()
{
	ClearScene();
	auto& contents = JGlobalContentManager::GetInstance()->GetManager()->GetContentTable();

	// 1. EditorPlayer 추가
	{
		m_WorldMatrices.emplace_back();
		m_WorldMatricesTP.emplace_back();
		std::unique_ptr<JEditorPlayerObject> eplayer = std::make_unique<JEditorPlayerObject>(
			m_WorldMatrices,
			m_WorldMatricesTP,
			static_cast<UINT>(m_WorldMatrices.size() - 1),
			"EditorPlayer"
		);
		eplayer->SetRotation(45.f, -45.f, 0.f, true);
		eplayer->SetPosition(35.f, 35.f, -35.f, true);
		m_EditorPlayer = eplayer.get();

		m_Objects.try_emplace(eplayer->GetUID(), std::move(eplayer));

		// Camera 추가 예제
		m_WorldMatrices.emplace_back();
		m_WorldMatricesTP.emplace_back();
		std::unique_ptr<JCameraObject> camera = std::make_unique<JCameraObject>(
			m_WorldMatrices,
			m_WorldMatricesTP,
			static_cast<UINT>(m_WorldMatrices.size() - 1),
			"EditorCamera"
		);
		m_EditorCamera = camera.get();
		m_Cameras.emplace_back(m_EditorCamera);

		m_EditorPlayer->Attach(camera.get());
		m_Objects.try_emplace(camera->GetUID(), std::move(camera));
	}

	{	// StaticMesh 추가 예제
		if (not CreateObject("JStaticMeshObject"))
			spdlog::error("JStaticMeshObject 추가 실패");
		else {
			JStaticMeshObject* meshobject = dynamic_cast<JStaticMeshObject*>(m_Objects[m_WorldMatrices.size() - 1].get());
			meshobject->AddMaterial(contents["Resource/JKEngine/Materials/DefaultMaterial.jkmt"].get());
			meshobject->SetShaderName("DefaultShader");
			meshobject->SetStaticMesh(contents["Resource/JKEngine/Meshes/Cube.jkmh"].get());
		}
	}


	// player + camera
	{
		if(not CreateObject("JPlayerObject"))
			spdlog::error("JPlayerObject 추가 실패");
		else {
			JPlayerObject* player = dynamic_cast<JPlayerObject*>(m_Objects[m_WorldMatrices.size() - 1].get());
			player->SetPosition(0.f, 0.f, -15.f, true);

			if (not CreateObject("JCameraObject")) {
				spdlog::error("JCameraObject 추가 실패");
			}
			else {
				JCameraObject* camera = dynamic_cast<JCameraObject*>(m_Objects[m_WorldMatrices.size() - 1].get());
				player->Attach(camera);
			}
		}
	}


	SetDirty();
	spdlog::info("DefaultScene Load Complete");
}

void JEngineScene::SetPlayMode(bool pmode)
{
	m_PlayMode = pmode;
	for (auto& object : m_Objects)
		object.second->SetPlayMode(m_PlayMode);

	if (m_PlayMode) {
		m_CurrentMainCamera = m_TempCameraIndex + 1;
	}
	else {
		m_CurrentMainCamera = 0;
	}
}

void JEngineScene::EditorCameraResize(float width, float height)
{
	auto& factor = m_EditorCamera->GetProjFactor();
	if (factor.bOrtho)
		m_EditorCamera->SetProjFactorOrthographic(-width / 2, width / 2, -height / 2, height / 2, factor.Near, factor.Far);
	else
		m_EditorCamera->SetProjFactorPerspective(factor.Fov, static_cast<UINT>(width), static_cast<UINT>(height), factor.Near, factor.Far);
	auto& vp = m_EditorCamera->GetViewport();
	m_EditorCamera->SetViewport(vp.TopLeftX, vp.TopLeftY, width, height, vp.MinDepth, vp.MaxDepth);
}

// DEV_MODE일때만 사용, 반드시 Scene에 EditorCamera&Player가 있어야함
void JEngineScene::RenderSettingImGui()
{
	ImGui::SeparatorText("General");
	char cname[128]{};
	strcpy(cname, m_name.c_str());

	if (ImGui::InputText("Scene Name", cname, std::size(cname), ImGuiInputTextFlags_EnterReturnsTrue)) {
		if (IsValidString(cname)) {
			m_name = cname;
		}
	}
	// player에는 editorplayer 안들어감
	{
		const char* preview = (m_CurrentPlayer > -1) ? m_Players[m_CurrentPlayer]->GetName().c_str() : "No Player";
		if (ImGui::BeginCombo("Player Select", preview)) {
			for (int i = 0; i < m_Players.size(); ++i) {
				bool is_selected = m_CurrentPlayer == i;
				if (ImGui::Selectable((m_Players[i]->GetName() + "##" + std::to_string(m_Players[i]->GetUID())).c_str(), is_selected)) {
					m_CurrentPlayer = i;
					m_InitialPlayerIdx = i;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}
	// camera는 들어감
	{
		const char* preview = (m_TempCameraIndex > -1) ? m_Cameras[m_TempCameraIndex + 1]->GetName().c_str() : "No Camera";
		if (ImGui::BeginCombo("Camera Select", preview)) {
			for (int i = 1; i < m_Cameras.size(); ++i) {	// 0번째가 editor이다.
				bool is_selected = m_TempCameraIndex == i - 1;
				if (ImGui::Selectable((m_Cameras[i]->GetName() + "##" + std::to_string(m_Cameras[i]->GetUID())).c_str(), is_selected)) {
					m_TempCameraIndex = i - 1;
					m_InitialCameraIdx = i - 1;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}
}
#endif