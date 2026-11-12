#include "JStaticMeshObject.h"
#include "Engine/JokeEngineConfig.h"
#include "Engine/JContent/JMaterial.h"
#include "Engine/JContent/JStaticMesh.h"
#include "Engine/D3D11/GResource/D3D11MeshConstant.h"
#include "JObjectRegistry.h"



JStaticMeshObject::JStaticMeshObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT uid, const char* name)
	: JObject(vWorld, vWorldTP, uid, name)
{
	auto* opt = JEngineDefaultGlobalConfig::GetInstance()->GetConfigFactor();
	switch (opt->DirectX_Version) {
	case 11:
		m_MeshCB = std::make_unique<JMeshConstantDX11>();
		m_MeshCB->m_CBMesh.bSkinning = false;
		m_MeshCB->m_CBMesh.nodeIndex = uid;
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
	m_MeshCB->m_CBMesh.nodeIndex = m_UID;
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
	size_t materialsize = m_Materials.size();
	size_t mati{};

	for (size_t i = 0; i < indices.size() && materialsize > 0; ++i) {
		mati = (i >= materialsize) ? materialsize - 1 : i;
		m_Materials[mati]->UpdateBuffer(currentFrameIndex);
		m_Materials[mati]->SetGPUBuffer(currentFrameIndex, 2, JS_PS);
		m_StaticMesh->Render(i);
	}

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
bool JStaticMeshObject::RenderInspector()
{
	bool ret = JObject::RenderInspector();

	ImVec2 avail = ImGui::GetContentRegionAvail();
	ImDrawList* dl = ImGui::GetWindowDrawList();

	ImGui::SeparatorText("StaticMesh");
	ImGui::Text("Mesh");
	{	// Mesh
		ImVec2 curScreenPos = ImGui::GetCursorScreenPos();
		ImVec2 curPos = ImGui::GetCursorPos();
		ImVec2 rectmax{ curScreenPos.x + avail.x, curScreenPos.y + 62.f };
		dl->AddRectFilled(curScreenPos, rectmax, IM_COL32(45, 50, 60, 255));
		ImGui::SetCursorPos(ImVec2(curPos.x + 3.f, curPos.y + 3.f));
		void* imageref = nullptr;
		ImGui::ImageButton("MeshSelectButton", imageref, ImVec2(50.f, 50.f));
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right, false)) m_StaticMesh = nullptr;
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAG_CONTENT_BROWSER")) {
				JContent* pcon = *static_cast<JContent* const*>(payload->Data);
				JStaticMesh* pmesh = dynamic_cast<JStaticMesh*>(pcon);
				if (pmesh)
					m_StaticMesh = pmesh;
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::SetCursorPos(ImVec2(curPos.x + 65.f, curPos.y + 6.f));
		if (m_StaticMesh) {
			ImGui::TextAligned(0.0f, avail.x - 65.f, m_StaticMesh->GetName().c_str());
			dl->AddLineH(curScreenPos.x + 65, curScreenPos.x + avail.x - 3.f, curScreenPos.y + 25.f, IM_COL32(255, 255, 255, 255), 0.5f);
			ImGui::SetCursorPos(ImVec2(curPos.x + 65.f, curPos.y + 33.f));
			ImGui::Text("FilePath:");
			ImGui::SetCursorPos(ImVec2(curPos.x + 65.f, curPos.y + 47.f));
			ImGui::TextAligned(0.0f, avail.x - 65.f, m_StaticMesh->GetFullFilePath().string().c_str());
		}
		else {
			ImGui::Text("No Mesh");
		}
		ImGui::SetCursorPos(ImVec2(curPos.x, curPos.y + 70.f));
	}
	{	// Material
		ImGui::Separator();
		ImGui::Text("Materials");
	}

	return ret;
}
#endif


REGISTER_OBJECT(JStaticMeshObject)