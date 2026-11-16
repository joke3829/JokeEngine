#include "JContent.h"
#include "JContentManager.h"

size_t g_numNoNameContent{};

JContent::JContent(const char* name)
{
	if (!name)
		m_FullFilePath = "Resource/Content" + std::to_string(g_numNoNameContent++);
	else
		m_FullFilePath = name;

	m_name = m_FullFilePath.filename().string();
}

JContent::JContent(std::filesystem::path filepath)
{
	m_FullFilePath = filepath;
	m_name = m_FullFilePath.filename().string();
}

void JContent::Initialize()
{
	if (std::filesystem::exists(m_FullFilePath))
		LoadFromFile();
	else
		CreateContentFile();
}

#if DEV_MODE 
void JContent::RenderGUI(ImVec2 startPos, void* imageref)
{
	// 110 by 110이니까 버튼 90 90 중상단에 하고 이름 하단 중앙 110 * 20
	ImVec2 ButtonPos = startPos;
	ButtonPos.x += 10;
	ImGui::SetCursorPos(ButtonPos);
	ImGui::ImageButton(m_name.c_str(), imageref, ImVec2(90.f, 90.f));
	if (ImGui::BeginDragDropSource()) {
		JContent* self = this;
		ImGui::SetDragDropPayload("DRAG_CONTENT_BROWSER", &self, sizeof(JContent*));
		ImGui::Text(m_name.c_str());
		ImGui::EndDragDropSource();
	}
	if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered()) {
		ReadyTempData();
		m_ShowDetailGui = true;
	}
	ImVec2 TextPos = startPos; TextPos.y += 96; TextPos.x += 5.5f;
	ImGui::SetCursorPos(TextPos);
	ImGui::TextAligned(0.5f, 110, m_name.c_str());
}

void JContent::RenderGeneralInspect()
{
	float cursorx = 120.f;
	ImGui::SeparatorText("General");
	char cname[128]{};
	strcpy(cname, m_ModifyName.c_str());
	ImGui::Text("Name"); ImGui::SameLine();
	
	ImGui::SetCursorPosX(cursorx);
	if (ImGui::InputText("##InputName", cname, std::size(cname), ImGuiInputTextFlags_EnterReturnsTrue)) {
		if (IsValidString(cname)) {
			std::filesystem::path tpath(cname);
			if (tpath.extension() != m_FullFilePath.extension()) {
				m_ModifyName = cname + m_FullFilePath.extension().string();
			}
			else
				m_ModifyName = cname;
		}
	}
	ImGui::Text("File Path: %s", m_FullFilePath.string().c_str());
}

bool JContent::CheckRenderDetailPanelAndRender()
{
	bool ret{};
	if (m_ShowDetailGui) {
		ImGui::SetNextWindowSize(ImVec2(500.f, 640.f), ImGuiCond_FirstUseEver);
		if (ImGui::Begin(std::string(m_name + " DetailPanel###" + std::to_string(reinterpret_cast<uintptr_t>(this))).c_str(), &m_ShowDetailGui)) {
			ImVec2 curScreenPos = ImGui::GetCursorScreenPos();
			ImVec2 avail = ImGui::GetContentRegionAvail();
			RenderDetailPanel();

			ImGui::SetCursorScreenPos(ImVec2(curScreenPos.x + avail.x - 150.f, curScreenPos.y + avail.y - 20.f));
			if (ImGui::Button("Save##TheEgo", ImVec2(70.f, 20.f))) {
				// 임시를 실제로 바꾸고
				ApplyTempDataToRealData();
				// 파일 재생성(최신화)
				CreateContentFile();
				ret = true;
				m_ShowDetailGui = false;
			}
			ImGui::SetCursorScreenPos(ImVec2(curScreenPos.x + avail.x - 70.f, curScreenPos.y + avail.y - 20.f));
			if (ImGui::Button("Cancel##ChangeContent", ImVec2(70.f, 20.f))) {
				CancelTempDataModify();
				m_ShowDetailGui = false;
			}
		}
		ImGui::End();
	}
	return ret;
}

void JContent::ReadyTempData()
{
	m_ModifyName = m_name;
}

void JContent::ApplyTempDataToRealData()
{
	if (m_name != m_ModifyName) {
		// rename을 해주기
		if (not std::filesystem::exists(m_FullFilePath)) {
			spdlog::error("{} - Not exist File", m_FullFilePath.string());
			return;
		}

		std::filesystem::path newPath = m_FullFilePath;

		newPath.replace_filename(m_ModifyName);
		std::filesystem::rename(m_FullFilePath, newPath);

		m_name = m_ModifyName;
		m_FullFilePath = newPath;
	}
}
#endif