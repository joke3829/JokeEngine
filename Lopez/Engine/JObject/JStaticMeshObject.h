#pragma once

#include "JObject.h"

class JContent;
class JStaticMesh;
class JMaterial;
class JMeshConstant;

class JStaticMeshObject : public JObject {
public:
	JStaticMeshObject(std::vector<XMFLOAT4X4>& vWorld, UINT nodeIndex, const char* name = nullptr);
	~JStaticMeshObject();

	void Render();

public:
	void AddMaterial(std::shared_ptr<JMaterial> material, int index = -1);

	void SetStaticMesh(std::shared_ptr<JContent> mesh);
protected:
	// 메시 상수 버퍼가 필요한데
	std::unique_ptr<JMeshConstant>					m_MeshCB{};

	std::vector<std::shared_ptr<JMaterial>>			m_Materials{};

	std::shared_ptr<JStaticMesh>					m_StaticMesh{};
};