#pragma once

#include "Engine/JMeshConstant.h"

class JMeshConstantDX11 : public JMeshConstant {
public:
	JMeshConstantDX11();
	JMeshConstantDX11(CB_Mesh cb);

	void Update(float elapsedTime) {}
	void UpdateBuffer(UINT currentFrameIndex);
	void SetDXBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage);

protected:
	void BufferReady();

	ComPtr<ID3D11Buffer>		m_CBBuffer[g_NumRenderTarget]{};
};