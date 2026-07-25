#pragma once
#include "JContent.h"


class JTexture : public JContent{
public:
	JTexture(const char* name = "NullTexture");
	JTexture(const wchar_t* filepath, const char* name = nullptr);	// name이 없으면 filepath가 곧 이름

	virtual void LoadTextureFromFile(const wchar_t* filepath) {}
	virtual void MakeNullTexture() {}

};