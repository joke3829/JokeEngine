#pragma once


#include "Engine/JRenderer/JEngineRenderer.h"

class JEngineRendererDX11 : public JEngineRenderer {
public:
	JEngineRendererDX11(UINT width, UINT height, const char* name = "DefaultRenderer");

	void ResizeTarget(UINT width, UINT height);

	void RenderFrame();

	void CopyResult(void* outBuffer);

	void SetScene(std::shared_ptr<JEngineScene> scene);

	// 11은 Fence를 쓸 필요가 (있을 수 있지만)없기 때문에 AdvanceBufferIndex override X

protected:
	void CreateRTV_DSV(UINT width, UINT height);
protected:
	
	// Main RenderTarget, DepthStencil
	ComPtr<ID3D11Texture2D>			m_RenderTarget[g_NumRenderTarget]{};
	ComPtr<ID3D11RenderTargetView>	m_RenderTargetView[g_NumRenderTarget]{};

	ComPtr<ID3D11Texture2D>			m_DepthStencil[g_NumRenderTarget]{};
	ComPtr<ID3D11DepthStencilView>	m_DepthStencilView[g_NumRenderTarget]{};
};