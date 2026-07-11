#pragma once

#include "Engine/JScene/JEngineScene.h"



class JEngineSceneDX11 : public JEngineScene {
public:
	virtual void Initialize() {}

	virtual void SetDXBuffer(UINT parameter, JShaderStage stage, UINT currentBufferIndex);
	virtual void UpdateBuffers(UINT currentBufferIndex);

protected:
	ComPtr<ID3D11Buffer>				m_WorldMatricesBuffer[g_NumRenderTarget]{};
	ComPtr<ID3D11ShaderResourceView>	m_WorldMatricesBufferSRV[g_NumRenderTarget]{};
	size_t								m_nAlignWorldMatrices[g_NumRenderTarget]{};

	ComPtr<ID3D11Buffer>				m_CameraMatricesBuffer[g_NumRenderTarget]{};
	ComPtr<ID3D11ShaderResourceView>	m_CameraMatricesBufferSRV[g_NumRenderTarget]{};
	std::vector<XMFLOAT4X4>				m_CameraMatrices{};
	size_t								m_nAlignCameraMatrices[g_NumRenderTarget]{};
};

// =============================================================================



class JEngineDefaultSceneDX11 : public JEngineSceneDX11 {
public:
	void Initialize();

	void SetDXBuffer(UINT parameter, JShaderStage stage, UINT currentBufferIndex);
	void UpdateBuffers(UINT currentBufferIndex);
protected:
	struct DefaultSceneConstant {
		UINT mainCameraIndex = 0;
	} m_SceneConstant;

	ComPtr<ID3D11Buffer>				m_SceneConstantBuffer[g_NumRenderTarget]{};
};