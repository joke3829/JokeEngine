#include "D3D11Texture.h"
#include "Engine/D3D11/D3D11GlobalFactor.h"

JTextureDX11::JTextureDX11(const char* name)
	: JTexture(name)
{

}

JTextureDX11::JTextureDX11(const wchar_t* filepath, const char* name)
	: JTexture(filepath, name)
{

}

void JTextureDX11::LoadTextureFromFile(const wchar_t* filepath)
{
	auto path = std::filesystem::path(filepath);
	auto ext = path.extension();

	auto* device = JD3D11GlobalFactor::GetInstance()->GetDevice();
	auto* context = JD3D11GlobalFactor::GetInstance()->GetDeviceContext();
	if (ext == ".dds") {
		ID3D11Resource** temp = reinterpret_cast<ID3D11Resource**>(m_Texture.ReleaseAndGetAddressOf());
		ThrowIfFailed(DirectX::CreateDDSTextureFromFile(
			device,
			filepath,
			temp,
			m_SRV.ReleaseAndGetAddressOf()
		));
	}
	else {

	}
}

void JTextureDX11::MakeNullTexture()
{
	auto* device = JD3D11GlobalFactor::GetInstance()->GetDevice();

	D3D11_TEXTURE2D_DESC desc{
		.Width = 1,
		.Height = 1,
		.MipLevels = 1,
		.ArraySize = 1,
		.Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
		.SampleDesc = {.Count = 1},
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_SHADER_RESOURCE
	};

	ThrowIfFailed(device->CreateTexture2D(&desc, nullptr, m_Texture.ReleaseAndGetAddressOf()));

	D3D11_SHADER_RESOURCE_VIEW_DESC sdesc{
		.Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
		.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D
	};

	ThrowIfFailed(device->CreateShaderResourceView(m_Texture.Get(), nullptr, m_SRV.ReleaseAndGetAddressOf()));
}
