#include "D3D11MeshConstant.h"
#include "D3D11GlobalFactor.h"

JMeshConstantDX11::JMeshConstantDX11()
	: JMeshConstant()
{
	BufferReady();
}

JMeshConstantDX11::JMeshConstantDX11(CB_Mesh cb)
	: JMeshConstant(cb)
{
	BufferReady();
}

void JMeshConstantDX11::Update()
{
	if (m_Dirty) {
		auto* context = JD3D11GlobalFactor::GetInstance()->GetDeviceContext();
		D3D11_MAPPED_SUBRESOURCE mapped{};
		context->Map(m_CBBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		memcpy(mapped.pData, &m_CBMesh, sizeof(CB_Mesh));
		context->Unmap(m_CBBuffer.Get(), 0);
	}
}

void JMeshConstantDX11::SetDXBuffer(UINT parameter, JShaderStage stage)
{
	auto* context = JD3D11GlobalFactor::GetInstance()->GetDeviceContext();

	switch (stage) {
	case JS_VS:
		context->VSSetConstantBuffers(parameter, 1, m_CBBuffer.GetAddressOf());
		break;
	case JS_PS:
		context->PSSetConstantBuffers(parameter, 1, m_CBBuffer.GetAddressOf());
		break;
	case JS_GS:
		context->GSSetConstantBuffers(parameter, 1, m_CBBuffer.GetAddressOf());
		break;
	case JS_HS:
		context->HSSetConstantBuffers(parameter, 1, m_CBBuffer.GetAddressOf());
		break;
	case JS_DS:
		context->DSSetConstantBuffers(parameter, 1, m_CBBuffer.GetAddressOf());
		break;
	case JS_CS:
		context->CSSetConstantBuffers(parameter, 1, m_CBBuffer.GetAddressOf());
		break;
	default:
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("[DX11] MeshConstant 에서 잘못된 SetDXBuffer를 호출했습니다.");
#endif
		assert(0);
	}
}

void JMeshConstantDX11::BufferReady()
{
	auto* device = JD3D11GlobalFactor::GetInstance()->GetDevice();

	D3D11_BUFFER_DESC desc{
		.ByteWidth = Align(sizeof(CB_Mesh), 16),
		.Usage = D3D11_USAGE_DYNAMIC,
		.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
		.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
	};

	ThrowIfFailed(device->CreateBuffer(&desc, nullptr, m_CBBuffer.ReleaseAndGetAddressOf()));
	m_Dirty = true;
}
