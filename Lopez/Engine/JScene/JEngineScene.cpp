#include "JEngineScene.h"
#include "Engine/JContent/JContentManager.h"
#include "Engine/JObject/JCameraObject.h"
#include "Engine/JObject/JStaticMeshObject.h"

void JEngineScene::Update(float elapsedTime)
{
	for (auto& object : m_Objects)
		object->Update(elapsedTime);
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

		meshobject->SetRotation(45.f, 45.f, 45.f);

		m_Objects.emplace_back(meshobject);
	}

	{	// Camera 추가 예제
		m_WorldMatrices.emplace_back();
		m_WorldMatricesTP.emplace_back();
		std::shared_ptr<JCameraObject> camera = std::make_shared<JCameraObject>(
			m_WorldMatrices,
			m_WorldMatricesTP,
			static_cast<UINT>(m_WorldMatrices.size() - 1),
			"MyCamera"
		);

		//camera->SetRotation(45.f, 45.f, 0.f);
		//camera->SetPosition(-30.f, 30.f, -30.f);
		//camera->SetRotation(45.f, 0.f, 0.f);
		camera->SetPosition(0.f, 0.f, -30.f);
		m_Cameras.emplace_back(camera);

		m_Objects.emplace_back(camera);
	}


#if defined(_DEBUG) || defined(DEBUG)
	spdlog::info("DefaultScene Load Complete");
#endif
}