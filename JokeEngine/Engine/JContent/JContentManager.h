#pragma once
#include "stdafx.h"
#include "JContent.h"


class JContentManager {
public:
	
	virtual void ReadyDefaultContent() {}							// Null Texture도 만들어 두기
	virtual void AddContentFromDirectory(const char* dir) {}	// 그 디렉토리 안에 하위 폴더를 전부 탐색 및 콘텐츠 생성
	virtual void AddContentFromFile(const char* filename) {}	// 하나의 특정 파일만 로드 할 때
	void AddContent(std::shared_ptr<JContent> content);

	size_t GetNumContents() { return m_ContentLookupTable.size(); }

	std::unordered_map<std::filesystem::path, std::shared_ptr<JContent>>& GetContentTable() { return m_ContentLookupTable; }
#if DEV_MODE 
	void RenderImGuiContents(ImVec2 startCursorPos, int row, int col, int curRow, int curCol, const std::filesystem::path& parent);
#endif
protected:

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