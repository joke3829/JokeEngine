#include "D3D11Material.h"
#include "../D3D11GlobalFactor.h"

JMaterialSimpleDX11::JMaterialSimpleDX11(const char* name)
	: JMaterialSimple(name)
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
	
	D3D11_SUBRESOURCE_DATA data{
		.pSysMem = &m_SimpleMaterial
	};

	ThrowIfFailed(device->CreateBuffer(&desc, &data, m_MaterialConstantBuffer.GetAddressOf()));
}

// 이거도 확장할 수 있을 것 이대로는 하나만 넣을 수 있음
void JMaterialSimpleDX11::SetDXBuffer(UINT parameter, JShaderStage stage)
{
	auto* gFactor = JD3D11GlobalFactor::GetInstance();
	auto* context = gFactor->GetDeviceContext();

	switch (stage) {
	case JS_VS:
		context->VSSetConstantBuffers(parameter, 1, m_MaterialConstantBuffer.GetAddressOf());
		break;
	case JS_PS:
		context->PSSetConstantBuffers(parameter, 1, m_MaterialConstantBuffer.GetAddressOf());
		break;
	case JS_GS:
		context->GSSetConstantBuffers(parameter, 1, m_MaterialConstantBuffer.GetAddressOf());
		break;
	case JS_HS:
		context->HSSetConstantBuffers(parameter, 1, m_MaterialConstantBuffer.GetAddressOf());
		break;
	case JS_DS:
		context->DSSetConstantBuffers(parameter, 1, m_MaterialConstantBuffer.GetAddressOf());
		break;
	case JS_CS:
		context->CSSetConstantBuffers(parameter, 1, m_MaterialConstantBuffer.GetAddressOf());
		break;
	default:
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("[DX11] {0}에서 잘못된 SetDXBuffer를 호출했습니다.", m_name.c_str());
#endif
		assert(0);
	}
}