#pragma once
#include "Engine/JCommon.h"

// ==========================================================================================
// JObject
// Scene에 포함 돼 자신만의 기능을 하는 요소
// 
// JObject는 기본적으로 기하 정보를 가지고 그 외의 역할은 없습니다.
// 추가 역할을 원하시면 JObject를 상속해 CustomObject를 만들어야합니다.
// 
// !!!상속 시 반드시 지킬것!!!
// 1. 기본생성자를 아래 JObject의 파라미터와 같게 작성(JObjectRegistry가 해당 양식을 사용)
// 2. Reset, RenderInspector override 필요 하지 않을 경우 JObject의 기본 요소만 사용됨
// 3. File 관련 저장법, 읽는법 작성 필요
// 
// .cpp에 #include "Engine/JObject/JObjectRegistry.h" 포함 + .cpp 끝에는 REGISTER_OBJECT()
// 매크로를 사용해야 정상적으로 Object가 등록되어 사용 가능합니다.
// ==========================================================================================

class JEngineShader;

class JObject : public JCommon {
public:
	JObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name = nullptr);
	virtual ~JObject() {}

	virtual void Reset();

	void Attach(std::shared_ptr<JObject> leaf);
	void Dettach(std::string& name);
	void Delete();

	virtual void Update(float elapsedTime, XMFLOAT4X4* parent = nullptr);

	virtual void Render(UINT currentFrameIndex) {};

	bool IsHasParent() { return m_ParentObject ? true : false; }
	void SetParent(JObject* parent) { m_ParentObject = parent; }

	// 렌더링이 필요하면 반드시 셰이더 이름을 작성
	void SetShaderName(const char* name) { m_ShaderName = name; }
	const std::string& GetShaderName() { return m_ShaderName; }
	
	void SetActiveState(bool active, bool edit = false) { m_Active = active; if (edit) m_InitialActive = active; }
	bool GetActiveState() { return m_Active; }

	// bool edit가 true일 땐 Initial 정보도 함께 변경(EditorMod일 때만 사용)
	void SetPosition(float x, float y, float z, bool edit = false) { m_Position = XMFLOAT3(x, y, z); if (edit) m_InitialPosition = m_Position; }
	void SetPosition(XMFLOAT3 position, bool edit = false) { m_Position = position; if (edit) m_InitialPosition = m_Position; }
	const XMFLOAT3& GetPosition() { return m_Position; }

	void SetRotation(float x, float y, float z, bool edit = false);
	void SetRotation(XMFLOAT3 rotate, bool edit = false);
	void SetRotation(float x, float y, float z, float w, bool edit = false);
	void SetRotation(XMFLOAT4 quarternion, bool edit = false);
	const XMFLOAT3& GetRotation() { return m_Rotation; }

	void SetScale(float x, float y, float z, bool edit = false) { m_Scale = XMFLOAT3(x, y, z); if (edit) m_InitialScale = m_Scale; }
	void SetScale(XMFLOAT3 scale, bool edit = false) { m_Scale = scale; if (edit) m_InitialScale = m_Scale; }
	const XMFLOAT3& GetScale() { return m_Scale; }

	// Update 이후에 사용해야 올바른 값이 나온다.
	const XMFLOAT4X4& GetWorldTransform() { return m_WorldTransform[m_NodeIndex]; }

	std::vector<std::shared_ptr<JObject>>& GetLeafObjects() { return m_LeafObjects; }

#if DEV_MODE
public:
	void SetPlayMode(bool mod);
	virtual void RenderInspector();
	const XMFLOAT4X4& GetParentWorldTransform() { return m_ParentWorldMatix; }
protected:
	XMFLOAT4X4 m_ParentWorldMatix{};
	bool m_PlayMode{ false };
#endif

protected:
	void MakeLocalTransform();

protected:
	bool										m_Active{ true };		// 렌더 여부, 카메라 사용 여부 등, 오브젝트 별로 기능 
	bool										m_InitialActive{ true };
	std::vector<std::shared_ptr<JObject>>		m_LeafObjects{};
	JObject*									m_ParentObject{};		// 트리에 사용, 부모 object의 pointer

	XMFLOAT3									m_Position{};
	XMFLOAT3									m_Rotation{};				// Yaw, Pitch, Roll
	XMFLOAT4									m_Quaternion{ 0.f, 0.f, 0.f, 1.f }; //xyz: vector, w: scalar
	XMFLOAT3									m_Scale{ 1.f, 1.f ,1.f };

	// 초기 상태, Reset할 때 사용된다. editor에서 수정할 땐 함께 수정됨
	XMFLOAT3									m_InitialPosition{};
	XMFLOAT3									m_InitialRotation{};
	XMFLOAT4									m_InitialQuaternion{ 0.f, 0.f, 0.f, 1.f };
	XMFLOAT3									m_InitialScale{ 1.f, 1.f, 1.f };

	XMFLOAT4X4									m_LocalTransform{};
	std::vector<XMFLOAT4X4>&					m_WorldTransform;				// 이것에[m_NodeIndex]가 m_WorldTransform
	std::vector<XMFLOAT4X4>&					m_WorldTransformTP;				// Transpose WorldMatrix
	UINT										m_NodeIndex{};					// node id로도 사용
	
	std::string									m_ShaderName{};	// 이 오브젝트를 렌더링 할 때 사용할 Shader(매핑 할 때 사용)

};