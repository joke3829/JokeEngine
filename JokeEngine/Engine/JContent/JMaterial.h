#pragma once
#include "JContent.h"

// 마테리얼은 이걸로 고정한다. 사용자가 슬롯에 꼭 맞는 텍스쳐를 낄 필요는 없다.
// 자신만의 슬롯으로 넣는다면 셰이더 단계에서 사용하도록 하자
// 그리고 roughness metallic으로 바꿀거임
struct cbSimpleMaterial {
	XMFLOAT4 BaseColor{ 1.f, 1.f, 1.f, 1.f };
	XMFLOAT4 Emissive{};		// .a는 강도
	XMFLOAT2 MetallicRoughnessFactor{};
	float padding[2]{};

	int BaseColorTexIndex{ -1 };
	int NormalTexIndex{ -1};
};

enum TextureOrder : unsigned char {
	BaseColor,
	Normal,
	Count
};

class JTexture;


class JMaterial : public JContent {
public:
	JMaterial(const char* name = nullptr);

	virtual void Update(float elapsedTime) {}
	virtual void UpdateBuffer(UINT currentFrameIndex) {}
	virtual void SetGPUBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage = JS_NONE) {}

	virtual void SetTexture(JContent* texture, unsigned char order) {}
	virtual void DettachTexture(unsigned char order);

#if DEV_MODE
public:
	void RenderGUI(ImVec2 startPos, void* imageref = nullptr);
#endif
protected:
	std::vector<JTexture*>	m_Textures{};
};


class JDMaterial : public JMaterial {
public:
	JDMaterial(const char* name = nullptr);

	virtual void SetTexture(JContent* texture, unsigned char order);
	virtual void DettachTexture(unsigned char order);

	cbSimpleMaterial& GetMaterial() { return m_SimpleMaterial; }
#if DEV_MODE
public:
	virtual void RenderDetailPanel();
#endif
protected:
	cbSimpleMaterial							m_SimpleMaterial{};
	// 실제 텍스쳐 리스트는 DX쪽에서 자신의 버젼에 맞는 것의 리스트로 가진다.
};




// material 추가 원할 시 cb와 그에 따른 요소 추가 - default는 JMaterial이다.
