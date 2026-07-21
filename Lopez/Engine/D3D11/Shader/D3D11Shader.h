#pragma once

#include "Engine/JShader/JEngineShader.h"

// Base ========================================================

class JEngineShaderDX11 : public JEngineShader {
public:
	JEngineShaderDX11(const char* name = "DefaultShader") : JEngineShader(name) {}

	void SetShader();
	void SetSamplers(UINT parameter, JShaderStage stage = JS_NONE);

	void RenderObjects(UINT currentFrameIndex) {};
	void RenderObjects(UINT currentFrameIndex, void** rtv, UINT numRTV, void* dsv = nullptr);


protected:
	
	virtual void CreateInputLayout(bool skinning);		

protected:

	ComPtr<ID3D11VertexShader>				m_VS{};
	ComPtr<ID3D11HullShader>				m_HS{};
	ComPtr<ID3D11DomainShader>				m_DS{};
	ComPtr<ID3D11GeometryShader>			m_GS{};
	ComPtr<ID3D11PixelShader>				m_PS{};

	// 컴퓨트
	ComPtr<ID3D11ComputeShader>				m_CS{};


	ComPtr<ID3D11InputLayout>				m_InputLayout{};
	ComPtr<ID3D11BlendState>				m_BlendState{};
	ComPtr<ID3D11DepthStencilState>			m_DepthStencilState{};
	ComPtr<ID3D11RasterizerState>			m_RasterizerState{};
	std::vector<ComPtr<ID3D11SamplerState>>	m_SamplerStates{};		// PS가 요구하면 정의
	std::vector<ID3D11SamplerState*>		m_Samplers{};
};

// Default =============================================================


class JEngineDefaultShaderDX11 : public JEngineShaderDX11 {
public:
	JEngineDefaultShaderDX11(const char* name = "DefaultShader");
};