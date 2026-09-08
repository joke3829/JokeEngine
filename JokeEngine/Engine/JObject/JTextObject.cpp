#include "JTextObject.h"
#include "Engine/D3D11/GResource/D3D11TextResource.h"
#include "Engine/JokeEngineConfig.h"

JTextObject::JTextObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name)
	: JObject(vWorld, vWorldTP, nodeIndex, name)
{
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

void JTextObject::MakeDirtyFlag()
{
	for (UINT i = 0; i < kNumRenderTarget; ++i)
		m_Dirty[i] = true;
}
