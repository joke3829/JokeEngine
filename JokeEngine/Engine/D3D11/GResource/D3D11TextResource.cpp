#include "D3D11TextResource.h"
#include "Engine/D3D11/D3D11GlobalFactor.h"

constexpr UINT kTextTexture2DSize = 256;

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
}

void JTextResourceDX11::SetGPUBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage)
{
}

void JTextResourceDX11::ReadyDX11Resource()
{
	auto* device = JD3D11GlobalFactor::GetInstance()->GetDevice();
	{
		D3D11_BUFFER_DESC desc{
			.ByteWidth = sizeof(cbUV),
			.Usage = D3D11_USAGE_DYNAMIC,
			.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
			.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
		};

		for (UINT i = 0; i < kNumRenderTarget; ++i)
			ThrowIfFailed(device->CreateBuffer(&desc, nullptr, m_cbUVTransform[i].ReleaseAndGetAddressOf()));
	}
	{
		D3D11_TEXTURE2D_DESC desc{
			.Width = kTextTexture2DSize,
			.Height = kTextTexture2DSize,
			.MipLevels = 1,
			.ArraySize = 1,
			.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
			.SampleDesc = {.Count = 1},
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		};

		for (UINT i = 0; i < kNumRenderTarget; ++i)
			ThrowIfFailed(device->CreateTexture2D(&desc, nullptr, m_TextureRT[i].ReleaseAndGetAddressOf()));
	}
	{
		for (UINT i = 0; i < kNumRenderTarget; ++i)
			ThrowIfFailed(device->CreateShaderResourceView(m_TextureRT[i].Get(), nullptr, m_TextureRTSRV[i].ReleaseAndGetAddressOf()));
	}
}
