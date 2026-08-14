#include "D3D11SpriteMeshConstant.h"
#include "Engine/JContent/JSpriteAnimationSet.h"
#include "D3D11GlobalFactor.h"

JSpriteMeshConstantDX11::JSpriteMeshConstantDX11()
	: JSpriteMeshConstant()
{
	BufferReady();
}


void JSpriteMeshConstantDX11::UpdateFromData(const JSAnimData& data, bool mirror)
{
	// 1. 스프라이트 1개 셀이 전체 UV(0~1)에서 차지하는 비율(Scale) 계산
	float scaleX = data.perWidth / data.ImageWidth;
	float scaleY = data.perHeight / data.ImageHeight;

	// 2. 행/열 위치에 따른 기본 UV 시작 오프셋(Translation) 계산
	float offsetX = data.col * scaleX;
	float offsetY = data.row * scaleY;

	XMMATRIX transformMatrix;

	if (mirror)
	{
		// [좌우 반전 시]
		// X축 스케일을 - scaleX 로 뒤집고, 
		// 오프셋을 col 위치가 아닌 (col + 1) 셀의 우측 끝 지점으로 이동시킵니다.
		XMMATRIX scaleMat = XMMatrixScaling(-scaleX, scaleY, 1.0f);
		XMMATRIX transMat = XMMatrixTranslation(offsetX + scaleX, offsetY, 0.0f);

		transformMatrix = scaleMat * transMat;
	}
	else
	{
		// [일반 상태]
		XMMATRIX scaleMat = XMMatrixScaling(scaleX, scaleY, 1.0f);
		XMMATRIX transMat = XMMatrixTranslation(offsetX, offsetY, 0.0f);

		transformMatrix = scaleMat * transMat;
	}

	XMStoreFloat4x4(&m_CBSprite.uvMatrix, XMMatrixTranspose(transformMatrix));
}

void JSpriteMeshConstantDX11::UpdateBuffer(UINT currentFrameIndex)
{
	auto* context = JD3D11GlobalFactor::GetInstance()->GetDeviceContext();
	D3D11_MAPPED_SUBRESOURCE mapped{};
	context->Map(m_CBBuffer[currentFrameIndex].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, &m_CBSprite, sizeof(CB_Sprite));
	context->Unmap(m_CBBuffer[currentFrameIndex].Get(), 0);
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
