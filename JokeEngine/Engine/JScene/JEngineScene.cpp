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
#endif


JEngineScene::JEngineScene()
{
#if !DEV_MODE
	if (CreateObject("JCameraObject")) {
		std::shared_ptr<JCameraObject> camera = std::dynamic_pointer_cast<JCameraObject>(m_Objects.back());
		m_Cameras.emplace_back(camera);
	}
#endif
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
	m_CurrnetMainCamera = 0;
	m_InitialCameraIdx = 0;

	m_ObjectNames.clear();
	m_Objects.clear();
}

bool JEngineScene::CreateObject(const char* ObjectType)
{
	m_WorldMatrices.emplace_back();
	m_WorldMatricesTP.emplace_back();
	std::string name{};
	std::shared_ptr<JObject> object;
	bool noloop = false;
	if (!ObjectNameDuplicateCheck(ObjectType)) {
		object = JObjectRegistry::CreateObject(
			ObjectType,
			m_WorldMatrices,
			m_WorldMatricesTP,
			static_cast<UINT>(m_WorldMatrices.size() - 1),
			ObjectType);
		noloop = true;
	}
	bool breakloop = false;
	if (not noloop) {
		for (unsigned int i = 1; i < 0xFFFFFFFF; ++i) {
			name = ObjectType + std::to_string(i);
			if (ObjectNameDuplicateCheck(name)) continue;
			object = JObjectRegistry::CreateObject(
				ObjectType,
				m_WorldMatrices,
				m_WorldMatricesTP,
				static_cast<UINT>(m_WorldMatrices.size() - 1),
				name.c_str());
			breakloop = true;
			break;
		}
	}

	if (!object) {
#if DEV_MODE
		if (breakloop)
			spdlog::error("{} Create Failed.", ObjectType);
		else
			spdlog::error("{0}(num)가 너무 많습니다. {0}(num) 일부의 이름을 변경 후 다시 시도해주세요.", ObjectType);
#endif
		return false;
	}

	m_ObjectNames.insert(object->GetName());
	m_Objects.emplace_back(object);
	return true;
}

void JEngineScene::Update(float elapsedTime)
{
	// 키입력에 따른 업데이트 
	std::vector<JKeyState> inputs{};
	m_InputThread->GetInputs(inputs);

#if DEV_MODE
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
		if(not object->IsHasParent())
			object->Update(elapsedTime);
}

void JEngineScene::Reset()
{
	for (auto& object : m_Objects)
		object->Reset();
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
	auto& contents = m_ContentManager->GetContentTable();

	// 1. EditorPlayer 추가
	{
		m_WorldMatrices.emplace_back();
		m_WorldMatricesTP.emplace_back();
		std::shared_ptr<JEditorPlayerObject> eplayer = std::make_shared<JEditorPlayerObject>(
			m_WorldMatrices,
			m_WorldMatricesTP,
			static_cast<UINT>(m_WorldMatrices.size() - 1),
			"EditorPlayer"
		);
		eplayer->SetRotation(45.f, -45.f, 0.f, true);
		eplayer->SetPosition(15.f, 15.f, -15.f, true);
		m_EditorPlayer = eplayer;

		m_Objects.emplace_back(eplayer);

		// Camera 추가 예제
		m_WorldMatrices.emplace_back();
		m_WorldMatricesTP.emplace_back();
		std::shared_ptr<JCameraObject> camera = std::make_shared<JCameraObject>(
			m_WorldMatrices,
			m_WorldMatricesTP,
			static_cast<UINT>(m_WorldMatrices.size() - 1),
			"EditorCamera"
		);
		m_EditorCamera = camera;
		m_Cameras.emplace_back(camera);

		eplayer->Attach(camera);

		m_Objects.emplace_back(camera);
	}

	{	// StaticMesh 추가 예제
		if (not CreateObject("JStaticMeshObject"))
			spdlog::error("JStaticMeshObject 추가 실패");
		else {
			std::shared_ptr<JStaticMeshObject> meshobject = std::dynamic_pointer_cast<JStaticMeshObject>(m_Objects.back());
			meshobject->AddMaterial(contents["DefaultMaterial"]);
			meshobject->SetShaderName("DefaultShader");
			meshobject->SetStaticMesh(contents["DefaultCube"]);
		}
	}


	{// sprite object
		if (not CreateObject("JSpriteAnimationObject"))
			spdlog::error("JSpriteAnimationObject 추가 실패");
		else {
			std::shared_ptr<JSpriteAnimationObject> spriteobject = std::dynamic_pointer_cast<JSpriteAnimationObject>(m_Objects.back());
			spriteobject->SetStaticMesh(contents["DefaultPlaneq1"]);
			spriteobject->SetSpriteSet(contents["TestSprite"]);
			spriteobject->AddMaterial(contents["DefaultSpriteMaterial"]);
			spriteobject->SetShaderName("UVTransformShader");

			spriteobject->SetScale(85.f, 85.f, 1.f, true);
		}
	}

	// player + camera
	{
		if(not CreateObject("JPlayerObject"))
			spdlog::error("JPlayerObject 추가 실패");
		else {
			std::shared_ptr<JPlayerObject> player = std::dynamic_pointer_cast<JPlayerObject>(m_Objects.back());
			player->SetPosition(0.f, 0.f, -15.f, true);
			m_Players.emplace_back(player);
			m_CurrentPlayer = 0;

			if (not CreateObject("JCameraObject")) {
				spdlog::error("JCameraObject 추가 실패");
			}
			else {
				std::shared_ptr<JCameraObject> camera = std::dynamic_pointer_cast<JCameraObject>(m_Objects.back());
				m_Cameras.emplace_back(camera);
				player->Attach(camera);
			}
		}
	}

	//{
	//	m_WorldMatrices.emplace_back();
	//	m_WorldMatricesTP.emplace_back();
	//	std::shared_ptr<JTextObject> text = std::make_shared<JTextObject>(
	//		m_WorldMatrices,
	//		m_WorldMatricesTP,
	//		static_cast<UINT>(m_WorldMatrices.size() - 1),
	//		"TextObject"
	//	);
	//	text->ReadyPlanes(contents);
	//	text->SetShaderName("UVTransformShader");
	//	//text->SetScale(0.05f, 0.05f, 0.05f);
	//	text->SetPosition(2.5f, -1.f, 0.f);

	//	m_Objects.emplace_back(text);
	//}


	spdlog::info("DefaultScene Load Complete");
}

void JEngineScene::SetPlayMode(bool pmode)
{
	m_PlayMode = pmode;
	for (auto& object : m_Objects)
		object->SetPlayMode(m_PlayMode);

	if (m_PlayMode) {
		m_CurrnetMainCamera += 1;
	}
	else
		m_CurrnetMainCamera = 0;
}

void JEngineScene::EditorCameraResize(float width, float height)
{
	auto& factor = m_EditorCamera->GetProjFactor();
	m_EditorCamera->SetProjFactorPerspective(factor.Fov, width / height, factor.Near, factor.Far);
	auto& vp = m_EditorCamera->GetViewport();
	m_EditorCamera->SetViewport(vp.TopLeftX, vp.TopLeftY, width, height, vp.MinDepth, vp.MaxDepth);
}
#endif