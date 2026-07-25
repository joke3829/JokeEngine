#pragma once 

#include "Engine/JContent/JTexture.h"
#include "Package/DirectXTex/DDSTextureLoader11.h"
#include "Package/DirectXTex/WICTextureLoader11.h"

class JTextureDX11 : public JTexture {
public:
	JTextureDX11(const char* name = "NullTexture");
	JTextureDX11(const wchar_t* filepath, const char* name = nullptr);

	void LoadTextureFromFile(const wchar_t* filepath);
	void MakeNullTexture();

	ID3D11Texture2D* GetTexture2D() { return m_Texture.Get(); }
	ID3D11ShaderResourceView* GetSRV() { return m_SRV.Get(); }

protected:
	ComPtr<ID3D11Texture2D>				m_Texture{};
	ComPtr<ID3D11ShaderResourceView>	m_SRV{};
};