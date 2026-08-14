#include "JTexture.h"

JTexture::JTexture(const char* name)
	: JContent(name)
{

}

JTexture::JTexture(const wchar_t* filepath, const char* name)
	: JContent(name)
{
	if (name) {
		SetName(name);
	}
	else {
		m_name = std::filesystem::path(filepath).string();
	}
}


