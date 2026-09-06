#pragma once

#include "Engine/D2D/DWriteTextResource.h"

class JTextResourceDX11 : public JTextResourceD2D {
public:
	JTextResourceDX11(const char* name);
protected:

	ComPtr<ID3D11Texture2D> m_TextureRT{};
};