#include "D3D11SpriteMeshConstant.h"
#include "Engine/JContent/JSpriteAnimationSet.h"
#include "D3D11GlobalFactor.h"

JSpriteMeshConstantDX11::JSpriteMeshConstantDX11()
	: JSpriteMeshConstant()
{
	BufferReady();
}


void JSpriteMeshConstantDX11::UpdateBuffer(UINT currentFrameIndex)
{
	// Set된 값과 같으면 Map 하지 않음
	if (m_CBSprite == m_CurrentSetCB[currentFrameIndex])
		return;

	auto* context = JD3D11GlobalFactor::GetInstance()->GetDeviceContext();
	D3D11_MAPPED_SUBRESOURCE mapped{};
	context->Map(m_CBBuffer[currentFrameIndex].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, &m_CBSprite, sizeof(CB_Sprite));
	context->Unmap(m_CBBuffer[currentFrameIndex].Get(), 0);

	m_CurrentSetCB[currentFrameIndex] = m_CBSprite;
}

void JSpriteMeshConstantDX11::SetDXBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage)
{
	auto* context = JD3D11GlobalFactor::GetInstance()->GetDeviceContext();

	switch (stage) {
	case JS_VS:
		context->VSSetConstantBuffers(parameter, 1, m_CBBuffer[currentFrameIndex].GetAddressOf());
		break;
	case JS_PS:
		context->PSSetConstantBuffers(parameter, 1, m_CBBuffer[currentFrameIndex].GetAddressOf());
		break;
	case JS_GS:
		context->GSSetConstantBuffers(parameter, 1, m_CBBuffer[currentFrameIndex].GetAddressOf());
		break;
	case JS_HS:
		context->HSSetConstantBuffers(parameter, 1, m_CBBuffer[currentFrameIndex].GetAddressOf());
		break;
	case JS_DS:
		context->DSSetConstantBuffers(parameter, 1, m_CBBuffer[currentFrameIndex].GetAddressOf());
		break;
	case JS_CS:
		context->CSSetConstantBuffers(parameter, 1, m_CBBuffer[currentFrameIndex].GetAddressOf());
		break;
	default:
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("[DX11] MeshConstant 에서 잘못된 SetDXBuffer를 호출했습니다.");
#endif
		assert(0);
	}
}

void JSpriteMeshConstantDX11::BufferReady()
{
	auto* device = JD3D11GlobalFactor::GetInstance()->GetDevice();

	D3D11_BUFFER_DESC desc{
		.ByteWidth = Align(sizeof(CB_Sprite), 16),
		.Usage = D3D11_USAGE_DYNAMIC,
		.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
		.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
	};
	for (int i = 0; i < g_NumRenderTarget; ++i) {
		ThrowIfFailed(device->CreateBuffer(&desc, nullptr, m_CBBuffer[i].ReleaseAndGetAddressOf()));
	}
}
