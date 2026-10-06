#include "D3D11TextResource.h"
#include "Engine/D3D11/D3D11GlobalFactor.h"


JTextResourceDX11::JTextResourceDX11(const char* name)
{
	m_name = name;

	ReadyDX11Resource();
}

void JTextResourceDX11::Update(float elapsedTime)
{

}

void JTextResourceDX11::UpdateBuffer(UINT currentFrameIndex)
{
	JTextResourceD2D::UpdateBuffer(currentFrameIndex);
	// cb 업데이트

	D3D11_MAPPED_SUBRESOURCE data{};
	auto* context = JD3D11GlobalFactor::GetInstance()->GetDeviceContext();
	context->Map(m_cbUVTransform[currentFrameIndex].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	memcpy(data.pData, &m_uvTransform, sizeof(XMFLOAT4X4));
	context->Unmap(m_cbUVTransform[currentFrameIndex].Get(), 0);
}

// 일단은 그냥 srv 하나면 넣기 parameter는 srv (t)
void JTextResourceDX11::SetGPUBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage)
{
	auto* context =JD3D11GlobalFactor::GetInstance()->GetDeviceContext();
	context->VSSetConstantBuffers(3, 1, m_cbUVTransform[currentFrameIndex].GetAddressOf());
	switch (stage) {
	case JS_VS:
		context->VSSetShaderResources(parameter, 1, m_TextureRTSRV[currentFrameIndex].GetAddressOf());
		break;
	case JS_PS:
		context->PSSetShaderResources(parameter, 1, m_TextureRTSRV[currentFrameIndex].GetAddressOf());
		break;
	case JS_GS:
		context->GSSetShaderResources(parameter, 1, m_TextureRTSRV[currentFrameIndex].GetAddressOf());
		break;
	case JS_HS:
		context->HSSetShaderResources(parameter, 1, m_TextureRTSRV[currentFrameIndex].GetAddressOf());
		break;
	case JS_DS:
		context->DSSetShaderResources(parameter, 1, m_TextureRTSRV[currentFrameIndex].GetAddressOf());
		break;
	case JS_CS:
		context->CSSetShaderResources(parameter, 1, m_TextureRTSRV[currentFrameIndex].GetAddressOf());
		break;
	default:
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("[DX11] {0}에서 잘못된 SetGPUBuffer를 호출했습니다.", m_name.c_str());
#endif
		assert(0);
	}
}

JTextMetrics JTextResourceDX11::GetMetricsWithUVMatrixUpdate()
{
	JTextMetrics metric = JTextResourceD2D::GetMetrics();

	float rtSize = static_cast<float>(kTextTexture2DSize);
	float uSize = metric.Width / rtSize;
	float vSize = metric.Height / rtSize;
	float offsetU = metric.Left / rtSize;
	float offsetV = metric.Top / rtSize;

	XMFLOAT4X4 uvmat{};
	uvmat._11 = uSize;
	uvmat._22 = vSize;
	uvmat._33 = 1;
	uvmat._44 = 1;
	uvmat._14 = offsetU;
	uvmat._24 = offsetV;

	m_uvTransform.uvMatrix = uvmat;

	return metric;
}

void JTextResourceDX11::ReadyDX11Resource()
{
	auto* device = JD3D11GlobalFactor::GetInstance()->GetDevice();
	auto* d2d = JD2DTextEngine::GetInstance();
	{
		D3D11_BUFFER_DESC bdesc{
			.ByteWidth = sizeof(cbUV),
			.Usage = D3D11_USAGE_DYNAMIC,
			.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
			.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
		};

		D3D11_TEXTURE2D_DESC tdesc{
			.Width = kTextTexture2DSize,
			.Height = kTextTexture2DSize,
			.MipLevels = 1,
			.ArraySize = 1,
			.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
			.SampleDesc = {.Count = 1},
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		};

		for (UINT i = 0; i < kNumRenderTarget; ++i) {
			ThrowIfFailed(device->CreateBuffer(&bdesc, nullptr, m_cbUVTransform[i].ReleaseAndGetAddressOf()));
			ThrowIfFailed(device->CreateTexture2D(&tdesc, nullptr, m_TextureRT[i].ReleaseAndGetAddressOf()));
			ThrowIfFailed(device->CreateShaderResourceView(m_TextureRT[i].Get(), nullptr, m_TextureRTSRV[i].ReleaseAndGetAddressOf()));

			m_ResourceNames[i] = m_name + "_" + std::to_string(i);
			d2d->AddRTForD3D11Texture2D(m_ResourceNames[i], m_TextureRT[i].Get());
		}
	}

}
