#include "JEngineRenderer.h"



JEngineRenderer::JEngineRenderer(UINT width, UINT height, const char* name)
	: JCommon(name)
{
	CreateRTV_DSV(width, height);
	m_TargetSize.x = width; m_TargetSize.y = height;
}

void JEngineRenderer::ResizeTarget(UINT width, UINT height)
{
	CreateRTV_DSV(width, height);
	m_TargetSize.x = width; m_TargetSize.y = height;
}

void JEngineRenderer::AdvanceBufferIndex()
{
	m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % kNumRenderTarget;
}