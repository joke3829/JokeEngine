#include "JContent.h"
#include "JContentManager.h"

size_t g_numNoNameContent{};

JContent::JContent(const char* name)
	: JCommon(name)
{
	if (!name) m_name = "Content" + std::to_string(g_numNoNameContent++);
	else m_name = name;
}

#if DEV_MODE 
void JContent::RenderGUI(ImVec2 startPos, const char* dragPayload, void* imageref)
{
	// 110 by 110이니까 버튼 90 90 중상단에 하고 이름 하단 중앙 110 * 20
	ImVec2 ButtonPos = startPos;
	ButtonPos.x += 10;
	ImGui::SetCursorPos(ButtonPos);
	ImGui::ImageButton(m_name.c_str(), imageref, ImVec2(90.f, 90.f));
	if (dragPayload) {
		if (ImGui::BeginDragDropSource()) {
			JContent* self = this;
			ImGui::SetDragDropPayload(dragPayload, &self, sizeof(JContent*));
			ImGui::Text(m_name.c_str());
			ImGui::EndDragDropSource();
		}
	}
	if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered()) {
		m_ShowDetailGui = true;
	}
	if (m_ShowDetailGui) {
		if (ImGui::Begin(std::string(m_name + " DetailPanel").c_str(), &m_ShowDetailGui, ImGuiWindowFlags_AlwaysAutoResize)) {
			RenderDetailPanel();
		}
		ImGui::End();
	}
	ImVec2 TextPos = startPos; TextPos.y += 96; TextPos.x += 5.5f;
	ImGui::SetCursorPos(TextPos);
	ImGui::TextAligned(0.5f, 110, m_name.c_str());
}

void JContent::RenderGeneralInspect()
{
	float cursorx = 94.f;
	ImGui::SeparatorText("General");
	char cname[128]{};
	strcpy(cname, m_name.c_str());
	ImGui::Text("Name"); ImGui::SameLine();
	
	ImGui::SetCursorPosX(cursorx);
	if (ImGui::InputText("##InputName", cname, std::size(cname), ImGuiInputTextFlags_EnterReturnsTrue)) {
		if (IsValidString(cname)) {
			m_name = cname;
		}
	}
}
#endif