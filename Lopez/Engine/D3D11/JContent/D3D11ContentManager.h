#pragma once

#include "Engine/JContent/JContentManager.h"

class JContentManagerDX11 final : public JContentManager {
public:
	void AddDefaultContent() {}							// Null Texture도 만들어 두기
	void AddContentFromDirectory(const char* dir) {}	// 그 디렉토리 안에 하위 폴더를 전부 탐색 및 콘텐츠 생성
	void AddContentFromFile(const char* filename) {}	// 하나의 특정 파일만 로드 할 때
private:
	
};