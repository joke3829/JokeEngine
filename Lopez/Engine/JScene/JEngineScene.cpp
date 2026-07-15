#include "JEngineScene.h"
#include "Engine/JContent/JContentManager.h"
#include "Engine/JObject/JCameraObject.h"
#include "Engine/JObject/JStaticMeshObject.h"

void JEngineScene::BuildDefaultScene()
{
	{	// StaticMesh 추가 예제
		std::shared_ptr<JStaticMeshObject> meshobject = std::make_shared<JStaticMeshObject>(
			&m_WorldMatrices.emplace_back(),
			static_cast<UINT>(m_WorldMatrices.size() - 1),
			"MyBox"
		);

	}
}