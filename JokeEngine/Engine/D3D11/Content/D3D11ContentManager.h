#pragma once

#include "Engine/JContent/JContentManager.h"

class JContentManagerDX11 final : public JContentManager {
public:
	void ReadyDefaultContent();							// Null Texture도 만들어 두기
	void AddContentFromFile(std::filesystem::path filepath);
private:
	
};