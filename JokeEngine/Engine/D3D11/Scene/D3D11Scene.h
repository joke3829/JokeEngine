#pragma once

#include "Engine/JScene/JEngineScene.h"



class JEngineSceneDX11 : public JEngineScene {
public:
	virtual void SetGPUBuffer(UINT currentBufferIndex, UINT parameter, JShaderStage stage);
	virtual void UpdateBuffers(UINT currentBufferIndex);

protected:
	ComPtr<ID3D11Buffer>				m_WorldMatricesBuffer[g_NumRenderTarget]{};
	ComPtr<ID3D11ShaderResourceView>	m_WorldMatricesBufferSRV[g_NumRenderTarget]{};
	size_t								m_nAlignWorldMatrices[g_NumRenderTarget]{};

	ComPtr<ID3D11Buffer>				m_CameraMatricesBuffer[g_NumRenderTarget]{};
	ComPtr<ID3D11ShaderResourceView>	m_CameraMatricesBufferSRV[g_NumRenderTarget]{};
	size_t								m_nAlignCameraMatrices[g_NumRenderTarget]{};
};

// =============================================================================


// Scene을 개조 하고 싶다면 SceneConstant를 직접 선언 관리 하도록

class JEngineDefaultSceneDX11 : public JEngineSceneDX11 {
public:
	void SetGPUBuffer(UINT currentBufferIndex, UINT parameter, JShaderStage stage);
	void UpdateBuffers(UINT currentBufferIndex);
protected:
	struct DefaultSceneConstant {
		UINT mainCameraIndex = 0;
	} m_SceneConstant;

	ComPtr<ID3D11Buffer>				m_SceneConstantBuffer[g_NumRenderTarget]{};
};