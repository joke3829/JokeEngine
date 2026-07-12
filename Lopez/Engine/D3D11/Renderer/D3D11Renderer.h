#pragma once


#include "Engine/JRenderer/JEngineRenderer.h"

class JEngineRendererDX11 : public JEngineRenderer {
public:

	void ResizeTarget(UINT width, UINT height);

	void RenderFrame();

	void CopyResult(void* outBuffer);

	void AdvanceBufferIndex();

protected:
	void CreateRTV_DSV(UINT width, UINT height);
protected:
	
	// Main RenderTarget, DepthStencil
	ComPtr<ID3D11Texture2D>			m_RenderTarget[g_NumRenderTarget]{};
	ComPtr<ID3D11RenderTargetView>	m_RenderTargetView[g_NumRenderTarget]{};

	ComPtr<ID3D11Texture2D>			m_DepthStencil[g_NumRenderTarget]{};
	ComPtr<ID3D11DepthStencilView>	m_DepthStencilView[g_NumRenderTarget]{};
};