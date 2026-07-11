#include "D3D11Renderer.h"
#include "Engine/D3D11/D3D11GlobalFactor.h"

void JEngineRendererDX11::CreateRTV_DSV(UINT width, UINT height)
{
	auto* device = JD3D11GlobalFactor::GetInstance()->GetDevice();

	D3D11_TEXTURE2D_DESC desc{
		.Width = width,
		.Height = height,
		.ArraySize = 1,
		.SampleDesc = {.Count = 1},
		.Usage = D3D11_USAGE_DEFAULT,
	};

	for (UINT i = 0; i < g_NumRenderTarget; ++i) {
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET;
		ThrowIfFailed(device->CreateTexture2D(&desc, nullptr, m_RenderTarget[i].ReleaseAndGetAddressOf()));

		// ReverseZ를 고려해보자
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		ThrowIfFailed(device->CreateTexture2D(&desc, nullptr, m_DepthStencil[i].ReleaseAndGetAddressOf()));


		ThrowIfFailed(device->CreateRenderTargetView(m_RenderTarget[i].Get(), nullptr, m_RenderTargetView[i].ReleaseAndGetAddressOf()));
		ThrowIfFailed(device->CreateDepthStencilView(m_DepthStencil[i].Get(), nullptr, m_DepthStencilView[i].ReleaseAndGetAddressOf()));
	}
#if defined(_DEBUG) || define(DEBUG)
	spdlog::info("{} RTV_DSV Ready", m_name.c_str());
#endif
}