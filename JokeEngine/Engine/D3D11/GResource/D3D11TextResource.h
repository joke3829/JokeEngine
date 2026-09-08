#pragma once

#include "Engine/D2D/DWriteTextResource.h"



class JTextResourceDX11 : public JTextResourceD2D {
public:
	JTextResourceDX11(const char* name);

	void Update(float elapsedTime);
	void UpdateBuffer(UINT currentFrameIndex);
	void SetGPUBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage = JS_NONE);
protected:
	void ReadyDX11Resource();

	std::string m_ResourceNames[kNumRenderTarget]{};

	struct cbUV {
		XMFLOAT4X4 uvMatrix{};
	}m_uvTransform;
	ComPtr<ID3D11Buffer>				m_cbUVTransform[kNumRenderTarget]{};
	ComPtr<ID3D11Texture2D>				m_TextureRT[kNumRenderTarget]{};
	ComPtr<ID3D11ShaderResourceView>	m_TextureRTSRV[kNumRenderTarget]{};
};