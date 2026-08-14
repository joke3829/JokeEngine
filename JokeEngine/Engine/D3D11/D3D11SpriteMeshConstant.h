#pragma once

#include "Engine/JSpriteMeshConstant.h"

struct JSAnimData;

class JSpriteMeshConstantDX11 : public JSpriteMeshConstant {
public:
	JSpriteMeshConstantDX11();

	void Update(float elapsedTime) {}
	void UpdateFromData(const JSAnimData& data, bool mirror = false);
	void UpdateBuffer(UINT currentFrameIndex);
	void SetDXBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage);

protected:
	void BufferReady();

	ComPtr<ID3D11Buffer>		m_CBBuffer[g_NumRenderTarget]{};
};