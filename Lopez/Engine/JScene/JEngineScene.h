#pragma once
#include "Engine/JObject/JObject.h"

class JContentManager;
class JCameraObject;



class JEngineScene : public JCommon {
public:
	virtual void SetDXBuffer(UINT currentBufferIndex, UINT parameter, JShaderStage stage) {}

	void Update(float elapsedTime);
	virtual void UpdateBuffers(UINT currentBufferIndex) {}
	

	// Scene 기능을 사용하기 전 반드시 설정을 해줘야한다.
	void SetContentManager(std::shared_ptr<JContentManager>& manager) { m_ContentManager = manager; }

	// DefaultScene - 불러올 Scene 파일이 없으면 이걸 불러라
	void BuildDefaultScene();

	std::vector<std::shared_ptr<JObject>>& GetObjects() { return m_Objects; }

protected:


	std::vector<std::shared_ptr<JObject>>			m_Objects;
	std::vector<std::shared_ptr<JCameraObject>>		m_Cameras{};

	std::vector<XMFLOAT4X4>						m_WorldMatrices{};
	std::vector<XMFLOAT4X4>						m_CameraMatrices{};
	// 카메라의 번호(m_WorldMatrices에서 어떤게 카메라인지 

	
	// 버퍼들 (SRV로 사용)
	// 카메라 버퍼들 (마찬가지 SRV)

	std::shared_ptr<JContentManager>			m_ContentManager{};
};