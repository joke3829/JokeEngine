#include "JTextObject.h"
#include "Engine/JokeEngineConfig.h"

constexpr float kTextScale = 3.0f;

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
	// 개별 행동, 딱히 뭐 없음
	m_TextResource->Update(elapsedTime);

	// 여기서 CPUDirty -> IDWrite, Brush 등 최신화
	if (m_CPUDirty) {
		m_TextResource->CreateDWriteTextFomat(m_FontName, m_FontCollectionName, m_FontSize, m_FontWeight, m_FontWidth);
		float maxsize = static_cast<float>(kTextTexture2DSize);
		m_TextResource->CreateDWriteTextLayout(m_Text, maxsize, maxsize);
		m_CPUDirty = false;

		m_TextMetrics = m_TextResource->GetMetricsWithUVMatrixUpdate();
		// uv 변환 행렬 제작
	}
	// 행렬 만들 재료 준비 혹은 여기서 직접 만들어주기
	// 월드 행렬 배열에 들어갈 저거랑 부모 행렬로 줄 저거는 다른거다.

	// JObject::Update()

	{
		XMMATRIX parentMatrix;

		if (parent) parentMatrix = XMLoadFloat4x4(parent);
		else parentMatrix = XMMatrixIdentity();
		MakeLocalTransform();
		XMMATRIX realworld = XMLoadFloat4x4(&m_LocalTransform) * parentMatrix;
		XMFLOAT4X4 realworld_f{};
		XMStoreFloat4x4(&realworld_f, realworld);

		for (auto& child : m_LeafObjects) {
			child->Update(elapsedTime, &realworld_f);
		}

		// 렌더링용 월드 행렬을 만들어 배열에 넣기
		XMFLOAT2 textscale{};

		textscale.x = m_TextMetrics.Width * kTextScale;
		textscale.y = m_TextMetrics.Height * kTextScale;

		if (m_HorizonAlignment == JText_Alignment::Center) {
			textscale.x /= 2;
			textscale.y /= 2;
		}
		XMMATRIX tscale = XMMatrixScaling(textscale.x, textscale.y, 1.f);
		XMMATRIX result = tscale * realworld;
		// 상수 버퍼에 들어가는 것만 업데이트, WorldTransform은 일단은 제외
		XMStoreFloat4x4(&m_WorldTransformTP[m_NodeIndex], XMMatrixTranspose(result));
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
