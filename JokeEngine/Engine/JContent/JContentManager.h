#pragma once
#include "stdafx.h"
#include "JContent.h"


class JContentManager {
public:
	void AddContentFromDirectory(std::filesystem::path filepath, bool clear = true);
	virtual bool AddContentFromFile(std::filesystem::path filepath);	// 하나의 특정 파일만 로드 할 때 -> override
	void AddContent(std::filesystem::path filepath);
	void ReSaveAll();

	size_t GetNumContents() { return m_ContentLookupTable.size(); }

	std::unordered_map<std::filesystem::path, std::shared_ptr<JContent>>& GetContentTable() { return m_ContentLookupTable; }
#if DEV_MODE 
	void RenderImGuiContents(ImVec2 startCursorPos, int row, int col, int curRow, int curCol, const std::filesystem::path& parent);
	virtual void AddMeshWithFlag(BYTE& flag, std::filesystem::path& filepath) {}
#endif
protected:
	void ReadyMissingTexture();
	// key: FullPath -> 확장자 명까지, value: 실제 Content
	std::unordered_map<std::filesystem::path, std::shared_ptr<JContent>>			m_ContentLookupTable{};
};


class JGlobalContentManager {
public:
	static JGlobalContentManager* GetInstance();
	JContentManager* GetManager();
private:
	JGlobalContentManager();
	~JGlobalContentManager();

	static JGlobalContentManager* m_Instance;
	
	std::unique_ptr<JContentManager> m_Manager{};
};