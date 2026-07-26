#pragma once

#include "Engine/JContent/JMaterial.h"

// 텍스쳐들에 대한 SRV도 가짐
// 더블 버퍼링 고려

struct cbSimpleMaterial {
	XMFLOAT4 Albedo{ 1.f, 1.f, 1.f, 1.f };
	XMFLOAT4 Specular{ 1.f, 1.f, 1.f, 1.f };
	XMFLOAT4 Ambient{ 1.f, 1.f, 1.f,1.f };
	XMFLOAT4 Emissive{ 0.f, 0.f, 0.f, 1.f };
	int AlbedoTexIndex{};
};

enum TextureOrder : unsigned char {
	Albedo,
	Normal,
	Count
};

class JTextureDX11;

class JMaterialDX11 final : public JMaterial {
public:
	JMaterialDX11(const char* name = nullptr);

	void Update(float elapsedTime) {}
	void UpdateBuffer(UINT currentFrameIndex);
	void SetDXBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage = JS_NONE);
	
	void SetTextureNull(std::shared_ptr<JContent>& nulltexture);
	void SetTexture(std::shared_ptr<JContent>& texture, TextureOrder order);

	cbSimpleMaterial& GetMaterial() { return m_SimpleMaterial; }
private:
	ComPtr<ID3D11Buffer>						m_MaterialConstantBuffer[g_NumRenderTarget]{};
	cbSimpleMaterial							m_SimpleMaterial{};

	std::vector<std::shared_ptr<JTextureDX11>>	m_Textures{};

};