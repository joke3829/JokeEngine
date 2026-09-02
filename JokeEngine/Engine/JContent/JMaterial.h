#pragma once
#include "JContent.h"


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


class JMaterial : public JContent {
public:
	JMaterial(const char* name = nullptr);

	virtual void Update(float elapsedTime) {}
	virtual void UpdateBuffer(UINT currentFrameIndex) {}
	virtual void SetGPUBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage = JS_NONE) {}

	virtual void SetTextureNull(std::shared_ptr<JContent>& nulltexture) {}
	virtual void SetTexture(std::shared_ptr<JContent>& texture, TextureOrder order) {}
};


class JDMaterial : public JMaterial {
public:
	JDMaterial(const char* name = nullptr);

	cbSimpleMaterial& GetMaterial() { return m_SimpleMaterial; }
protected:
	cbSimpleMaterial							m_SimpleMaterial{};
	// 실제 텍스쳐 리스트는 DX쪽에서 자신의 버젼에 맞는 것의 리스트로 가진다.
};




// material 추가 원할 시 cb와 그에 따른 요소 추가 - default는 JMaterial이다.
