#include "JEngineScene.h"
#include "Engine/JContent/JContentManager.h"
#include "Engine/JObject/JCameraObject.h"
#include "Engine/JObject/JStaticMeshObject.h"
#include "Engine/JObject/JPlayerObject.h"


void JEngineScene::Update(float elapsedTime)
{
	// 키입력에 따른 업데이트 
	std::vector<JKeyState> inputs{};
	m_InputThread->GetInputs(inputs);

	// player, ui 업데이트
	for (auto& state : inputs) {
		m_Players[m_CurrentPlayer]->UpdateInput(state);
	}


	for (auto& object : m_Objects)
		object->Update(elapsedTime);
}

void JEngineScene::ProcessMouseMessage(JMouseState& state)
{
	// 일반적으론 player, ui 업데이트
	static bool lc{};
	switch (state.message) {
	case WM_LBUTTONDOWN:
		lc = true;
		break;
	case WM_LBUTTONUP:
		lc = false;
		break;
	}

		spdlog::info("{:d}, {:d}", state.cursor.x, state.cursor.y);
	
}

void JEngineScene::BuildDefaultScene()
{
	auto& contents = m_ContentManager->GetContentTable();

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

		meshobject->SetRotation(45.f, 65.f, 45.f);

		m_Objects.emplace_back(meshobject);
	}

	// player + camera
	{
		m_WorldMatrices.emplace_back();
		m_WorldMatricesTP.emplace_back();
		std::shared_ptr<JPlayerObject> player = std::make_shared<JPlayerObject>(
			m_WorldMatrices,
			m_WorldMatricesTP,
			static_cast<UINT>(m_WorldMatrices.size() - 1),
			"Player0"
		);

		player->SetPosition(0.f, 0.f, -15.f);
		m_Players.emplace_back(player);

		// Camera 추가 예제
		m_WorldMatrices.emplace_back();
		m_WorldMatricesTP.emplace_back();
		std::shared_ptr<JCameraObject> camera = std::make_shared<JCameraObject>(
			m_WorldMatrices,
			m_WorldMatricesTP,
			static_cast<UINT>(m_WorldMatrices.size() - 1),
			"MyCamera"
		);
		m_Cameras.emplace_back(camera);

		player->Attach(camera);

		m_Objects.emplace_back(player);
	}

	//{	// Camera 추가 예제
	//	m_WorldMatrices.emplace_back();
	//	m_WorldMatricesTP.emplace_back();
	//	std::shared_ptr<JCameraObject> camera = std::make_shared<JCameraObject>(
	//		m_WorldMatrices,
	//		m_WorldMatricesTP,
	//		static_cast<UINT>(m_WorldMatrices.size() - 1),
	//		"MyCamera"
	//	);

	//	//camera->SetRotation(45.f, 45.f, 0.f);
	//	//camera->SetPosition(-30.f, 30.f, -30.f);
	//	//camera->SetRotation(45.f, 0.f, 0.f);
	//	camera->SetPosition(0.f, 0.f, -15.f);
	//	m_Cameras.emplace_back(camera);

	//	m_Objects.emplace_back(camera);
	//}


#if defined(_DEBUG) || defined(DEBUG)
	spdlog::info("DefaultScene Load Complete");
#endif
}