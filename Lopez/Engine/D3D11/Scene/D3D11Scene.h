#pragma once

#include "Engine/JScene/JEngineScene.h"



class JEngineSceneDX11 : public JEngineScene {
public:
	virtual void Initialize() {}

	virtual void SetDXBuffer(UINT parameter, JShaderStage stage);
	virtual void UpdateBuffers();

protected:
	ComPtr<ID3D11Buffer>				m_WorldMatricesBuffer{};
	ComPtr<ID3D11ShaderResourceView>	m_WorldMatricesBufferSRV{};
	size_t								m_nAlignWorldMatrices{};

	ComPtr<ID3D11Buffer>				m_CameraMatricesBuffer{};
	ComPtr<ID3D11ShaderResourceView>	m_CameraMatricesBufferSRV{};
	std::vector<XMFLOAT4X4>				m_CameraMatrices{};
	size_t								m_nAlignCameraMatrices{};
};

// =============================================================================



class JEngineDefaultSceneDX11 : public JEngineSceneDX11 {
public:
	void Initialize();

	void SetDXBuffer(UINT parameter, JShaderStage stage);
	void UpdateBuffers();
protected:
	struct DefaultSceneConstant {
		UINT mainCameraIndex = 0;
	} m_SceneConstant;

	ComPtr<ID3D11Buffer>				m_SceneConstantBuffer{};
};