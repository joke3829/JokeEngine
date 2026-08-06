#include "D3D11Material.h"
#include "../D3D11GlobalFactor.h"
#include "D3D11Texture.h"

JMaterialDX11::JMaterialDX11(const char* name)
	: JDMaterial(name)
{
	auto* gFactor = JD3D11GlobalFactor::GetInstance();
	auto* device = gFactor->GetDevice();
	

	// Usage, BindFlag 변화 고려
	D3D11_BUFFER_DESC desc{
	.ByteWidth = Align(sizeof(m_SimpleMaterial), 16),
	.Usage = D3D11_USAGE_DYNAMIC,
	.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
	.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
	};
	
	for(UINT i = 0 ; i < g_NumRenderTarget; ++i)
		ThrowIfFailed(device->CreateBuffer(&desc, nullptr, m_MaterialConstantBuffer[i].GetAddressOf()));

	m_Textures.assign(TextureOrder::Count, nullptr);
}

void JMaterialDX11::UpdateBuffer(UINT currentFrameIndex)
{
	auto* context = JD3D11GlobalFactor::GetInstance()->GetDeviceContext();
	if (not m_MaterialConstantBuffer[currentFrameIndex]) {
		auto* device = JD3D11GlobalFactor::GetInstance()->GetDevice();


		// Usage, BindFlag 변화 고려
		D3D11_BUFFER_DESC desc{
		.ByteWidth = Align(sizeof(m_SimpleMaterial), 16),
		.Usage = D3D11_USAGE_DYNAMIC,
		.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
		.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
		};

		ThrowIfFailed(device->CreateBuffer(&desc, nullptr, m_MaterialConstantBuffer[currentFrameIndex].GetAddressOf()));
	}

	D3D11_MAPPED_SUBRESOURCE data{};
	context->Map(m_MaterialConstantBuffer[currentFrameIndex].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	memcpy(data.pData, &m_SimpleMaterial, sizeof(m_SimpleMaterial));
	context->Unmap(m_MaterialConstantBuffer[currentFrameIndex].Get(), 0);
}

// 이거도 확장할 수 있을 것 이대로는 하나만 넣을 수 있음
void JMaterialDX11::SetDXBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage)
{
	auto* gFactor = JD3D11GlobalFactor::GetInstance();
	auto* context = gFactor->GetDeviceContext();
	auto& ci = currentFrameIndex;


	ID3D11ShaderResourceView* srvs[TextureOrder::Count]{};
	for (UINT i = 0; i < m_Textures.size(); ++i)
		srvs[i] = m_Textures[i]->GetSRV();

	switch (stage) {
	case JS_VS:
		context->VSSetConstantBuffers(parameter, 1, m_MaterialConstantBuffer[ci].GetAddressOf());
		context->VSSetShaderResources(2, TextureOrder::Count, srvs);
		break;
	case JS_PS:
		context->PSSetConstantBuffers(parameter, 1, m_MaterialConstantBuffer[ci].GetAddressOf());
		context->PSSetShaderResources(2, TextureOrder::Count, srvs);
		break;
	case JS_GS:
		context->GSSetConstantBuffers(parameter, 1, m_MaterialConstantBuffer[ci].GetAddressOf());
		context->GSSetShaderResources(2, TextureOrder::Count, srvs);
		break;
	case JS_HS:
		context->HSSetConstantBuffers(parameter, 1, m_MaterialConstantBuffer[ci].GetAddressOf());
		context->HSSetShaderResources(2, TextureOrder::Count, srvs);
		break;
	case JS_DS:
		context->DSSetConstantBuffers(parameter, 1, m_MaterialConstantBuffer[ci].GetAddressOf());
		context->DSSetShaderResources(2, TextureOrder::Count, srvs);
		break;
	case JS_CS:
		context->CSSetConstantBuffers(parameter, 1, m_MaterialConstantBuffer[ci].GetAddressOf());
		context->CSSetShaderResources(2, TextureOrder::Count, srvs);
		break;
	default:
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("[DX11] {0}에서 잘못된 SetDXBuffer를 호출했습니다.", m_name.c_str());
#endif
		assert(0);
	}
}

void JMaterialDX11::SetTextureNull(std::shared_ptr<JContent>& nulltexture)
{
	auto texDx11 = std::dynamic_pointer_cast<JTextureDX11>(nulltexture);
	if (!texDx11) {
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("{0} material에서 잘못된 texture를 이용한 SetTextureNull을 호출했습니다.", m_name.c_str());
#endif
		return;
	}

	for (auto& t : m_Textures)
		t = texDx11;
}

void JMaterialDX11::SetTexture(std::shared_ptr<JContent>& texture, TextureOrder order)
{
	auto texDx11 = std::dynamic_pointer_cast<JTextureDX11>(texture);
	if (!texDx11) {
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("{0} material에서 잘못된 texture를 이용한 SetTexture를 호출했습니다.", m_name.c_str());
#endif
		return;
	}

	switch (order) {
	case Albedo:
		m_Textures[order] = texDx11;
		m_SimpleMaterial.AlbedoTexIndex = 1;
		break;
	case Normal:
		m_Textures[order] = texDx11;
		//m_SimpleMaterial.Normal = 1;
		break;
	default:
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("{0} material에서 SetTexture에 잘못된 order를 설정했습니다.", m_name.c_str());
#endif
		break;
	}
}
