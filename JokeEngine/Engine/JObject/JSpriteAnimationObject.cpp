#include "JSpriteAnimationObject.h"
#include "Engine/JokeEngineConfig.h"
#include "Engine/JContent/JMaterial.h"
#include "Engine/JContent/JStaticMesh.h"
#include "Engine/D3D11/GResource/D3D11MeshConstant.h"
#include "Engine/D3D11/GResource/D3D11SpriteMeshConstant.h"
#include "Engine/JContent/JSpriteAnimationSet.h"

JSpriteAnimationObject::JSpriteAnimationObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name)
	: JStaticMeshObject(vWorld, vWorldTP, nodeIndex, name)
{
	auto* opt = JEngineDefaultGlobalConfig::GetInstance()->GetConfigFactor();
	switch (opt->DirectX_Version) {
	case 11:
		m_SpriteMeshCB = std::make_unique<JSpriteMeshConstantDX11>();
		break;
	case 12:
		break;
	default:
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("{0} - JSpriteAnimationMeshObject가 잘못된 DirectX Version을 받았습니다.", m_name.c_str());
#endif
		assert(0);
		break;
	}
}

JSpriteAnimationObject::~JSpriteAnimationObject()
{
}

void JSpriteAnimationObject::Update(float elapsedTime, XMFLOAT4X4* parent)
{
	m_ElapsedTime += elapsedTime;
	
	if (m_SpriteSet) {
		const JSAnimData& data = m_SpriteSet->GetKeyFrameData(m_CurrentAnimation.first, m_ElapsedTime);
		m_CurrentAnimation.second = data.materialIndex;

		m_SpriteMeshCB->UpdateFromData(data, m_bMirror);
	}
	for (auto& p : m_Materials)
		p->Update(elapsedTime);
	
	JObject::Update(elapsedTime, parent);
}

void JSpriteAnimationObject::SetAnimationSet(const char* setName, float minustime)
{
	m_CurrentAnimation.first = setName;
	m_ElapsedTime = minustime;
}

void JSpriteAnimationObject::Render(UINT currentFrameIndex)
{
	if (!m_StaticMesh)
		return;
	m_MeshCB->UpdateBuffer(currentFrameIndex);
	m_MeshCB->SetGPUBuffer(currentFrameIndex, 0, JS_VS);

	m_SpriteMeshCB->UpdateBuffer(currentFrameIndex);
	m_SpriteMeshCB->SetGPUBuffer(currentFrameIndex, 3, JS_VS);

	m_Materials[m_CurrentAnimation.second]->UpdateBuffer(currentFrameIndex);
	m_Materials[m_CurrentAnimation.second]->SetGPUBuffer(currentFrameIndex, 2, JS_PS);
	m_StaticMesh->Render();
}


void JSpriteAnimationObject::SetSpriteSet(std::shared_ptr<JContent> spriteSet)
{
	auto p = std::dynamic_pointer_cast<JSpriteAnimationSet>(spriteSet);
	if (!p) {
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("{0} JSpriteAnimationObject에서 잘못된 SetSpriteSet을 호출했습니다.", m_name.c_str());
#endif
		return;
	}
	m_SpriteSet = p;
}