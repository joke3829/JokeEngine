#include "JMaterial.h"
#include "JTexture.h"
#include "JContentManager.h"

JMaterial::JMaterial(const char* name)
	: JContent(name)
{
	
}


void JMaterial::DettachTexture(unsigned char order)
{
	if (order > -1 && order < m_Textures.size())
		m_Textures[order] = nullptr;
}


JDMaterial::JDMaterial(const char* name)
	: JMaterial(name)
{
	m_Textures.assign(TextureOrder::Count, nullptr);
}


void JDMaterial::SetTexture(JContent* texture, unsigned char order)
{
	JTexture* tex = dynamic_cast<JTexture*>(texture);
	if (not tex) {
#if DEV_MODE
		spdlog::error("{0} material에서 잘못된 texture를 이용한 SetTexture를 호출했습니다.", m_name.c_str());
#endif
		return;
	}
	switch (order) {
	case BaseColor:
		m_Textures[order] = tex;
		m_SimpleMaterial.BaseColorTexIndex = 1;
		break;
	case Normal:
		m_Textures[order] = tex;
		m_SimpleMaterial.NormalTexIndex = 1;
		break;
	default:
#if DEV_MODE
		spdlog::error("{0} material에서 SetTexture에 잘못된 order를 설정했습니다.", m_name.c_str());
#endif
		break;
	}
}

void JDMaterial::DettachTexture(unsigned char order)
{
	JMaterial::DettachTexture(order);
	switch (order) {
	case TextureOrder::BaseColor:
		m_SimpleMaterial.BaseColorTexIndex = -1;
		break;
	case TextureOrder::Normal:
		m_SimpleMaterial.NormalTexIndex = -1;
		break;
	}
}



#if DEV_MODE
void JMaterial::RenderGUI(ImVec2 startPos, void* imageref)
{
	JContent::RenderGUI(startPos, JGlobalContentManager::GetInstance()->GetManager()->GetContentTable()["Resource/JKEngine/Textures/MaterialFile.dds"]->GetDedicatedFactor());
}

// =================================================================

void JDMaterial::RenderDetailPanel()
{
	JContent::RenderDetailPanel();
	ImGui::SeparatorText("Material Textures");

	float px = 120.f;
	// 색깔 상수 바꾸기 
	ImGuiColorEditFlags flags =
		ImGuiColorEditFlags_AlphaBar |
		ImGuiColorEditFlags_Float;
	ImGui::Text("BaseColor"); ImGui::SameLine(); ImGui::SetCursorPosX(px);
	ImGui::ColorEdit4("##BaseColorEditor", reinterpret_cast<float*>(&m_SimpleMaterial.BaseColor), flags);

	ImGui::Text("Metallic"); ImGui::SameLine(); ImGui::SetCursorPosX(px);
	ImGui::DragFloat("##MetaillicEditor", reinterpret_cast<float*>(&m_SimpleMaterial.MetallicRoughnessFactor.x), 0.005f, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);

	ImGui::Text("Roughness"); ImGui::SameLine(); ImGui::SetCursorPosX(px);
	ImGui::DragFloat("##RoughnessEditor", reinterpret_cast<float*>(&m_SimpleMaterial.MetallicRoughnessFactor.y), 0.005f, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);

	ImGui::Text("Emissive"); ImGui::SameLine(); ImGui::SetCursorPosX(px);
	ImGui::ColorEdit3("##EmissiveEditor", reinterpret_cast<float*>(&m_SimpleMaterial.Emissive), ImGuiColorEditFlags_Float);

	ImGui::Text("EmissiveIntensity"); ImGui::SameLine(); ImGui::SetCursorPosX(px);
	ImGui::DragFloat("##EmissiveIntensityEditor", reinterpret_cast<float*>(&m_SimpleMaterial.Emissive.w), 0.005f, 0.0f, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp);

	// 텍스쳐 지정하기

	{	
		ImGui::Separator();
		auto* dl = ImGui::GetWindowDrawList();
		ImVec2 avail = ImGui::GetContentRegionAvail();
		ImVec2 curScreenPos = ImGui::GetCursorScreenPos();
		float matsize = static_cast<float>(TextureOrder::Count);
		float rectY = 3.f + (matsize * 75.f);
		dl->AddRectFilled(curScreenPos, ImVec2(curScreenPos.x + avail.x, curScreenPos.y + rectY), IM_COL32(45, 50, 60, 255));

		std::string tags[] = { "BaseColorTexture", "NormalTexture" };
		int tagvalues[] = { m_SimpleMaterial.BaseColorTexIndex, m_SimpleMaterial.NormalTexIndex };

		for (size_t i = 0; i < matsize; ++i) {
			void* imageref = (m_Textures[i]) ? m_Textures[i]->GetDedicatedFactor() : nullptr;
			ImGui::SetCursorScreenPos(ImVec2(curScreenPos.x + 3.f, curScreenPos.y + 3.f + 2.f + (75.f * i)));
			ImGui::Text(tags[i].c_str());
			ImGui::SetCursorScreenPos(ImVec2(curScreenPos.x + 3.f, curScreenPos.y + 19.f + (75.f * i)));
			ImGui::ImageButton(std::string(std::to_string(i) + "##MaterialSettingButton").c_str(), imageref, ImVec2(50.f, 50.f));
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right, false)) DettachTexture(i);
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAG_CONTENT_BROWSER")) {
					JContent* pcon = *static_cast<JContent* const*>(payload->Data);
					SetTexture(pcon, i);
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::SetCursorScreenPos(ImVec2(curScreenPos.x + 65.f, curScreenPos.y + 22.f + (75.f * i)));
			if (m_Textures[i]) {
				ImGui::TextAligned(0.0f, avail.x - 65.f, m_Textures[i]->GetName().c_str());
				dl->AddLineH(curScreenPos.x + 65, curScreenPos.x + avail.x - 3.f, curScreenPos.y + 41.f + (75.f * i), IM_COL32(255, 255, 255, 255), 0.5f);
				ImGui::SetCursorScreenPos(ImVec2(curScreenPos.x + 65.f, curScreenPos.y + 46.f + (75.f * i)));
				ImGui::Text("FilePath:");
				ImGui::SetCursorScreenPos(ImVec2(curScreenPos.x + 65.f, curScreenPos.y + 60.f + (75.f * i)));
				ImGui::TextAligned(0.0f, avail.x - 65.f, m_Textures[i]->GetFullFilePath().string().c_str());
			}
			else {
				ImGui::Text("No Material");
			}
		}
	}

}

#endif
