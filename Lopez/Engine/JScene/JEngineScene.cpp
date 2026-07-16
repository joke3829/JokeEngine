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
		meshobject->SetShaderName("그거");
		meshobject->SetStaticMesh(contents["DefaultCube"]);
	}


#if defined(_DEBUG) || defined(DEBUG)
	spdlog::info("DefaultScene Load Complete");
#endif
}