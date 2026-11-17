#include "D3D11ContentManager.h"
#include "D3D11Material.h"
#include "D3D11StaticMesh.h"
#include "D3D11Texture.h"
#include "Engine/JContent/JSpriteAnimationSet.h"

void JContentManagerDX11::ReadyDefaultContent()
{
	// 1. Default StaticMesh 준비
	{	// Default Cube
		std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>("Resource/JKEngine/Meshes/Cube.jkmh");
		mesh->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(mesh->GetFullFilePath(), mesh);
#if DEV_MODE
		if(not inserted) ShowInsertedFailed(mesh->GetFullFilePath().string(), "ContentManager");
#endif
	}
	{ // Default Sphere
		std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>("Resource/JKEngine/Meshes/Sphere.jkmh");
		mesh->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(mesh->GetFullFilePath(), mesh);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(mesh->GetFullFilePath().string(), "ContentManager");
#endif
	}

	{	// DefaultPlane
		//quad1
		std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>("Resource/JKEngine/Meshes/Planeq1.jkmh");
		mesh->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(mesh->GetFullFilePath(), mesh);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(mesh->GetFullFilePath().string(), "ContentManager");
#endif
	}
	{
		std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>("Resource/JKEngine/Meshes/Planeq2.jkmh");
		mesh->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(mesh->GetFullFilePath(), mesh);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(mesh->GetFullFilePath().string(), "ContentManager");
#endif
	}
	{
		std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>("Resource/JKEngine/Meshes/Planeq3.jkmh");
		mesh->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(mesh->GetFullFilePath(), mesh);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(mesh->GetFullFilePath().string(), "ContentManager");
#endif
	}
	{
		std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>("Resource/JKEngine/Meshes/Planeq4.jkmh");
		mesh->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(mesh->GetFullFilePath(), mesh);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(mesh->GetFullFilePath().string(), "ContentManager");
#endif
	}
	{
		std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>("Resource/JKEngine/Meshes/Plane.jkmh");
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
		std::shared_ptr<JTextureDX11> texture = std::make_shared<JTextureDX11>("Resource/JKEngine/Textures/MaterialFile.dds");
		texture->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(texture->GetFullFilePath(), texture);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(texture->GetFullFilePath().string(), "ContentManager");
#endif
	}
	{
		std::shared_ptr<JTextureDX11> texture = std::make_shared<JTextureDX11>("Resource/JKEngine/Textures/SpriteAnimationSetFile.dds");
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
		std::shared_ptr<JMaterialDX11> material = std::make_shared<JMaterialDX11>("Resource/JKEngine/Materials/DefaultMaterial.jkmt");
		material->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(material->GetFullFilePath(), material);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(material->GetFullFilePath().string(), "ContentManager");
#endif
	}

	{
		std::shared_ptr<JMaterialDX11> material = std::make_shared<JMaterialDX11>("Resource/JKEngine/Materials/DefaultSpriteMaterial.jkmt");
		material->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(material->GetFullFilePath(), material);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(material->GetFullFilePath().string(), "ContentManager");
#endif
	}

	// 4. SpriteAnimationSet
	{
		std::shared_ptr<JSpriteAnimationSet> sprite = std::make_shared<JSpriteAnimationSet>("Resource/SpriteAnimation/TestSprite.jkspt");
		sprite->Initialize();
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(sprite->GetFullFilePath(), sprite);
#if DEV_MODE
		if (not inserted) ShowInsertedFailed(sprite->GetFullFilePath().string(), "ContentManager");
#endif
	}

#if DEV_MODE
	spdlog::info("ContentManager의 DefualtContent가 준비되었습니다.");
#endif
}

void JContentManagerDX11::AddContentFromFile(std::filesystem::path filepath)
{
	if (filepath.has_extension()) {
		const auto& ext = filepath.extension();
		if (ext == ".dds" || ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp" || ext == ".tiff" || ext == ".tif" || ext == ".gif" || ext == ".ico" || ext == ".cur") {
			std::shared_ptr<JTextureDX11> texture = std::make_shared<JTextureDX11>(filepath);
			texture->Initialize();
			m_ContentLookupTable.try_emplace(filepath, texture);
		}
		else if (ext == ".jkmh") {
			std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>(filepath);
			mesh->Initialize();
			m_ContentLookupTable.try_emplace(filepath, mesh);
		}
		else if (ext == ".jkspt") {
			std::shared_ptr<JSpriteAnimationSet> sset = std::make_shared<JSpriteAnimationSet>(filepath);
			sset->Initialize();
			m_ContentLookupTable.try_emplace(filepath, sset);
		}
		else if (ext == ".jkmt") {
			std::shared_ptr<JMaterialDX11> material = std::make_shared<JMaterialDX11>(filepath);
			material->Initialize();
			m_ContentLookupTable.try_emplace(filepath, material);
		}
	}
}



#if DEV_MODE
void JContentManagerDX11::AddMeshWithFlag(BYTE& flag, std::filesystem::path& filepath)
{
	if (flag & 0b00000001) {	// cube
		ImGui::DragFloat3("center##cubecenter", reinterpret_cast<float*>(&meshCreateFactor.center));
		ImGui::DragFloat3("extent##cubeextent", reinterpret_cast<float*>(&meshCreateFactor.extent), 1.f, 0.1f, 999999.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
		if (ImGui::Button("Create##Mesh", ImVec2(80, 0))) {
			std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>(meshCreateFactor.center, meshCreateFactor.extent, filepath.string().c_str());
			mesh->Initialize();
			m_ContentLookupTable.try_emplace(filepath, mesh);
			flag = 0;
			ImGui::CloseCurrentPopup();
		} ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(80, 0))) { flag = 0; ImGui::CloseCurrentPopup(); }
	}
	else if (flag & 0b00000010) { // plane
		ImGui::DragFloat3("center##planecenter", reinterpret_cast<float*>(&meshCreateFactor.center));
		ImGui::DragFloat2("width height##planeextent", reinterpret_cast<float*>(&meshCreateFactor.extent), 1.f, 0.1f, 999999.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
		int q = static_cast<int>(meshCreateFactor.extent.z); if (q < 0 || q > 4) q = 0;
		ImGui::SliderInt("Quad##plane", &q, 0, 4, "%d", ImGuiSliderFlags_AlwaysClamp);
		switch (q) {
		case 0: ImGui::Text("quad_no"); break;
		case 1: ImGui::Text("quad1"); break;
		case 2: ImGui::Text("quad2"); break;
		case 3: ImGui::Text("quad3"); break;
		case 4: ImGui::Text("quad4"); break;
		}
		meshCreateFactor.extent.z = static_cast<float>(q);
		if (ImGui::Button("Create##Mesh", ImVec2(80, 0))) {
			std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>(meshCreateFactor.center, meshCreateFactor.extent.x, meshCreateFactor.extent.y, JSMPlane(q), filepath.string().c_str());
			mesh->Initialize();
			m_ContentLookupTable.try_emplace(filepath, mesh);
			flag = 0;
			ImGui::CloseCurrentPopup();
		} ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(80, 0))) { flag = 0; ImGui::CloseCurrentPopup(); }
	}
	else if (flag & 0b00000100) { // sphere
		ImGui::DragFloat3("center##spherecenter", reinterpret_cast<float*>(&meshCreateFactor.center));
		ImGui::DragFloat("radius##sphereradius", reinterpret_cast<float*>(&meshCreateFactor.extent), 1.f, 0.1f, 999999.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
		int count, stack; count = static_cast<int>(meshCreateFactor.extent.y); stack = static_cast<int>(meshCreateFactor.extent.z);
		if (count < 8 || count > 128) count = 8;if (stack < 8 || stack > 128) stack = 8;
		ImGui::SliderInt("SliceCount##sphere", &count, 8, 128, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("StackCount##sphere", &stack, 8, 128, "%d", ImGuiSliderFlags_AlwaysClamp);
		meshCreateFactor.extent.y = static_cast<float>(count);meshCreateFactor.extent.z = static_cast<float>(stack);
		if (ImGui::Button("Create##Mesh", ImVec2(80, 0))) {
			std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>(meshCreateFactor.center, meshCreateFactor.extent.x, count, stack, 0, filepath.string().c_str());
			mesh->Initialize();
			m_ContentLookupTable.try_emplace(filepath, mesh);
			flag = 0;
			ImGui::CloseCurrentPopup();
		} ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(80, 0))) { flag = 0; ImGui::CloseCurrentPopup(); }
	}
}
#endif
