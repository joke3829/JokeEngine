#include "JEngineRenderer.h"



JEngineRenderer::JEngineRenderer(UINT width, UINT height, const char* name)
	: JCommon(name)
{
	CreateRTV_DSV(width, height);
}

void JEngineRenderer::ResizeTarget(UINT width, UINT height)
{
	CreateRTV_DSV(width, height);
}

void JEngineRenderer::AdvanceBufferIndex()
{
	m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % g_NumRenderTarget;
}