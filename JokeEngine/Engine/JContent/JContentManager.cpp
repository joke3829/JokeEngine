#include "JContentManager.h"
#include "Engine/D3D11/Content/D3D11ContentManager.h"
#include "Engine/JokeEngineConfig.h"

void JContentManager::AddContent(std::shared_ptr<JContent> content) 
{
	const std::string& name = content->GetName();
	
	m_ContentLookupTable.insert({ name, content });
}

#if DEV_MODE
void JContentManager::RenderImGuiContents()
{
	

	size_t count = m_ContentLookupTable.size();
	ImVec2 windowSize = ImGui::GetContentRegionAvail();
	ImVec2 startcursorpos = ImGui::GetCursorPos();

	// 115로 일단 잡아보기
	int col = static_cast<int>(windowSize.x / 115);
	if (col <= 1) col = 1;
	int row = (count % col) ? (count / col) + 1 : count / col;

	float stride{};
	int curRow{};
	int curCol{};
	ImVec2 curPos;

	
	// icon은 한 항목당 110*110 간격은 5~20
	for (auto& p : m_ContentLookupTable) {
		if (curCol >= col) { curCol = 0; ++curRow; }
		curPos.x = startcursorpos.x + (static_cast<float>(curCol) * 115);
		curPos.y = startcursorpos.y + (static_cast<float>(curRow) * 115);
		p.second->RenderGUI(curPos);
		++curCol;
	}
}
#endif

JGlobalContentManager* JGlobalContentManager::m_Instance = nullptr;

JGlobalContentManager* JGlobalContentManager::GetInstance()
{
	if (not m_Instance) m_Instance = new JGlobalContentManager;
	return m_Instance;
}

JContentManager* JGlobalContentManager::GetManager()
{
	return m_Manager.get();
}

JGlobalContentManager::JGlobalContentManager()
{
	auto* conf = JEngineDefaultGlobalConfig::GetInstance()->GetConfigFactor();
	switch (conf->DirectX_Version) {
	case 11:
		m_Manager = std::make_unique<JContentManagerDX11>();
		break;
	case 12:
		break;
	default:
		break;
	}

	m_Manager->ReadyDefaultContent();
}

JGlobalContentManager::~JGlobalContentManager()
{
	if (m_Instance) delete m_Instance;
}
