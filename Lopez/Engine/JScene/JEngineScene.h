#pragma once
#include "stdafx.h"
#include "Engine/JObject/JCameraObject.h"


class JEngineScene {
public:
	virtual void Initialize() {}

	virtual void SetDXBuffer(UINT parameter, JShaderStage stage) {}


	

	const std::vector<std::shared_ptr<JObject>>& GetObjects() { return m_Objects; }

protected:

	void BuildDefaultScene();


	std::string									m_name{};
	std::vector<std::shared_ptr<JObject>>		m_Objects;

	std::vector<XMFLOAT4X4>						m_WorldMatrices{};
	// 카메라의 번호(m_WorldMatrices에서 어떤게 카메라인지 
	std::vector<UINT>							m_CameraIndex{};
	
	// 버퍼들 (SRV로 사용)
	// 카메라 버퍼들 (마찬가지 SRV)
};