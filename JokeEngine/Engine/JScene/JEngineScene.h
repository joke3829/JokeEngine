#pragma once
#include "Engine/JObject/JObject.h"
#include "Engine/JInputThread.h"

class JContentManager;
class JCameraObject;
class JPlayerObject;


class JEngineScene : public JCommon {
public:
	virtual void SetGPUBuffer(UINT currentBufferIndex, UINT parameter, JShaderStage stage) {}

	void Update(float elapsedTime);
	virtual void UpdateBuffers(UINT currentBufferIndex) {}
	
	// 동기 마우스 메시지 처리
	virtual void ProcessMouseMessage(JMouseState& state);

	// Scene 기능을 사용하기 전 반드시 설정을 해줘야한다.
	void SetContentManager(std::shared_ptr<JContentManager>& manager) { m_ContentManager = manager; }
	void SetInputThread(std::shared_ptr<JInputThread>& input) { m_InputThread = input; }

	// DefaultScene - 불러올 Scene 파일이 없으면 이걸 불러라
	void BuildDefaultScene();

	std::vector<std::shared_ptr<JObject>>& GetObjects() { return m_Objects; }

protected:

	std::vector<std::shared_ptr<JObject>>			m_Objects;
	std::vector<std::shared_ptr<JCameraObject>>		m_Cameras{};
	std::vector<std::shared_ptr<JPlayerObject>>		m_Players{};
	// player(character) 목록 추기
	unsigned int									m_CurrentPlayer{};


	std::vector<XMFLOAT4X4>						m_WorldMatrices{};
	std::vector<XMFLOAT4X4>						m_WorldMatricesTP{};	// Transpose

	std::vector<XMFLOAT4X4>						m_CameraMatrices{};
	std::vector<XMFLOAT4X4>						m_CameraMatricesTP{};	// Transpose

	std::shared_ptr<JContentManager>			m_ContentManager{};
	std::shared_ptr<JInputThread>				m_InputThread{};
};