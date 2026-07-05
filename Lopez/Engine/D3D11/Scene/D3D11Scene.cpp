#include "D3D11Scene.h"
#include "Engine/D3D11/D3D11GlobalFactor.h"

void JEngineSceneDX11::SetDXBuffer(UINT parameter, JShaderStage stage)
{
	auto* gFactor = JD3D11GlobalFactor::GetInstance();
	auto* context = gFactor->GetDeviceContext();

	ID3D11ShaderResourceView* srvs[] = { m_WorldMatricesBufferSRV.Get(), m_CameraMatricesBufferSRV.Get() };

	switch (stage) {
	case JS_VS:
		context->VSSetShaderResources(parameter, 2, srvs);
		break;
	case JS_PS:
		context->PSSetShaderResources(parameter, 2, srvs);
		break;
	case JS_GS:
		context->GSSetShaderResources(parameter, 2, srvs);
		break;
	case JS_HS:
		context->HSSetShaderResources(parameter, 2, srvs);
		break;
	case JS_DS:
		context->DSSetShaderResources(parameter, 2, srvs);
		break;
	case JS_CS:
		context->CSSetShaderResources(parameter, 2, srvs);
		break;
	default:
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("[DX11] {0}에서 잘못된 SetDXBuffer를 호출했습니다.", m_name.c_str());
#endif
		assert(0);
	}
}

void JEngineSceneDX11::UpdateBuffers()
{

}

// ============================================================

void JEngineDefaultSceneDX11::Initialize()
{
	BuildDefaultScene();
}

void JEngineDefaultSceneDX11::SetDXBuffer(UINT parameter, JShaderStage stage)
{
	JEngineSceneDX11::SetDXBuffer(parameter, stage);

	auto* gFactor = JD3D11GlobalFactor::GetInstance();
	auto* context = gFactor->GetDeviceContext();

	switch (stage) {
	case JS_VS:
		context->VSSetConstantBuffers(parameter, 1, m_SceneConstantBuffer.GetAddressOf());
		break;
	case JS_PS:
		context->PSSetConstantBuffers(parameter, 1, m_SceneConstantBuffer.GetAddressOf());
		break;
	case JS_GS:
		context->GSSetConstantBuffers(parameter, 1, m_SceneConstantBuffer.GetAddressOf());
		break;
	case JS_HS:
		context->HSSetConstantBuffers(parameter, 1, m_SceneConstantBuffer.GetAddressOf());
		break;
	case JS_DS:
		context->DSSetConstantBuffers(parameter, 1, m_SceneConstantBuffer.GetAddressOf());
		break;
	case JS_CS:
		context->CSSetConstantBuffers(parameter, 1, m_SceneConstantBuffer.GetAddressOf());
		break;
	default:
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("[DX11] {0}에서 잘못된 SetDXBuffer를 호출했습니다.", m_name.c_str());
#endif
		assert(0);
	}
}

void JEngineDefaultSceneDX11::UpdateBuffers()
{

}