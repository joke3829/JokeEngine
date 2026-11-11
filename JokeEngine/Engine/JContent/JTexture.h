#pragma once
#include "JContent.h"


class JTexture : public JContent{
public:
	JTexture(const char* name) : JContent(name) {}
	JTexture(std::filesystem::path name) : JContent(name) {}

	virtual void LoadTextureFromFile(const wchar_t* filepath) {}
	virtual void MakeMissingTexture() {}

protected:
	virtual void CreateContentFile() { MakeMissingTexture(); }
};