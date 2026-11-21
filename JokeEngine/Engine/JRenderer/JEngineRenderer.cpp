#include "JEngineRenderer.h"
#include "Engine/JContent/JContentManager.h"



JEngineRenderer::JEngineRenderer(UINT width, UINT height, const char* name)
	: JCommon(name)
{
	CreateRTV_DSV(width, height);
	m_TargetSize.x = width; m_TargetSize.y = height;
}

void JEngineRenderer::ResizeTarget(UINT width, UINT height)
{
	CreateRTV_DSV(width, height);
	m_TargetSize.x = width; m_TargetSize.y = height;
}

void JEngineRenderer::RenderFrame()
{
	if (not m_Scene)
		return;
	if (m_Scene->GetDirty()) {
		// 큐를 꺼내서 찾아 해메
		auto& q = m_Scene->GetCDQueue();
		while (not q.empty()) {
			auto& info = q.front();
			if (m_PSOMap.contains(info.shadername)) {
				if (info.object) {	// 추가
					m_PSOMap[info.shadername]->AddObject(info.uid, info.object);	// 방어 없음
				}
				else {		// 제거
					m_PSOMap[info.shadername]->DeleteObject(info.uid);
				}
			}
			q.pop();
		}
		m_Scene->SetDirty(false);
	}
}

void JEngineRenderer::AdvanceBufferIndex()
{
	m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % kNumRenderTarget;
}


#if DEV_MODE
void JEngineRenderer::RenderPSOMapImGui()
{
	size_t count = m_PSOMap.size();
	ImVec2 windowSize = ImGui::GetContentRegionAvail();
	ImVec2 startPos = ImGui::GetCursorScreenPos();

	int col = static_cast<int>(windowSize.x / 115);
	if (col <= 1) col = 1;
	int row = (count % col) ? (count / col) + 1 : count / col;

	int curRow{};
	int curCol{};
	ImVec2 curPos;
	int curCount{};

	auto& content = JGlobalContentManager::GetInstance()->GetManager()->GetContentTable();
	
	for (auto& pso : m_PSOMap) {
		if (curCol >= col) { curCol = 0; ++curRow; }
		curPos.x = startPos.x + (static_cast<float>(curCol) * 115);
		curPos.y = startPos.y + (static_cast<float>(curRow) * 115);
		ImVec2 ButtonPos = curPos;
		ButtonPos.x += 10;
		ImGui::SetCursorScreenPos(ButtonPos);
		void* imageref = (content.contains("Resource/JKEngine/Textures/Shader.dds")) ? content["Resource/JKEngine/Textures/Shader.dds"]->GetDedicatedFactor() : nullptr;
		ImGui::ImageButton(("##" + pso.first + std::to_string(curCount)).c_str(), imageref, ImVec2(90.f, 90.f));
		if (ImGui::BeginDragDropSource()) {
			const char* shadername = pso.first.c_str();
			ImGui::SetDragDropPayload("DRAG_SHADERNAME", shadername, strlen(shadername) + 1);
			ImGui::Text(shadername);
			ImGui::EndDragDropSource();
		}

		ImVec2 TextPos = curPos; TextPos.y += 96; TextPos.x += 5.5f;
		ImGui::SetCursorScreenPos(TextPos);
		ImGui::TextAligned(0.5f, 110, pso.first.c_str());
		++curCount;
		++curCol;
	}
}
#endif