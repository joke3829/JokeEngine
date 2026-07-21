#pragma once
#include "stdafx.h"
#include "JContent.h"


class JContentManager {
public:
	
	virtual void ReadyDefaultContent() {}							// Null Texture도 만들어 두기
	virtual void AddContentFromDirectory(const char* dir) {}	// 그 디렉토리 안에 하위 폴더를 전부 탐색 및 콘텐츠 생성
	virtual void AddContentFromFile(const char* filename) {}	// 하나의 특정 파일만 로드 할 때
	void AddContent(std::shared_ptr<JContent> content);

	std::unordered_map<std::string, std::shared_ptr<JContent>>& GetContentTable() { return m_ContentLookupTable; }
protected:


	std::unordered_map<std::string, std::shared_ptr<JContent>>			m_ContentLookupTable{};
};