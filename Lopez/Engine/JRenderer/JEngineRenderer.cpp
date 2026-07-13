#include "JEngineRenderer.h"

void JEngineRenderer::Initialize(UINT width, UINT height)
{
	CreateRTV_DSV(width, height);
}


void JEngineRenderer::ResizeTarget(UINT width, UINT height)
{
	CreateRTV_DSV(width, height);
}

void JEngineRenderer::AdvanceBufferIndex()
{
	m_CurrentBufferIndex = (m_CurrentBufferIndex + 1) % g_NumRenderTarget;
}