#pragma once
#include "stdafx.h"

class JObject {
public:
	JObject(XMFLOAT4X4* WorldMatrixByScene, const char* name = nullptr);
	virtual ~JObject() {}

	void Attach(std::shared_ptr<JObject> leaf) { m_LeafObjects.emplace_back(leaf); }

	virtual void Update(float elapsedTime, XMFLOAT4X4* parent = nullptr);

	virtual void Render() {}
	virtual void Render(void* command) {}	// command -> 11: DeviceContext, 12: CommandList

	void SetName(const char* name) { m_name = name; }
	const std::string& GetName() { return m_name; }

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

protected:
	void MakeLocalTransform();

protected:
	bool										m_Active{ true };		// 렌더 여부, 카메라 사용 여부 등, 오브젝트 별로 기능 
	std::string									m_name{};
	std::vector<std::shared_ptr<JObject>>		m_LeafObjects{};

	XMFLOAT3									m_Position{};
	XMFLOAT3									m_Rotation{};				// Yaw, Pitch, Roll
	XMFLOAT3									m_Scale{ 1.f, 1.f ,1.f };

	XMFLOAT4X4									m_LocalTransform{};
	XMFLOAT4X4*									m_WorldTransform{};	// 원본은 Scene이 관리함(Bindless를 위한 설정)	반드시 오브젝트 추가할 때 Scene에게서 받아야한다.

	std::string									m_ShaderName{};	// 이 오브젝트를 렌더링 할 때 사용할 Shader(매핑 할 때 사용)
};