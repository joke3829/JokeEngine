#pragma once
#include "Engine/JObject/JObject.h"
#include "Engine/JInputThread.h"

class JContentManager;
class JCameraObject;
class JPlayerObject;


class JEngineScene : public JCommon {
public:
	// scene 파일 읽어 Scene 구성하는 함수
	JEngineScene();

	void ClearScene();

	std::shared_ptr<JObject> CreateObject(const char* ObjectType);
	void CreateSceneFromFile() {}

	virtual void SetGPUBuffer(UINT currentBufferIndex, UINT parameter, JShaderStage stage) {}

	virtual void Update(float elapsedTime);
	virtual void UpdateBuffers(UINT currentBufferIndex) {}

	virtual void Reset();
	
	// 동기 마우스 메시지 처리
	virtual void WindowMessageProcessor(UINT msg, WPARAM wParam, LPARAM lParam);

	// Scene 기능을 사용하기 전 반드시 설정을 해줘야한다.
	void SetContentManager(std::shared_ptr<JContentManager>& manager) { m_ContentManager = manager; }
	void SetInputThread(std::shared_ptr<JInputThread>& input) { m_InputThread = input; }

	// Scene 내 ObjectList 요청
	std::vector<std::shared_ptr<JObject>>& GetObjects() { return m_Objects; }

#if DEV_MODE
public:
	void BuildDefaultEditorScene();
	void SetPlayMode(bool pmode);
	void SetPause(bool pause) { m_Pause = pause; }
	void SetControlEditorScene(bool ces) { m_ControlEditorScene = ces; }
	void EditorCameraResize(float width, float height);
	JCameraObject* GetEditorCamera() { return m_EditorCamera.get(); }
	JCameraObject* GetCurrentMainCamera() { return m_Cameras[m_CurrnetMainCamera].get(); }
protected:
	bool m_PlayMode{};
	bool m_Pause{};
	bool m_ControlEditorScene{};
	std::shared_ptr<JPlayerObject> m_EditorPlayer{};
	std::shared_ptr<JCameraObject> m_EditorCamera{};
#endif
protected:
	std::vector<std::shared_ptr<JObject>>			m_Objects;
	std::vector<std::shared_ptr<JCameraObject>>		m_Cameras{};
	std::vector<std::shared_ptr<JPlayerObject>>		m_Players{};
	// player(character) 목록 추기
	int												m_CurrentPlayer{ -1 };	// -1일땐 Scene에 Player가 없는걸로
	int												m_CurrnetMainCamera{};	// 상수버퍼에 넘길 카메라 인덱스(사용할 것)
	int												m_InitialCameraIdx{};	// Scene이 시작할 때 사용할 카메라 인덱스


	std::vector<XMFLOAT4X4>						m_WorldMatrices{};
	std::vector<XMFLOAT4X4>						m_WorldMatricesTP{};	// Transpose

	std::vector<XMFLOAT4X4>						m_CameraMatrices{};
	std::vector<XMFLOAT4X4>						m_CameraMatricesTP{};	// Transpose

	std::shared_ptr<JContentManager>			m_ContentManager{};
	std::shared_ptr<JInputThread>				m_InputThread{};
};