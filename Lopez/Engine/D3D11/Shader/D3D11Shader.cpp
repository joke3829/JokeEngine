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

