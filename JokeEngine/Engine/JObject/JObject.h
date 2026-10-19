#pragma once
#include "Engine/JCommon.h"

class JEngineShader;

class JObject : public JCommon {
public:
	JObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name = nullptr);
	virtual ~JObject() {}

	virtual void Reset();

	void Attach(std::shared_ptr<JObject> leaf) { m_LeafObjects.emplace_back(leaf); }

	virtual void Update(float elapsedTime, XMFLOAT4X4* parent = nullptr);

	virtual void Render(UINT currentFrameIndex) {};

	// 렌더링이 필요하면 반드시 셰이더 이름을 작성
	void SetShaderName(const char* name) { m_ShaderName = name; }
	const std::string& GetShaderName() { return m_ShaderName; }
	
	void SetActiveState(bool active, bool edit = false) { m_Active = active; if (edit) m_InitialActive = active; }
	bool GetActiveState() { return m_Active; }

	// bool edit가 true일 땐 Initial 정보도 함께 변경(EditorMod일 때만 사용)
	void SetPosition(float x, float y, float z, bool edit = false) { m_Position = XMFLOAT3(x, y, z); if (edit) m_InitialPosition = m_Position; }
	void SetPosition(XMFLOAT3 position, bool edit = false) { m_Position = position; }
	const XMFLOAT3& GetPosition() { return m_Position; }

	void SetRotation(float x, float y, float z, bool edit = false) { m_Rotation = XMFLOAT3(x, y, z); if (edit) m_InitialRotation = m_Rotation; }
	void SetRotation(XMFLOAT3 rotate, bool edit = false) { m_Rotation = rotate; if (edit) m_InitialRotation = rotate; }
	const XMFLOAT3& GetRotation() { return m_Rotation; }

	void SetScale(float x, float y, float z, bool edit = false) { m_Scale = XMFLOAT3(x, y, z); if (edit) m_InitialScale = m_Scale; }
	void SetScale(XMFLOAT3 scale, bool edit = false) { m_Scale = scale; if (edit) m_InitialScale = m_Scale; }
	const XMFLOAT3& GetScale() { return m_Scale; }

	std::vector<std::shared_ptr<JObject>>& GetLeafObjects() { return m_LeafObjects; }

protected:
	void MakeLocalTransform();

protected:
	bool										m_Active{ true };		// 렌더 여부, 카메라 사용 여부 등, 오브젝트 별로 기능 
	bool										m_InitialActive{ true };
	std::vector<std::shared_ptr<JObject>>		m_LeafObjects{};

	XMFLOAT3									m_Position{};
	XMFLOAT3									m_Rotation{};				// Yaw, Pitch, Roll
	XMFLOAT3									m_Scale{ 1.f, 1.f ,1.f };

	// 초기 상태, Reset할 때 사용된다. editor에서 수정할 땐 함께 수정됨
	XMFLOAT3									m_InitialPosition{};
	XMFLOAT3									m_InitialRotation{};
	XMFLOAT3									m_InitialScale{ 1.f, 1.f, 1.f };

	XMFLOAT4X4									m_LocalTransform{};
	std::vector<XMFLOAT4X4>&					m_WorldTransform;				// 이것에[m_NodeIndex]가 m_WorldTransform
	std::vector<XMFLOAT4X4>&					m_WorldTransformTP;				// Transpose WorldMatrix
	UINT										m_NodeIndex{};					// node id로도 사용
	
	std::string									m_ShaderName{};	// 이 오브젝트를 렌더링 할 때 사용할 Shader(매핑 할 때 사용)

};