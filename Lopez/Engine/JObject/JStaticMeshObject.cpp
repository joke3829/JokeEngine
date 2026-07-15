#include "JStaticMeshObject.h"
#include "Engine/JokeEngineConfig.h"
#include "Engine/JContent/JMaterial.h"
#include "Engine/JContent/JStaticMesh.h"
#include "Engine/D3D11/D3D11MeshConstant.h"



JStaticMeshObject::JStaticMeshObject(XMFLOAT4X4* WorldMatrixByScene, UINT nodeIndex, const char* name)
	: JObject(WorldMatrixByScene, name)
{
	auto* opt = JEngineDefaultGlobalConfig::GetInstance()->GetConfigFactor();
	switch (opt->DirectX_Version) {
	case 11:
		m_MeshCB = std::make_unique<JMeshConstantDX11>();
		m_MeshCB->m_CBMesh.bSkinning = false;
		m_MeshCB->m_CBMesh.nodeIndex = nodeIndex;
		break;
	case 12:
		break;
	default:
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("{0} - JStaticMeshObject가 잘못된 DirectX Version을 받았습니다.", m_name.c_str());
#endif
		assert(0);
		break;
	}
}

void JStaticMeshObject::Render()
{
	m_MeshCB->Update();
	m_MeshCB->SetDXBuffer(0, JS_VS);

	auto& indices = m_StaticMesh->GetIndices();

	for (size_t i = 0; i < indices.size(); ++i) {
		m_Materials[i]->SetDXBuffer(0, JS_PS);
		m_StaticMesh->Render(i);
	}
}

void JStaticMeshObject::AddMaterial(std::shared_ptr<JMaterial> material, int index)
{
	if (index >= 0 && m_Materials.size() > index) {
		m_Materials[index] = material;
	}
	else {
		m_Materials.emplace_back(material);
	}
}
