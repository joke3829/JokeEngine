#include "JTextObject.h"
#include "Engine/D3D11/GResource/D3D11TextResource.h"
#include "Engine/JokeEngineConfig.h"

JTextObject::JTextObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name)
	: JObject(vWorld, vWorldTP, nodeIndex, name)
{
	m_Text = L"Text";
	m_FontName = L"바탕";
	m_FontSize = 12.f;

	auto* opt = JEngineDefaultGlobalConfig::GetInstance()->GetConfigFactor();
	switch (opt->DirectX_Version) {
	case 11:
		m_TextResource = std::make_unique<JTextResourceDX11>(name);
		break;
	case 12:
		break;
	default:
		// 문제 예외 처리
		break;
	}

	MakeDirtyFlag();
}

JTextObject::~JTextObject() = default;

void JTextObject::Update(float elapsedTime, XMFLOAT4X4* parent)
{
	// 여기서 CPUDirty -> IDWrite, Brush 등 최신화
	m_TextResource->Update(elapsedTime);
	if (m_CPUDirty) {
		m_TextResource->CreateDWriteTextFomat(m_FontName, m_FontCollectionName, m_FontSize, m_FontWeight, m_FontWidth);
		float maxsize = static_cast<float>(kTextTexture2DSize);
		m_TextResource->CreateDWriteTextLayout(m_Text, maxsize, maxsize);
		m_CPUDirty = false;
	}
}

void JTextObject::Render(UINT currentFrameIndex)
{
	// updatebuffer = 렌더타겟에 텍스트 쓰기
	if (m_GPUDirty[currentFrameIndex]) {
		// 이거 조금 고민해보기 (updateBuffer라는 이름이 texture 업데이트만 하면 되는가?
		m_TextResource->UpdateBuffer(currentFrameIndex);
		m_GPUDirty[currentFrameIndex] = false;
	}
	// setGPUBuffer = SRV로 올리기

	// render 
}

void JTextObject::SetFontSize(float size)
{
	if (size < 0) {
		m_FontSize = 0; 
		return;
	}
	m_FontSize = size;
}

void JTextObject::SetFontWeight(float weight)
{
	if (weight < 1) {
		m_FontWeight = 1;
		return;
	}

	if (weight > 1000) {
		m_FontWeight = 1000;
		return;
	}

	m_FontWeight = weight;
}

void JTextObject::SetFontWidth(float width)
{
	m_FontWidth = width <= 0 ? 1 : width;
}

void JTextObject::MakeDirtyFlag()
{
	m_CPUDirty = true;
	for (UINT i = 0; i < kNumRenderTarget; ++i)
		m_GPUDirty[i] = true;
}
