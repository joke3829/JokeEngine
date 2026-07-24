#include "D3D11Shader.h"
#include "Engine/D3D11/D3D11GlobalFactor.h"

void JEngineShaderDX11::SetShader()
{
	auto* context = JD3D11GlobalFactor::GetInstance()->GetDeviceContext();
	if (m_VS) context->VSSetShader(m_VS.Get(), nullptr, 0); else context->VSSetShader(nullptr, nullptr, 0);
	if (m_HS) context->HSSetShader(m_HS.Get(), nullptr, 0); else context->HSSetShader(nullptr, nullptr, 0);
	if (m_DS) context->DSSetShader(m_DS.Get(), nullptr, 0); else context->DSSetShader(nullptr, nullptr, 0);
	if (m_GS) context->GSSetShader(m_GS.Get(), nullptr, 0); else context->GSSetShader(nullptr, nullptr, 0);
	if (m_PS) context->PSSetShader(m_PS.Get(), nullptr, 0); else context->PSSetShader(nullptr, nullptr, 0);
	if (m_CS) context->CSSetShader(m_CS.Get(), nullptr, 0); else context->CSSetShader(nullptr, nullptr, 0);

	if (m_InputLayout) context->IASetInputLayout(m_InputLayout.Get()); else context->IASetInputLayout(nullptr);
	if (m_BlendState) context->OMSetBlendState(m_BlendState.Get(), nullptr, 0xffffffff); else context->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	if (m_DepthStencilState) context->OMSetDepthStencilState(m_DepthStencilState.Get(), 256); else context->OMSetDepthStencilState(nullptr, 0);
	if (m_RasterizerState) context->RSSetState(m_RasterizerState.Get()); else context->RSSetState(nullptr);
}

void JEngineShaderDX11::SetSamplers(UINT parameter, JShaderStage stage)
{
	if (0 == m_Samplers.size()) {
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("{0} Shader에서 잘못된 SetSamplaers를 호출했습니다. - Samplers.size() == 0", m_name.c_str());
#endif
		return;
	}
	auto* context = JD3D11GlobalFactor::GetInstance()->GetDeviceContext();

	switch (stage) {
	case JS_VS:
		context->VSSetSamplers(parameter, m_Samplers.size(), m_Samplers.data());
		break;
	case JS_PS:
		context->PSSetSamplers(parameter, m_Samplers.size(), m_Samplers.data());
		break;
	case JS_GS:
		context->GSSetSamplers(parameter, m_Samplers.size(), m_Samplers.data());
		break;
	case JS_HS:
		context->HSSetSamplers(parameter, m_Samplers.size(), m_Samplers.data());
		break;
	case JS_DS:
		context->DSSetSamplers(parameter, m_Samplers.size(), m_Samplers.data());
		break;
	case JS_CS:
		context->CSSetSamplers(parameter, m_Samplers.size(), m_Samplers.data());
		break;
	default:
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("[DX11] {0}에서 잘못된 SetSampler를 호출했습니다.", m_name.c_str());
#endif
		assert(0);
	}
}


void JEngineShaderDX11::RenderObjects(UINT currentFrameIndex, void** rtv, UINT numRTV, void* dsv)
{
	JEngineShaderDX11::SetShader();

	auto* context = JD3D11GlobalFactor::GetInstance()->GetDeviceContext();
	ID3D11RenderTargetView** drtv = reinterpret_cast<ID3D11RenderTargetView**>(rtv);
	ID3D11DepthStencilView* ddsv = reinterpret_cast<ID3D11DepthStencilView*>(dsv);
	
	// 렌더타겟 Set
	context->OMSetRenderTargets(numRTV, drtv, ddsv);

	for (auto& object : m_Objects)
		object->Render(currentFrameIndex);
}

void JEngineShaderDX11::CreateInputLayout(bool skinning)
{
	auto* device = JD3D11GlobalFactor::GetInstance()->GetDevice();

	ComPtr<ID3DBlob> vs{};


	if (skinning) {
		
	}
	else {
		vs = CompileHLSL(L"Shaders/Default/DefaultShader.hlsl", nullptr, "DefaultVS", "vs_5_0");

		D3D11_INPUT_ELEMENT_DESC desc[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 5, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 1, DXGI_FORMAT_R32G32B32_FLOAT, 6, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		ThrowIfFailed(device->CreateInputLayout(desc, std::size(desc), vs->GetBufferPointer(), vs->GetBufferSize(), m_InputLayout.ReleaseAndGetAddressOf()));
	}
}

// ================================================================================

JEngineDefaultShaderDX11::JEngineDefaultShaderDX11(const char* name)
	: JEngineShaderDX11(name)
{
	auto* device = JD3D11GlobalFactor::GetInstance()->GetDevice();
	// InputLayout
	CreateInputLayout(false);

	// VS
	{
		ComPtr<ID3DBlob> vs{};
		vs = CompileHLSL(L"Shaders/Default/DefaultShader.hlsl", nullptr, "DefaultVS", "vs_5_0");

		ThrowIfFailed(device->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), nullptr, m_VS.ReleaseAndGetAddressOf()));
	}

	// PS
	{
		ComPtr<ID3DBlob> ps{};
		ps = CompileHLSL(L"Shaders/Default/DefaultShader.hlsl", nullptr, "DefaultPS", "ps_5_0");

		ThrowIfFailed(device->CreatePixelShader(ps->GetBufferPointer(), ps->GetBufferSize(), nullptr, m_PS.ReleaseAndGetAddressOf()));
	}

	//{
	//	D3D11_RASTERIZER_DESC desc{};
	//	desc.FillMode = D3D11_FILL_SOLID;
	//	desc.CullMode = D3D11_CULL_NONE;
	//	desc.FrontCounterClockwise = FALSE;
	//	device->CreateRasterizerState(&desc, m_RasterizerState.GetAddressOf());
	//}
}
