#include "JTexture.h"

JTexture::JTexture(const char* name)
	: JContent(name)
{

}

JTexture::JTexture(const wchar_t* filepath, const char* name)
{
	if (name) {
		SetName(name);
	}
	else {
		m_name = std::filesystem::path(filepath).string();
	}
	LoadTextureFromFile(filepath);
}


