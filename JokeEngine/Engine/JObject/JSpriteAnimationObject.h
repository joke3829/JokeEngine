#pragma once

#include "JStaticMeshObject.h"

class JSpriteAnimationSet;
class JSpriteMeshConstant;

// Mesh는 반드시 Plane을 줘야 함
class JSpriteAnimationObject : public JStaticMeshObject {
public:
	JSpriteAnimationObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name = nullptr);
	~JSpriteAnimationObject();

	void Update(float elapsedTime, XMFLOAT4X4* parent = nullptr);
	
	void SetAnimationSet(const char* setName, float minustime = 0.f);

	void Render(UINT currentFrameIndex);

public:
	void SetSpriteSet(std::shared_ptr<JContent> spriteSet);
protected:
	bool									m_bMirror{};

	std::pair<std::string, UINT>			m_CurrentAnimation{};

	float									m_ElapsedTime{};

	std::unique_ptr<JSpriteMeshConstant>	m_SpriteMeshCB{};
	std::shared_ptr<JSpriteAnimationSet>	m_SpriteSet{};
};