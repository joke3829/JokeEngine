#include "JEngineRenderer.h"

void JEngineRenderer::Initialize(UINT width, UINT height)
{
	CreateRTV_DSV(width, height);
}


void JEngineRenderer::ResizeTarget(UINT width, UINT height)
{
	CreateRTV_DSV(width, height);
}