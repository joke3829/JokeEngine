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