#include "D3D11ContentManager.h"
#include "D3D11Material.h"
#include "D3D11StaticMesh.h"
#include "D3D11Texture.h"
#include "Engine/JContent/JSpriteAnimationSet.h"

void JContentManagerDX11::ReadyDefaultContent()
{
	// 1. Default StaticMesh 준비
	{	// Default Cube
		std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>(XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(2.5f, 2.5f, 2.5f), "Resource/JKEngine/Meshes/DefaultCube");
		mesh->Initialize();
		//std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>(XMFLOAT3(0.f, 0.f, 0.f), 2.5f, 20, 20, 1, "DefaultCube");
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(mesh->GetFullFilePath(), mesh);
#if DEV_MODE
		if(not inserted) ShowInsertedFailed(mesh->GetFullFilePath().string(), "ContentManager");
#endif
	}
	{ // Default Sphere
		std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>(XMFLOAT3(0.f, 0.f, 0.f), 2.5f, 20, 20, 0, "Resource/JKEngine/Meshes/DefaultSphere");
		mesh->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(mesh->GetFullFilePath(), mesh);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(mesh->GetFullFilePath().string(), "ContentManager");
#endif
	}

	{	// DefaultPlane
		//quad1
		std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>(XMFLOAT3(0.f, 0.f, 0.f), 1.f, 1.f, JSMPlane::quad1, "Resource/JKEngine/Meshes/DefaultPlaneq1");
		mesh->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(mesh->GetFullFilePath(), mesh);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(mesh->GetFullFilePath().string(), "ContentManager");
#endif
	}
	{
		std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>(XMFLOAT3(0.f, 0.f, 0.f), 1.f, 1.f, JSMPlane::quad2, "Resource/JKEngine/Meshes/DefaultPlaneq2");
		mesh->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(mesh->GetFullFilePath(), mesh);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(mesh->GetFullFilePath().string(), "ContentManager");
#endif
	}
	{
		std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>(XMFLOAT3(0.f, 0.f, 0.f), 1.f, 1.f, JSMPlane::quad3, "Resource/JKEngine/Meshes/DefaultPlaneq3");
		mesh->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(mesh->GetFullFilePath(), mesh);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(mesh->GetFullFilePath().string(), "ContentManager");
#endif
	}
	{
		std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>(XMFLOAT3(0.f, 0.f, 0.f), 1.f, 1.f, JSMPlane::quad4, "Resource/JKEngine/Meshes/DefaultPlaneq4");
		mesh->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(mesh->GetFullFilePath(), mesh);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(mesh->GetFullFilePath().string(), "ContentManager");
#endif
	}
	{
		std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>(XMFLOAT3(0.f, 0.f, 0.f), 1.f, 1.f, JSMPlane::quad_no, "Resource/JKEngine/Meshes/DefaultPlane");
		mesh->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(mesh->GetFullFilePath(), mesh);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(mesh->GetFullFilePath().string(), "ContentManager");
#endif
	}

	// 2. Default Texture, testTexture 준비
	{
		// MissingTexture
		std::shared_ptr<JTextureDX11> texture = std::make_shared<JTextureDX11>("Resource/JKEngine/Textures/MissingTexture.png");
		texture->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(texture->GetFullFilePath(), texture);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(texture->GetFullFilePath().string(), "ContentManager");
#endif
	}
	{	// Folder Texture
		std::shared_ptr<JTextureDX11> texture = std::make_shared<JTextureDX11>("Resource/JKEngine/Textures/Folder.dds");
		texture->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(texture->GetFullFilePath(), texture);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(texture->GetFullFilePath().string(), "ContentManager");
#endif
	}
	{	// MeshFile Texture
		std::shared_ptr<JTextureDX11> texture = std::make_shared<JTextureDX11>("Resource/JKEngine/Textures/StaticMeshFile.dds");
		texture->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(texture->GetFullFilePath(), texture);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(texture->GetFullFilePath().string(), "ContentManager");
#endif
	}
	{
		std::shared_ptr<JTextureDX11> texture = std::make_shared<JTextureDX11>("Resource/test.dds");
		texture->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(texture->GetFullFilePath(), texture);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(texture->GetFullFilePath().string(), "ContentManager");
#endif
	}
	{
		std::shared_ptr<JTextureDX11> texture = std::make_shared<JTextureDX11>("Resource/SpriteAnimation/mushroom.dds");
		texture->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(texture->GetFullFilePath(), texture);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(texture->GetFullFilePath().string(), "ContentManager");
#endif
	}

	// 3. Default Material 준비 ! Material은 반드시 Texture Load 후에 진행 =======================================================

	{
		std::shared_ptr<JMaterialDX11> material = std::make_shared<JMaterialDX11>("Resource/JKEngine/Materials/DefaultMaterial");
		material->Initialize();
		material->SetMissingTexture(m_ContentLookupTable["Resource/JKEngine/Textures/MissingTexture.png"]);
		material->SetTexture(m_ContentLookupTable["Resource/test.dds"], TextureOrder::Albedo);

		auto [iter, inserted] = m_ContentLookupTable.try_emplace(material->GetFullFilePath(), material);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(material->GetFullFilePath().string(), "ContentManager");
#endif
	}

	{
		std::shared_ptr<JMaterialDX11> material = std::make_shared<JMaterialDX11>("Resource/JKEngine/Materials/DefaultSpriteMaterial");
		material->Initialize();
		material->SetMissingTexture(m_ContentLookupTable["Resource/JKEngine/Textures/MissingTexture.png"]);
		material->SetTexture(m_ContentLookupTable["Resource/SpriteAnimation/mushroom.dds"], TextureOrder::Albedo);

		auto [iter, inserted] = m_ContentLookupTable.try_emplace(material->GetFullFilePath(), material);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(material->GetFullFilePath().string(), "ContentManager");
#endif
	}

	// 4. SpriteAnimationSet
	{
		std::shared_ptr<JSpriteAnimationSet> sprite = std::make_shared<JSpriteAnimationSet>("Resource/SpriteAnimation/TestSprite");
		sprite->Initialize();
		sprite->LoadAnimationFactorFromYaml("Resource/SpriteAnimation/SpriteAnimationSample.yaml");
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(sprite->GetFullFilePath(), sprite);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(sprite->GetFullFilePath().string(), "ContentManager");
#endif
	}

#if DEV_MODE
	spdlog::info("ContentManager의 DefualtContent가 준비되었습니다.");
#endif
}
