#include "D3D11ContentManager.h"
#include "D3D11Material.h"
#include "D3D11StaticMesh.h"
#include "D3D11Texture.h"

bool JContentManagerDX11::AddContentFromFile(std::filesystem::path filepath)
{
	bool ret = JContentManager::AddContentFromFile(filepath);

	if (filepath.has_extension() && not ret) {
		const auto& ext = filepath.extension();
		std::shared_ptr<JContent> content{};
		if (ext == ".dds" || ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp" || ext == ".tiff" || ext == ".tif" || ext == ".gif" || ext == ".ico" || ext == ".cur") 
			content = std::make_shared<JTextureDX11>(filepath);
		else if (ext == ".jkmh")
			content = std::make_shared<JStaticMeshDX11>(filepath);
		else if (ext == ".jkmt")
			content = std::make_shared<JMaterialDX11>(filepath);

		if (content) {
			if (not content->Initialize()) {
#if DEV_MODE
				spdlog::warn("{}: Load Failed - Currupted or Not Supported File", filepath.string());
#endif
				return false;
			}
			m_ContentLookupTable.try_emplace(filepath, content);
			return true;
		}
	}
	return ret;
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
