#include "JStaticMeshObject.h"
#include "Engine/JContent/JMaterial.h"
#include "Engine/JContent/JStaticMesh.h"


JStaticMeshObject::JStaticMeshObject(XMFLOAT4X4* WorldMatrixByScene, const char* name)
	: JObject(WorldMatrixByScene, name)
{

}

void JStaticMeshObject::Render()
{
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
