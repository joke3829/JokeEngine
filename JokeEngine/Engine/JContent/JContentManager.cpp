#include "JContentManager.h"
#include "Engine/D3D11/Content/D3D11ContentManager.h"
#include "Engine/JokeEngineConfig.h"

void JContentManager::AddContent(std::shared_ptr<JContent> content) 
{
	const std::string& name = content->GetName();
	
	m_ContentLookupTable.insert({ name, content });
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

	m_Manager->ReadyDefaultContent();
}

JGlobalContentManager::~JGlobalContentManager()
{
	if (m_Instance) delete m_Instance;
}
