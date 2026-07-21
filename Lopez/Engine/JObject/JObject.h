#pragma once
#include "Engine/JCommon.h"

class JEngineShader;

class JObject : public JCommon {
public:
	JObject(std::vector<XMFLOAT4X4>& vWorld, UINT nodeIndex, const char* name = nullptr);
	virtual ~JObject() {}

	void Attach(std::shared_ptr<JObject> leaf) { m_LeafObjects.emplace_back(leaf); }

	virtual void Update(float elapsedTime, XMFLOAT4X4* parent = nullptr);

	virtual void Render(UINT currentFrameIndex) {};

	// 렌더링이 필요하면 반드시 셰이더 이름을 작성
	void SetShaderName(const char* name) { m_ShaderName = name; }
	const std::string& GetShaderName() { return m_ShaderName; }
	
	void SetActiveState(bool active) { m_Active = active; }
	bool GetActiveState() { return m_Active; }

	void SetPosition(float x, float y, float z) { m_Position = XMFLOAT3(x, y, z); }
	void SetPosition(XMFLOAT3 position) { m_Position = position; }
	const XMFLOAT3& GetPosition() { return m_Position; }

	void SetRotation(float x, float y, float z) { m_Rotation = XMFLOAT3(x, y, z); }
	void SetRotation(XMFLOAT3 rotate) { m_Rotation = rotate; }
	const XMFLOAT3& GetRotation() { return m_Rotation; }

	void SetScale(float x, float y, float z) { m_Scale = XMFLOAT3(x, y, z); }
	void SetScale(XMFLOAT3 scale) { m_Scale = scale; }
	const XMFLOAT3& GetScale() { return m_Scale; }

	std::vector<std::shared_ptr<JObject>>& GetLeafObjects() { return m_LeafObjects; }

protected:
	void MakeLocalTransform();

protected:
	bool										m_Active{ true };		// 렌더 여부, 카메라 사용 여부 등, 오브젝트 별로 기능 
	std::vector<std::shared_ptr<JObject>>		m_LeafObjects{};

	XMFLOAT3									m_Position{};
	XMFLOAT3									m_Rotation{};				// Yaw, Pitch, Roll
	XMFLOAT3									m_Scale{ 1.f, 1.f ,1.f };

	XMFLOAT4X4									m_LocalTransform{};
	std::vector<XMFLOAT4X4>&					m_WorldTransform;				// 이것에[m_NodeIndex]가 m_WorldTransform
	UINT										m_NodeIndex{};
	
	std::string									m_ShaderName{};	// 이 오브젝트를 렌더링 할 때 사용할 Shader(매핑 할 때 사용)

};