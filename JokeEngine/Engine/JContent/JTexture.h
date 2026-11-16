#pragma once
#include "JContent.h"


class JTexture : public JContent{
public:
	JTexture(const char* name) : JContent(name) {}
	JTexture(std::filesystem::path name) : JContent(name) {}

	virtual void LoadTextureFromFile(const wchar_t* filepath) {}
	virtual void MakeMissingTexture() {}
protected:
	virtual void CreateContentFile() { if(not m_Created) MakeMissingTexture(); }
	bool m_Created{}; // 이미 만들어졌나?
};