#pragma once
#include "stdafx.h"
#include "JObject/JCameraObject.h"


class JEngineScene {
public:
	virtual void BuildDefaultScene() {}

	const std::vector<std::shared_ptr<JObject>>& GetObjects() { return m_Objects; }
	const std::vector<std::shared_ptr<JCameraObject>>& GetCameras() { return m_Cameras; }

	

protected:
	std::vector<std::shared_ptr<JObject>>		m_Objects;		// 
	std::vector<std::shared_ptr<JCameraObject>>	m_Cameras;		// Scene에 있는 카메라의 수

	// 버퍼들 (SRV로 사용)
	// 카메라 버퍼들 (마찬가지 SRV)
};