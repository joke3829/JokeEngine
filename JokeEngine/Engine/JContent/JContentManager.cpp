#include "JContentManager.h"

void JContentManager::AddContent(std::shared_ptr<JContent> content) 
{
	const std::string& name = content->GetName();
	
	m_ContentLookupTable.insert({ name, content });
}