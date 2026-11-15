#include "JSpriteAnimationSet.h"
#include "JContentManager.h"


JSpriteAnimationSet::JSpriteAnimationSet(const char* name)
	: JContent(name)
{
}

const JSAnimData& JSpriteAnimationSet::GetKeyFrameData(std::string& animName, float& time)
{
	if (!m_KeyFrame.contains(animName)) {
		if (!m_KeyFrame.empty()) {
			auto s = m_KeyFrame.begin();
			animName = s->first;
		}
		else {
#if DEV_MODE
			spdlog::error("{0} m_KeyFrame이 비어있습니다.", m_name.c_str());
#endif
			JSAnimData data{};
			return data;
		}
	}

	auto& s = m_KeyFrame[animName];
	float playtime = m_PlayTime[animName];
	while (time > playtime)
		time -= playtime;

	auto it = std::upper_bound(s.begin(), s.end(), time, [](float value, JSAnimData& frame) {
		return value < frame.keyframeTime;
		});

	// time이 [0]보다 작았을 경우 상정 안함, 문제 있을 시 변경
	--it;
	return *it;
}

void JSpriteAnimationSet::LoadAnimationFactorFromYaml(const char* filepath)
{
	std::ifstream inFile{ filepath, std::ios::binary };
	if (not inFile.is_open()) {
#if DEV_MODE
		spdlog::error("{0} Load Failed - File Not Exist(JSpriteAnimationSet)", filepath);
#endif
		return;
	}

	try {
		// 중간 실패시 방어 없음
		auto node = fkyaml::node::deserialize(inFile);

		if (node.contains("Animations")) {
			m_KeyFrame.clear();
			m_PlayTime.clear();
		}

		for (auto& tracks : node["Animations"]) {
			std::string trackName = tracks["TrackName"].get_value<std::string>();
			UINT materialIndex = tracks["MaterialIndex"].get_value<unsigned int>();
			float playtime = tracks["PlayTime"].get_value<float>();
			float ImageWidth = tracks["T_Width"].get_value<float>();
			float ImageHeight = tracks["T_Height"].get_value<float>();
			float perWidth = tracks["S_Width"].get_value<float>();
			float perHeight = tracks["S_Height"].get_value<float>();

			std::vector<JSAnimData> datas{};
			for (auto& frames : tracks["Keyframes"]) {
				JSAnimData data{};
				data.keyframeTime = frames["Time"].get_value<float>();
				data.row = frames["Row"].get_value<float>();
				data.col = frames["Col"].get_value<float>();
				data.materialIndex = materialIndex;
				data.perWidth = perWidth;
				data.perHeight = perHeight;
				data.ImageWidth = ImageWidth;
				data.ImageHeight = ImageHeight;

				datas.emplace_back(data);
			}
			// 무사히 다 정보가 오면 map에 추가
			auto [iter, inserted] = m_KeyFrame.try_emplace(trackName, std::move(datas));
#if DEV_MODE
			if (not inserted) ShowInsertedFailed(trackName, "m_KeyFrame");
#endif
			m_PlayTime.try_emplace(trackName, playtime);
		}
	}
	catch (fkyaml::exception& e) {
#if DEV_MODE
		spdlog::error("{0} Load Failed", filepath);
		spdlog::error("{0}", e.what());
#endif
		return;
	}
}



#if DEV_MODE 
void JSpriteAnimationSet::RenderGUI(ImVec2 startPos, void* imageref)
{
	JContent::RenderGUI(startPos, JGlobalContentManager::GetInstance()->GetManager()->GetContentTable()["Resource/JKEngine/Textures/SpriteAnimationSetFile.dds"]->GetDedicatedFactor());
}

extern HWND g_Hwnd;

void JSpriteAnimationSet::RenderDetailPanel()
{
	JContent::RenderDetailPanel();
	ImGui::SeparatorText("Animation");
	// yaml읽어서 로드 하는 거 추가하기;
	ImGui::Text("LoadFromYamlFile");
	if (ImGui::Button("Load##YamlFileSprite")) {
		wchar_t filePath[256]{};

		OPENFILENAME ofn{};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = g_Hwnd;
		ofn.lpstrFile = filePath;
		ofn.nMaxFile = sizeof(filePath) / sizeof(wchar_t);
		ofn.lpstrFilter = L"Sprite YamlFile\0*.yaml\0";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		ofn.lpstrInitialDir = L"Resource";
		if (GetOpenFileName(&ofn) == TRUE){
			std::string spath = ConvertHelper::WideToUtf8(filePath);
			LoadAnimationFactorFromYaml(spath.c_str());
			m_UISelectedAnimationSet = 0;
		}
		else {
			spdlog::error("Sprite Yaml Load Failed");
		}
	}

	// 키프레임 에디터 - 이건 나중에

	// 키프레임 뷰어
	// 리스트 + 고른거를 키프레임 정보를 보여주기
	if (0 == m_KeyFrame.size()) {
		ImGui::Text("No Animation Data");
	}
	else {
		std::vector<std::string> animation_names(m_KeyFrame.size());
		int ii = 0;
		for (auto& str : m_KeyFrame) {
			animation_names[ii] = str.first;
			++ii;
		}

		if (ImGui::BeginCombo("Animations", animation_names[m_UISelectedAnimationSet].c_str())) {
			for (int i = 0; i < animation_names.size(); ++i) {
				bool select = i == m_UISelectedAnimationSet;
				if (ImGui::Selectable(std::string(animation_names[i] + "##AnimationSet").c_str(), select))
					m_UISelectedAnimationSet = i;

				if (select)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		auto& aniinfo = m_KeyFrame[animation_names[m_UISelectedAnimationSet]];
		float anitime = m_PlayTime[animation_names[m_UISelectedAnimationSet]];

		ImGui::Text("NumKeyFrame: %d", aniinfo.size());
		ImGui::Text("Animation PlayTime: %.3f", anitime);
	}

}
#endif 