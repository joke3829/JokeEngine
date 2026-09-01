#pragma once

#include "Engine/GResource/JSpriteMeshConstant.h"

struct JSAnimData;

class JSpriteMeshConstantDX11 : public JSpriteMeshConstant {
public:
	JSpriteMeshConstantDX11();

	void Update(float elapsedTime) {}
	void UpdateBuffer(UINT currentFrameIndex);
	void SetDXBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage);

protected:
	void BufferReady();

	ComPtr<ID3D11Buffer>		m_CBBuffer[g_NumRenderTarget]{};
};