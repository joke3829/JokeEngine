#include "JStaticMeshObject.h"
#include "Engine/JokeEngineConfig.h"
#include "Engine/JContent/JMaterial.h"
#include "Engine/JContent/JStaticMesh.h"
#include "Engine/D3D11/GResource/D3D11MeshConstant.h"
#include "JObjectRegistry.h"



JStaticMeshObject::JStaticMeshObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name)
	: JObject(vWorld, vWorldTP, nodeIndex, name)
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
#if DEV_MODE
		spdlog::error("{0} - JStaticMeshObject가 잘못된 DirectX Version을 받았습니다.", m_name.c_str());
#endif
		assert(0);
		break;
	}
}

JStaticMeshObject::~JStaticMeshObject()
{

}

void JStaticMeshObject::Update(float elapsedTime, XMFLOAT4X4* parent)
{
	m_MeshCB->m_CBMesh.nodeIndex = m_NodeIndex;
	m_MeshCB->Update(elapsedTime);
	for (auto& p : m_Materials)
		p->Update(elapsedTime);

	JObject::Update(elapsedTime, parent);
}

void JStaticMeshObject::Render(UINT currentFrameIndex)
{
	if (!m_StaticMesh)
		return;
	m_MeshCB->UpdateBuffer(currentFrameIndex);
	m_MeshCB->SetGPUBuffer(currentFrameIndex, 0, JS_VS);

	auto& indices = m_StaticMesh->GetIndices();

	for (size_t i = 0; i < indices.size(); ++i) {
		m_Materials[i]->UpdateBuffer(currentFrameIndex);
		m_Materials[i]->SetGPUBuffer(currentFrameIndex, 2, JS_PS);
		m_StaticMesh->Render(i);
	}

	// 이거 애매함, Shader가 가 이 메시를 콕 집고 있을거기에 이것만 호출해야함,
	//JObject::Render();
}

void JStaticMeshObject::AddMaterial(std::shared_ptr<JContent> material, int index)
{
	std::shared_ptr<JMaterial> temp = std::dynamic_pointer_cast<JMaterial>(material);
	if (!temp) {
#if DEV_MODE
		spdlog::error("{0} StaticMeshObject에서 잘못된 AddMaterial를 호출했습니다.", m_name.c_str());
#endif
		return;
	}
	if (index >= 0 && m_Materials.size() > index) {
		m_Materials[index] = temp;
	}
	else {
		m_Materials.emplace_back(temp);
	}
}

void JStaticMeshObject::SetStaticMesh(JContent* mesh)
{
	JStaticMesh* temp = dynamic_cast<JStaticMesh*>(mesh);
	if (!temp) {
#if DEV_MODE
		spdlog::error("{0} StaticMeshObject에서 잘못된 SetStaticMesh를 호출했습니다.", m_name.c_str());
#endif
		return;
	}
	m_StaticMesh = temp;
}


#if DEV_MODE
BYTE JStaticMeshObject::RenderInspector()
{
	BYTE ret = JObject::RenderInspector();

	ImGui::SeparatorText("StaticMesh");
	ImGui::Button("MeshTestButton", ImVec2(90.f, 90.f));

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAG_CONTENT_MESH")) {
			JContent* pcon = *static_cast<JContent* const*>(payload->Data);
			JStaticMesh* pmesh = dynamic_cast<JStaticMesh*>(pcon);
			if (pmesh)
				m_StaticMesh = pmesh;
		}
		ImGui::EndDragDropTarget();
	}

	return ret;
}
#endif


REGISTER_OBJECT(JStaticMeshObject)