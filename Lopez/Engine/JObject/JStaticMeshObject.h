#pragma once

#include "JObject.h"

class JStaticMesh;
class JMaterial;

class JStaticMeshObject : public JObject {
public:
	JStaticMeshObject(XMFLOAT4X4* WorldMatrixByScene, const char* name = nullptr);

	void Render();

public:
	void AddMaterial(std::shared_ptr<JMaterial> material, int index = -1);

	void SetStaticMesh(std::shared_ptr<JStaticMesh> mesh) { m_StaticMesh = mesh; }
protected:
	// 메시 상수 버퍼가 필요한데


	std::vector<std::shared_ptr<JMaterial>>			m_Materials{};

	std::shared_ptr<JStaticMesh>					m_StaticMesh{};
};