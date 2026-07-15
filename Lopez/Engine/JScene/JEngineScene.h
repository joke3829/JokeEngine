#pragma once
#include "stdafx.h"

class JObject;
class JContentManager;


// 렌더 타겟 수 - DX12의 경우 fence를 2개를 이용해 렌더링 시간 손해 없---
constexpr UINT									g_NumRenderTarget = 2;

class JEngineScene : public JCommon {
public:
	virtual void SetDXBuffer(UINT parameter, JShaderStage stage, UINT currentBufferIndex) {}
	virtual void UpdateBuffers(UINT currentBufferIndex) {}
	

	// Scene 기능을 사용하기 전 반드시 설정을 해줘야한다.
	void SetContentManager(std::shared_ptr<JContentManager>& manager) { m_ContentManager = manager; }

	// DefaultScene - 불러올 Scene 파일이 없으면 이걸 불러라
	void BuildDefaultScene();

	const std::vector<std::shared_ptr<JObject>>& GetObjects() { return m_Objects; }

protected:


	std::vector<std::shared_ptr<JObject>>		m_Objects;

	std::vector<XMFLOAT4X4>						m_WorldMatrices{};
	// 카메라의 번호(m_WorldMatrices에서 어떤게 카메라인지 
	std::vector<UINT>							m_CameraIndex{};
	
	// 버퍼들 (SRV로 사용)
	// 카메라 버퍼들 (마찬가지 SRV)

	std::shared_ptr<JContentManager>			m_ContentManager{};
};