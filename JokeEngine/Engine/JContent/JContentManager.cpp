#include "JContentManager.h"
#include "Engine/D3D11/Content/D3D11ContentManager.h"
#include "Engine/JokeEngineConfig.h"
#include "JSpriteAnimationSet.h"
#include "JSound.h"


// 그 디렉토리 안에 하위 폴더를 전부 탐색 및 콘텐츠 생성
// 누적을 원하면 clear = false
void JContentManager::AddContentFromDirectory(std::filesystem::path filepath, bool clear)
{
	if(clear)
		m_ContentLookupTable.clear(); // 부르기 전 clear

	// missing texture부터 준비
	ReadyMissingTexture();

	std::vector<std::filesystem::path> matFiles;
	for (const auto& entry : std::filesystem::recursive_directory_iterator(filepath)) {
		if (std::filesystem::is_directory(entry.path())) continue;
		if (entry.path().extension() != ".jkmt") {
			AddContentFromFile(entry.path());
			continue;
		}
		matFiles.emplace_back(entry.path());
	}

	for (auto& matpath : matFiles) {
		AddContentFromFile(matpath);
	}
}

bool JContentManager::AddContentFromFile(std::filesystem::path filepath)
{
	if (filepath.has_extension()) {
		const auto& ext = filepath.extension();
		std::shared_ptr<JContent> content{};
		if (ext == ".jkspt") 
			content = std::make_shared<JSpriteAnimationSet>(filepath);
		else if (ext == ".wav" || ext == ".WAV" || ext == ".mp3" || ext == ".ogg")
			content = std::make_shared<JSound>(filepath);

		if (content) {
			if (not content->Initialize()) {
#if DEV_MODE
				spdlog::warn("{}: Load Failed - Currupted or Not Supported File", filepath.string());
#endif
				return false;
			}
			m_ContentLookupTable.try_emplace(filepath, content);
			return true;
		}
	}
	return false;
}

void JContentManager::ReadyMissingTexture()
{
	AddContentFromFile("Resource/JKEngine/Textures/MissingTexture.png");
}

void JContentManager::AddContent(std::filesystem::path filepath)
{
	AddContentFromFile(filepath);
}

// contentmanager에 있는 모든 것의 파일 최신화 (material 내 기록된 참조 경로 등)
void JContentManager::ReSaveAll()
{
	for (auto& item : m_ContentLookupTable)
		item.second->SaveFile();
}

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
}

JGlobalContentManager::~JGlobalContentManager()
{
	if (m_Instance) delete m_Instance;
}


#if DEV_MODE

extern bool g_ModifiedContentPath;

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
		std::filesystem::path oldPath = p.second->GetFullFilePath();
		if (p.second->CheckRenderDetailPanelAndRender()) {
			if (oldPath != p.second->GetFullFilePath()) {
				auto con = m_ContentLookupTable.extract(oldPath);
				if (not con.empty()) {
					con.key() = p.second->GetFullFilePath();
					m_ContentLookupTable.insert(std::move(con));
					g_ModifiedContentPath = true;
				}
			}
		}
	}
}
#endif