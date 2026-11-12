#pragma once 

#include "Engine/JContent/JTexture.h"
#include "Package/DirectXTex/DDSTextureLoader11.h"
#include "Package/DirectXTex/WICTextureLoader11.h"

class JTextureDX11 : public JTexture {
public:
	JTextureDX11(const char* name = "MissingTexture.png") : JTexture(name) {};
	JTextureDX11(std::filesystem::path name) : JTexture(name) {};

	ID3D11Texture2D* GetTexture2D() { return m_Texture.Get(); }
	ID3D11ShaderResourceView* GetSRV() { return m_SRV.Get(); }

#if DEV_MODE
public:
	void RenderGUI(ImVec2 startPos, void* imageref = nullptr);
#endif
protected:
	void MakeMissingTexture();

	// 있다면 그걸 Load하라
	void LoadFromFile();

	ComPtr<ID3D11Texture2D>				m_Texture{};
	ComPtr<ID3D11ShaderResourceView>	m_SRV{};
};