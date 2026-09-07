#pragma once

#include "Engine/JContent/JMaterial.h"

// 텍스쳐들에 대한 SRV도 가짐
// 더블 버퍼링 고려



class JTextureDX11;

class JMaterialDX11 final : public JDMaterial {
public:
	JMaterialDX11(const char* name = nullptr);

	void Update(float elapsedTime) {}
	void UpdateBuffer(UINT currentFrameIndex);
	void SetGPUBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage = JS_NONE);
	
	void SetTextureNull(std::shared_ptr<JContent>& nulltexture);
	void SetTexture(std::shared_ptr<JContent>& texture, TextureOrder order);
private:
	ComPtr<ID3D11Buffer>						m_MaterialConstantBuffer[kNumRenderTarget]{};

	std::vector<std::shared_ptr<JTextureDX11>>	m_Textures{};
};