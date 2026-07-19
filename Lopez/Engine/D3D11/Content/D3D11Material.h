#pragma once

#include "Engine/JContent/JMaterial.h"

// 텍스쳐들에 대한 SRV도 가짐
// 더블 버퍼링 고려

struct cbSimpleMaterial {
	XMFLOAT4 Albedo{ 1.f, 1.f, 1.f, 1.f };
	XMFLOAT4 Specular{ 1.f, 1.f, 1.f, 1.f };
	XMFLOAT4 Ambient{ 1.f, 1.f, 1.f,1.f };
	XMFLOAT4 Emissive{ 0.f, 0.f, 0.f, 1.f };
};

class JMaterialDX11 final : public JMaterial {
public:
	JMaterialDX11(const char* name = nullptr);

	void Update();
	void SetDXBuffer(UINT parameter, JShaderStage stage = JS_NONE);

	cbSimpleMaterial& GetMaterial() { return m_SimpleMaterial; }
private:
	ComPtr<ID3D11Buffer>						m_MaterialConstantBuffer[g_{};
	cbSimpleMaterial							m_SimpleMaterial{};
};