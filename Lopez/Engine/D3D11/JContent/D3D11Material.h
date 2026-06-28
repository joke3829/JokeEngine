#pragma once

#include "Engine/JContent/JMaterial.h"

struct cbSimpleMaterial {
	XMFLOAT4 Albedo{ 1.f, 1.f, 1.f, 1.f };
	XMFLOAT4 Specular{ 1.f, 1.f, 1.f, 1.f };
	XMFLOAT4 Ambient{ 1.f, 1.f, 1.f,1.f };
	XMFLOAT4 Emissive{ 0.f, 0.f, 0.f, 1.f };
};

class JMaterialSimpleDX11 final : public JMaterialSimple {
public:
	JMaterialSimpleDX11(const char* name = nullptr);

	void SetDXBuffer(UINT parameter, JShaderStage stage = JS_NONE);
private:
	ComPtr<ID3D11Buffer>						m_MaterialConstantBuffer{};
	cbSimpleMaterial							m_SimpleMaterial{};
};