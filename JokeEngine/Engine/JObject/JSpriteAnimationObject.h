#pragma once

#include "JStaticMeshObject.h"

class JSpriteAnimationSet;
class JSpriteMeshConstant;

// Mesh는 반드시 Plane을 줘야 함
class JSpriteAnimationObject : public JStaticMeshObject {
public:
	JSpriteAnimationObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT uid, const char* name = nullptr);
	~JSpriteAnimationObject();

	virtual void Initialize();
	void Reset();

	void Update(float elapsedTime, XMFLOAT4X4* parent = nullptr);
	
	void SetAnimationSet(const char* setName, float elapsedTime = 0.f, float minustime = 0.f, bool edit = false);
	void SetAnimationSet(std::string& setName, float elapsedTime = 0.f, float minustime = 0.f, bool edit = false);

	void Render(UINT currentFrameIndex);

public:
	void SetSpriteSet(JContent* spriteSet);
#if DEV_MODE
public:
	virtual void SaveObjectToFile(nlohmann::json& json, std::unordered_map<UINT, UINT>& IDtable);
	virtual bool RenderInspector(UINT currentFrameIndex = 0);
#endif
protected:
	bool									m_bMirror{};
	bool									m_bInitialMirror{};

	std::pair<std::string, UINT>			m_CurrentAnimation{};
	std::string								m_InitialAnimation{};

	float									m_ElapsedTime{};
	float									m_InitialElapsedTime{};

	std::unique_ptr<JSpriteMeshConstant>	m_SpriteMeshCB{};
	JSpriteAnimationSet*					m_SpriteSet{};
};