#include "JEngineScene.h"
#include "Engine/JContent/JContentManager.h"
#include "Engine/JObject/JCameraObject.h"
#include "Engine/JObject/JStaticMeshObject.h"

void JEngineScene::BuildDefaultScene()
{
	auto& contents = m_ContentManager->GetContentTable();

	{	// StaticMesh 추가 예제
		m_WorldMatrices.emplace_back();
		std::shared_ptr<JStaticMeshObject> meshobject = std::make_shared<JStaticMeshObject>(
			m_WorldMatrices,
			static_cast<UINT>(m_WorldMatrices.size() - 1),
			"MyBox"
		);
		meshobject->AddMaterial(contents["DefaultMaterial"]);
		meshobject->SetShaderName("그거");
		meshobject->SetStaticMesh(contents["DefaultCube"]);
	}

	{	// Camera 추가 예제
		m_WorldMatrices.emplace_back();
		std::shared_ptr<JCameraObject> camera = std::make_shared<JCameraObject>(
			m_WorldMatrices,
			static_cast<UINT>(m_WorldMatrices.size() - 1),
			"MyCamera"
		);

		camera->SetRotation(45.f, 45.f, 0.f);
		camera->SetPosition(-30.f, 30.f, -30.f);
		m_CameraIndex.emplace_back(static_cast<UINT>(m_WorldMatrices.size() - 1));
	}


#if defined(_DEBUG) || defined(DEBUG)
	spdlog::info("DefaultScene Load Complete");
#endif
}