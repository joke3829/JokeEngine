#include "JEngineScene.h"
#include "Engine/JContent/JContentManager.h"
#include "Engine/JObject/JCameraObject.h"
#include "Engine/JObject/JStaticMeshObject.h"
#include "Engine/JObject/JPlayerObject.h"
#include "Engine/JObject/JSpriteAnimationObject.h"
#include "Engine/JObject/JTextObject.h"

#if DEV_MODE
#include "Engine/JObject/JEditorPlayer.h"
#endif


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

		m_Objects.emplace_back(eplayer);
	}

	{	// StaticMesh 추가 예제
		m_WorldMatrices.emplace_back();
		m_WorldMatricesTP.emplace_back();
		std::shared_ptr<JStaticMeshObject> meshobject = std::make_shared<JStaticMeshObject>(
			m_WorldMatrices,
			m_WorldMatricesTP,
			static_cast<UINT>(m_WorldMatrices.size() - 1),
			"MyBox"
		);
		meshobject->AddMaterial(contents["DefaultMaterial"]);
		meshobject->SetShaderName("DefaultShader");
		meshobject->SetStaticMesh(contents["DefaultCube"]);

		//meshobject->SetScale(5.f, 5.f, 5.f);
		//meshobject->SetRotation(45.f, 65.f, 45.f);

		m_Objects.emplace_back(meshobject);
	}


	{// sprite object
		m_WorldMatrices.emplace_back();
		m_WorldMatricesTP.emplace_back();
		std::shared_ptr<JSpriteAnimationObject> spriteobject = std::make_shared<JSpriteAnimationObject>(
			m_WorldMatrices,
			m_WorldMatricesTP,
			static_cast<UINT>(m_WorldMatrices.size() - 1),
			"MyTestSprite"
		);
		spriteobject->SetStaticMesh(contents["DefaultPlaneq1"]);
		spriteobject->SetSpriteSet(contents["TestSprite"]);
		spriteobject->AddMaterial(contents["DefaultSpriteMaterial"]);
		spriteobject->SetShaderName("UVTransformShader");

		spriteobject->SetScale(85.f, 85.f, 1.f, true);
		m_Objects.emplace_back(spriteobject);
	}

	// player + camera
	{
		m_WorldMatrices.emplace_back();
		m_WorldMatricesTP.emplace_back();
		std::shared_ptr<JPlayerObject> player = std::make_shared<JPlayerObject>(
			m_WorldMatrices,
			m_WorldMatricesTP,
			static_cast<UINT>(m_WorldMatrices.size() - 1),
			"Player"
		);

		player->SetPosition(0.f, 0.f, -15.f, true);
		m_Players.emplace_back(player);
		m_CurrentPlayer = 0;

		// Camera 추가 예제
		m_WorldMatrices.emplace_back();
		m_WorldMatricesTP.emplace_back();
		std::shared_ptr<JCameraObject> camera = std::make_shared<JCameraObject>(
			m_WorldMatrices,
			m_WorldMatricesTP,
			static_cast<UINT>(m_WorldMatrices.size() - 1),
			"PlayerCamera"
		);
		m_Cameras.emplace_back(camera);

		player->Attach(camera);

		m_Objects.emplace_back(player);
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