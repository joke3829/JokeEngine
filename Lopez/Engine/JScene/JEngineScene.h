#pragma once
#include "Engine/JObject/JCameraObject.h"

// 렌더 타겟 수 - DX12의 경우 fence를 2개를 이용해 렌더링 시간 손해 없---
constexpr UINT									g_NumRenderTarget = 2;

class JEngineScene : public JCommon {
public:
	virtual void Initialize() {}

	virtual void SetDXBuffer(UINT parameter, JShaderStage stage, UINT currentBufferIndex) {}

	virtual void UpdateBuffers(UINT currentBufferIndex) {}
	

	const std::vector<std::shared_ptr<JObject>>& GetObjects() { return m_Objects; }

protected:

	void BuildDefaultScene();

	std::vector<std::shared_ptr<JObject>>		m_Objects;

	std::vector<XMFLOAT4X4>						m_WorldMatrices{};
	// 카메라의 번호(m_WorldMatrices에서 어떤게 카메라인지 
	std::vector<UINT>							m_CameraIndex{};
	
	// 버퍼들 (SRV로 사용)
	// 카메라 버퍼들 (마찬가지 SRV)
};