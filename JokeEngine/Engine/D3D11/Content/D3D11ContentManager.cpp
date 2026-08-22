#include "D3D11ContentManager.h"
#include "D3D11Material.h"
#include "D3D11StaticMesh.h"
#include "D3D11Texture.h"
#include "Engine/JContent/JSpriteAnimationSet.h"

void JContentManagerDX11::ReadyDefaultContent()
{
	// 1. Default StaticMesh 준비
	{	// Default Cube
		std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>(XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(2.5f, 2.5f, 2.5f), "DefaultCube");
		//std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>(XMFLOAT3(0.f, 0.f, 0.f), 2.5f, 20, 20, 1, "DefaultCube");
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(mesh->GetName(), mesh);
#if defined(_DEBUG) || defined(DEBUG)
		if(not inserted) ShowInsertedFailed(mesh->GetName(), "ContentManager");
#endif
	}

	{	// DefaultPlane
		std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>(XMFLOAT3(0.f, 0.f, 0.f), 1.f, 1.f, JSMPlane::quad1, "DefaultPlane");
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(mesh->GetName(), mesh);
#if defined(_DEBUG) || defined(DEBUG)
		if (not inserted) ShowInsertedFailed(mesh->GetName(), "ContentManager");
#endif
	}

	// 2. Default Texture, testTexture 준비
	{
		std::shared_ptr<JTextureDX11> texture = std::make_shared<JTextureDX11>();
		texture->MakeNullTexture();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(texture->GetName(), texture);
#if defined(_DEBUG) || defined(DEBUG)
		if (not inserted) ShowInsertedFailed(texture->GetName(), "ContentManager");
#endif
	}
	{
		std::shared_ptr<JTextureDX11> texture = std::make_shared<JTextureDX11>(L"Resource/test.dds", "TestTexture");
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(texture->GetName(), texture);
#if defined(_DEBUG) || defined(DEBUG)
		if (not inserted) ShowInsertedFailed(texture->GetName(), "ContentManager");
#endif
	}
	{
		std::shared_ptr<JTextureDX11> texture = std::make_shared<JTextureDX11>(L"Resource/SpriteAnimation/mushroom.dds", "MushroomTexture");
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(texture->GetName(), texture);
#if defined(_DEBUG) || defined(DEBUG)
		if (not inserted) ShowInsertedFailed(texture->GetName(), "ContentManager");
#endif
	}

	// 3. Default Material 준비 ! Material은 반드시 Texture Load 후에 진행 =======================================================

	{
		std::shared_ptr<JMaterialDX11> material = std::make_shared<JMaterialDX11>("DefaultMaterial");
		material->SetTextureNull(m_ContentLookupTable["NullTexture"]);
		//material->SetTexture(m_ContentLookupTable["TestTexture"], TextureOrder::Albedo);

		auto [iter, inserted] = m_ContentLookupTable.try_emplace(material->GetName(), material);
#if defined(_DEBUG) || defined(DEBUG)
		if (not inserted) ShowInsertedFailed(material->GetName(), "ContentManager");
#endif
	}

	{
		std::shared_ptr<JMaterialDX11> material = std::make_shared<JMaterialDX11>("DefaultSpriteMaterial");
		material->SetTextureNull(m_ContentLookupTable["NullTexture"]);
		material->SetTexture(m_ContentLookupTable["MushroomTexture"], TextureOrder::Albedo);

		auto [iter, inserted] = m_ContentLookupTable.try_emplace(material->GetName(), material);
#if defined(_DEBUG) || defined(DEBUG)
		if (not inserted) ShowInsertedFailed(material->GetName(), "ContentManager");
#endif
	}

	// 4. SpriteAnimationSet
	{
		std::shared_ptr<JSpriteAnimationSet> sprite = std::make_shared<JSpriteAnimationSet>("TestSprite");
		sprite->LoadAnimationFactorFromYaml("Resource/SpriteAnimation/SpriteAnimationSample.yaml");
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(sprite->GetName(), sprite);
#if defined(_DEBUG) || defined(DEBUG)
		if (not inserted) ShowInsertedFailed(sprite->GetName(), "ContentManager");
#endif
	}

#if defined(_DEBUG) || defined(DEBUG)
	spdlog::info("ContentManager의 DefualtContent가 준비되었습니다.");
#endif
}
