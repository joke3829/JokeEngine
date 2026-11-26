#include "JSoundObject.h"
#include "JObjectRegistry.h"
#include "Engine/JContent/JSound.h"
#include "Engine/JSoundEngine/JSoundEngine.h"
#include "Engine/JSoundEngine/JSoundResourceXAudio2.h"

// 예비
#include "Engine/JContent/JContentManager.h"

JSoundObject::JSoundObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT uid, const char* name)
	: JObject(vWorld, vWorldTP, uid, name)
{
	m_SoundResource = std::make_unique<JSoundResourceXAudio2>();

	auto& test = JGlobalContentManager::GetInstance()->GetManager()->GetContentTable();
	SetSound(test["Resource/wavTest/ok_pcm16_mono_fmt16.wav"].get());
	Play();
}

void JSoundObject::Reset()
{

}

void JSoundObject::LoadObjectFromFile(nlohmann::json& object)
{

}

void JSoundObject::Play(bool begin)
{
	m_SoundResource->Play();
}

void JSoundObject::Pause()
{

}

void JSoundObject::Stop(bool AllStop)
{

}

void JSoundObject::SetVolume()
{

}

void JSoundObject::SetSubmixGroup(std::string& submix)
{

}

void JSoundObject::SetMultiSource(UINT num)
{

}

void JSoundObject::SetSound(JContent* sound)
{
	if (auto p = dynamic_cast<JSound*>(sound)) {
		m_Sound = p;
		m_SoundResource->SetSound(p);
	}
}

#if DEV_MODE
void JSoundObject::SaveObjectToFile(nlohmann::json& json, std::unordered_map<UINT, UINT>& IDtable)
{
	JObject::SaveObjectToFile(json, IDtable);

}

bool JSoundObject::RenderInspector(UINT currentFrameIndex)
{
	bool ret = JObject::RenderInspector(currentFrameIndex);

	ImGui::SeparatorText("JSoundObject");

	ImVec2 avail = ImGui::GetContentRegionAvail();
	ImVec2 curScreenPos = ImGui::GetCursorScreenPos();
	auto* dl = ImGui::GetWindowDrawList();
	{
		ImVec2 curPos = ImGui::GetCursorPos();
		ImVec2 rectmax{ curScreenPos.x + avail.x, curScreenPos.y + 62.f };
		dl->AddRectFilled(curScreenPos, rectmax, IM_COL32(45, 50, 60, 255));
		ImGui::SetCursorPos(ImVec2(curPos.x + 3.f, curPos.y + 3.f));
		void* imageref = (m_Sound) ? JGlobalContentManager::GetInstance()->GetManager()->GetContentTable()["Resource/JKEngine/Textures/SpriteAnimationSetFile.dds"]->GetDedicatedFactor()
			: nullptr;
		ImGui::ImageButton("SpriteSetSelectButton", imageref, ImVec2(50.f, 50.f));
		if (m_Sound) {
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered() && not m_Sound->GetShowDetailPanel()) {
				m_Sound->SetDetailPanelOn();
			}
		}
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right, false)) m_Sound = nullptr;
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAG_CONTENT_BROWSER")) {
				JContent* pcon = *static_cast<JContent* const*>(payload->Data);
				JSound* pSound = dynamic_cast<JSound*>(pcon);
				if (pSound) {
					SetSound(pSound);
				}
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::SetCursorPos(ImVec2(curPos.x + 65.f, curPos.y + 6.f));
		if (m_Sound) {
			ImGui::TextAligned(0.0f, avail.x - 65.f, m_Sound->GetName().c_str());
			dl->AddLineH(curScreenPos.x + 65, curScreenPos.x + avail.x - 3.f, curScreenPos.y + 25.f, IM_COL32(255, 255, 255, 255), 0.5f);
			ImGui::SetCursorPos(ImVec2(curPos.x + 65.f, curPos.y + 33.f));
			ImGui::Text("FilePath:");
			ImGui::SetCursorPos(ImVec2(curPos.x + 65.f, curPos.y + 47.f));
			ImGui::TextAligned(0.0f, avail.x - 65.f, m_Sound->GetFullFilePath().string().c_str());
		}
		else {
			ImGui::Text("No Sound");
		}
	}

	ImGui::SetCursorScreenPos(ImVec2(curScreenPos.x, curScreenPos.y + 100.f));
	ImGui::Separator();
	if(ImGui::Button("Play##tttt")) {
		if (m_Sound)
			m_SoundResource->Play(true);
	}

	return ret;
}

#endif

REGISTER_OBJECT(JSoundObject);