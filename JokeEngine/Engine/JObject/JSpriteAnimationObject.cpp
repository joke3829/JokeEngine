#include "JSpriteAnimationObject.h"
#include "Engine/JokeEngineConfig.h"
#include "Engine/JContent/JMaterial.h"
#include "Engine/JContent/JStaticMesh.h"
#include "Engine/D3D11/GResource/D3D11MeshConstant.h"
#include "Engine/D3D11/GResource/D3D11SpriteMeshConstant.h"
#include "Engine/JContent/JSpriteAnimationSet.h"
#include "JObjectRegistry.h"
#include "Engine/JContent/JContentManager.h"

JSpriteAnimationObject::JSpriteAnimationObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT uid, const char* name)
	: JStaticMeshObject(vWorld, vWorldTP, uid, name)
{
	auto* opt = JEngineDefaultGlobalConfig::GetInstance()->GetConfigFactor();
	switch (opt->DirectX_Version) {
	case 11:
		m_SpriteMeshCB = std::make_unique<JSpriteMeshConstantDX11>();
		break;
	case 12:
		break;
	default:
#if DEV_MODE
		spdlog::error("{0} - JSpriteAnimationMeshObject가 잘못된 DirectX Version을 받았습니다.", m_name.c_str());
#endif
		assert(0);
		break;
	}
}

JSpriteAnimationObject::~JSpriteAnimationObject()
{
}

void JSpriteAnimationObject::Reset()
{
	m_bMirror = m_bInitialMirror;
	m_CurrentAnimation.first = m_InitialAnimation;
	m_ElapsedTime = m_InitialElapsedTime;

	JObject::Reset();
}

void JSpriteAnimationObject::Update(float elapsedTime, XMFLOAT4X4* parent)
{
	m_ElapsedTime += elapsedTime;
	if (m_SpriteSet) {
		const JSAnimData& data = m_SpriteSet->GetKeyFrameData(m_CurrentAnimation.first, m_ElapsedTime);
		m_CurrentAnimation.second = data.materialIndex;

		m_SpriteMeshCB->UpdateFromData(data, m_bMirror);
	}

	JStaticMeshObject::Update(elapsedTime, parent);
}

// 말 그대로 minustime에는 음수가 들어가야한다.
void JSpriteAnimationObject::SetAnimationSet(const char* setName, float elapsedTime, float minustime, bool edit)
{
	m_CurrentAnimation.first = setName;
	m_ElapsedTime = elapsedTime + minustime;
	if (edit) {
		m_InitialAnimation = setName;
		m_InitialElapsedTime = elapsedTime;
	}
}

void JSpriteAnimationObject::SetAnimationSet(std::string& setName, float elapsedTime, float minustime, bool edit)
{
	m_CurrentAnimation.first = setName;
	m_ElapsedTime = elapsedTime + minustime;
	if (edit) {
		m_InitialAnimation = setName;
		m_InitialElapsedTime = elapsedTime;
	}
}

void JSpriteAnimationObject::Render(UINT currentFrameIndex)
{
	if (!m_StaticMesh)
		return;
	m_MeshCB->UpdateBuffer(currentFrameIndex);
	m_MeshCB->SetGPUBuffer(currentFrameIndex, 0, JS_VS);

	m_SpriteMeshCB->UpdateBuffer(currentFrameIndex);
	m_SpriteMeshCB->SetGPUBuffer(currentFrameIndex, 3, JS_VS);

	if (m_CurrentAnimation.second >= 0 && m_CurrentAnimation.second < m_Materials.size()) {
		if (m_Materials[m_CurrentAnimation.second]) {
			m_Materials[m_CurrentAnimation.second]->UpdateBuffer(currentFrameIndex);
			m_Materials[m_CurrentAnimation.second]->SetGPUBuffer(currentFrameIndex, 2, JS_PS);
			m_StaticMesh->Render();
		}
	}
}


void JSpriteAnimationObject::SetSpriteSet(JContent* spriteSet)
{
	auto p = dynamic_cast<JSpriteAnimationSet*>(spriteSet);
	if (!p) {
#if DEV_MODE
		spdlog::error("{0} JSpriteAnimationObject에서 잘못된 SetSpriteSet을 호출했습니다.", m_name.c_str());
#endif
		return;
	}
	m_SpriteSet = p;

	// 처음 AnimationSet이 붙으면 AnimationSet에 있는것으로 초기화
	m_SpriteSet->GetKeyFrameData(m_CurrentAnimation.first, m_ElapsedTime);
	m_InitialAnimation = m_CurrentAnimation.first;
}


#if DEV_MODE
bool JSpriteAnimationObject::RenderInspector(UINT currentFrameIndex)
{
	bool ret = JStaticMeshObject::RenderInspector(currentFrameIndex);

	ImVec2 avail = ImGui::GetContentRegionAvail();
	ImDrawList* dl = ImGui::GetWindowDrawList();
	if (m_PlayMode)
		ImGui::BeginDisabled(true);

	if (!m_StaticMesh) 
		for(int i = 0; i < 3; ++i)
			ImGui::Text("");

	ImGui::SeparatorText("JSpriteAnimationObject");
	ImVec2 curScreenPos = ImGui::GetCursorScreenPos();
	{
		ImVec2 curPos = ImGui::GetCursorPos();
		ImVec2 rectmax{ curScreenPos.x + avail.x, curScreenPos.y + 62.f };
		dl->AddRectFilled(curScreenPos, rectmax, IM_COL32(45, 50, 60, 255));
		ImGui::SetCursorPos(ImVec2(curPos.x + 3.f, curPos.y + 3.f));
		void* imageref = (m_SpriteSet) ? JGlobalContentManager::GetInstance()->GetManager()->GetContentTable()["Resource/JKEngine/Textures/SpriteAnimationSetFile.dds"]->GetDedicatedFactor()
			: nullptr;
		ImGui::ImageButton("SpriteSetSelectButton", imageref, ImVec2(50.f, 50.f));
		if (m_SpriteSet) {
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered() && not m_SpriteSet->GetShowDetailPanel()) {
				m_SpriteSet->SetDetailPanelOn();
			}
		}
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right, false)) m_SpriteSet = nullptr;
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAG_CONTENT_BROWSER")) {
				JContent* pcon = *static_cast<JContent* const*>(payload->Data);
				JSpriteAnimationSet* pSprite = dynamic_cast<JSpriteAnimationSet*>(pcon);
				if (pSprite) {
					SetSpriteSet(pSprite);
					if (not pSprite->GetHasKeyFrame())
						spdlog::warn("설정한 SpriteSet의 data가 없습니다.");
				}
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::SetCursorPos(ImVec2(curPos.x + 65.f, curPos.y + 6.f));
		if (m_SpriteSet) {
			ImGui::TextAligned(0.0f, avail.x - 65.f, m_SpriteSet->GetName().c_str());
			dl->AddLineH(curScreenPos.x + 65, curScreenPos.x + avail.x - 3.f, curScreenPos.y + 25.f, IM_COL32(255, 255, 255, 255), 0.5f);
			ImGui::SetCursorPos(ImVec2(curPos.x + 65.f, curPos.y + 33.f));
			ImGui::Text("FilePath:");
			ImGui::SetCursorPos(ImVec2(curPos.x + 65.f, curPos.y + 47.f));
			ImGui::TextAligned(0.0f, avail.x - 65.f, m_SpriteSet->GetFullFilePath().string().c_str());
		}
		else {
			ImGui::Text("No Set");
		}
	}
	{
		ImGui::SetCursorScreenPos(ImVec2(curScreenPos.x, curScreenPos.y + 65.f));
		ImGui::Separator();
		ImGui::Text("test");
		if (m_SpriteSet && m_SpriteSet->GetHasKeyFrame()) {
			// 애니메니이션 콤보 박스
			auto& play = m_SpriteSet->GetPlayTimeMap();
			std::vector<std::string> aninames; aninames.reserve(play.size());
			for (auto& d : play) {
				aninames.emplace_back(d.first);
			}
			if(ImGui::BeginCombo("Animations##List", m_CurrentAnimation.first.c_str())) {
				for (auto& str : aninames) {
					bool is_selected = str == m_CurrentAnimation.first;
					if (ImGui::Selectable(str.c_str(), is_selected))
						SetAnimationSet(str, 0.f, 0.f, true);

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			// 고른 애니에 대한 정보
			ImGui::Text("AnimationTime: %f", play.at(m_CurrentAnimation.first));
			ImGui::Text("UsingMaterial: %d", m_CurrentAnimation.second);
			if (ImGui::DragFloat("ElapsedTime##animation", &m_ElapsedTime, 0.125f, 0.0f, 3600.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) {
				m_InitialElapsedTime = m_ElapsedTime;
			}
		}
	}

	if (m_PlayMode)
		ImGui::EndDisabled();

	return ret;
}
#endif


REGISTER_OBJECT(JSpriteAnimationObject)