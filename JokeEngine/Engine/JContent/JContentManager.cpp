#include "JContentManager.h"
#include "Engine/D3D11/Content/D3D11ContentManager.h"
#include "Engine/JokeEngineConfig.h"

void JContentManager::AddContent(std::shared_ptr<JContent> content) 
{
	m_ContentLookupTable.insert({ content->GetFullFilePath(), content});
}

#if DEV_MODE
void JContentManager::RenderImGuiContents(ImVec2 startCursorPos, int row, int col, int curRow, int curCol, const std::filesystem::path& parent)
{
	int srow = curRow;
	int scol = curCol;
	ImVec2 curPos;

	
	// icon은 한 항목당 110*110 간격은 5~20
	for (auto& p : m_ContentLookupTable) {
		if (scol >= col) { scol = 0; ++srow; }
		if (p.second->GetFullFilePath().parent_path() == parent) {
			curPos.x = startCursorPos.x + (static_cast<float>(scol) * 115);
			curPos.y = startCursorPos.y + (static_cast<float>(srow) * 115);
			p.second->RenderGUI(curPos);
			++scol;
		}
		p.second->CheckRenderDetailPanelAndRender();
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
