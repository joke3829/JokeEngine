#pragma once
#include "Engine/JObject/JObject.h"
#include "Engine/JInputThread.h"

class JContentManager;
class JCameraObject;
class JPlayerObject;

// Renderer가 최신화 할 때 체크하는 정보, object == nullptr일 시 제거 요청
struct CDObjectInfo {	// CD: Create Delete
	std::string shadername{};
	UINT uid{};				// 제거할 uid
	JObject* object = nullptr;
};

class JEngineScene : public JCommon {
public:
	// scene 파일 읽어 Scene 구성하는 함수
	JEngineScene();

	void ClearScene();

	JObject* CreateObject(const char* ObjectType);
	void DeleteObject(UINT uid);

	void CreateSceneFromFile() {}

	virtual void SetGPUBuffer(UINT currentBufferIndex, UINT parameter, JShaderStage stage) {}

	virtual void Update(float elapsedTime);
	virtual void UpdateBuffers(UINT currentBufferIndex) {}

	virtual void Reset();
	
	// 동기 마우스 메시지 처리
	virtual void WindowMessageProcessor(UINT msg, WPARAM wParam, LPARAM lParam);

	// Scene 기능을 사용하기 전 반드시 설정을 해줘야한다.
	void SetInputThread(std::shared_ptr<JInputThread>& input) { m_InputThread = input; }

	// Scene 내 ObjectList 요청
	std::unordered_map<UINT, std::unique_ptr<JObject>>& GetObjects() { return m_Objects; }
	std::queue<CDObjectInfo>& GetCDQueue() { return m_CDQueue; }

	bool GetDirty() { return m_bDirty; }
	void SetDirty(bool dirty = true) { m_bDirty = dirty; }

#if DEV_MODE
public:
	void BuildDefaultEditorScene();
	void SetPlayMode(bool pmode);
	void SetPause(bool pause) { m_Pause = pause; }
	void SetControlEditorScene(bool ces) { m_ControlEditorScene = ces; }
	void EditorCameraResize(float width, float height);
	JCameraObject* GetEditorCamera() { return m_EditorCamera; }
	JCameraObject* GetCurrentMainCamera() { return m_Cameras[m_CurrentMainCamera]; }
protected:
	bool m_PlayMode{};
	bool m_Pause{};
	bool m_ControlEditorScene{};
	JPlayerObject* m_EditorPlayer{};
	JCameraObject* m_EditorCamera{};
#endif
protected:
	// first: uid -> m_WorldMatrices.size()
	std::unordered_map<UINT, std::unique_ptr<JObject>>			m_Objects;
	std::vector<JCameraObject*>						m_Cameras{};
	std::vector<JPlayerObject*>						m_Players{};
	// player(character) 목록 추기
	int												m_CurrentPlayer{ -1 };	// -1일땐 Scene에 Player가 없는걸로
	int												m_CurrentMainCamera{ -1 };	// 상수버퍼에 넘길 카메라 인덱스(사용할 것)
	int												m_InitialCameraIdx{};	// Scene이 시작할 때 사용할 카메라 인덱스


	std::vector<XMFLOAT4X4>						m_WorldMatrices{};
	std::vector<XMFLOAT4X4>						m_WorldMatricesTP{};	// Transpose

	std::vector<XMFLOAT4X4>						m_CameraMatrices{};
	std::vector<XMFLOAT4X4>						m_CameraMatricesTP{};	// Transpose

	std::shared_ptr<JInputThread>				m_InputThread{};

	bool										m_bDirty{};
	std::queue<CDObjectInfo>					m_CDQueue{};
};