#pragma once

#include "JObject.h"

class JContent;
class JStaticMesh;
class JMaterial;
class JMeshConstant;

class JStaticMeshObject : public JObject {
public:
	JStaticMeshObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT uid, const char* name = nullptr);
	virtual ~JStaticMeshObject();

	virtual void Update(float elapsedTime, XMFLOAT4X4* parent = nullptr);

	virtual void Render(UINT currentFrameIndex);

public:
	void AddMaterial(JContent* material = nullptr, int index = -1);
	void DeleteMaterial(int index = -1);
	void SetStaticMesh(JContent* mesh);

#if DEV_MODE
public:
	virtual bool RenderInspector();
#endif
protected:
	// 메시 상수 버퍼가 필요한데
	std::unique_ptr<JMeshConstant>					m_MeshCB{};

	std::vector<JMaterial*>			m_Materials{};

	JStaticMesh*					m_StaticMesh{};
};