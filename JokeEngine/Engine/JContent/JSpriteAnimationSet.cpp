#include "JSpriteAnimationSet.h"
#include "JContentManager.h"


const JSAnimData& JSpriteAnimationSet::GetKeyFrameData(std::string& animName, float& time)
{
	if (!m_KeyFrame.contains(animName)) {
		if (!m_KeyFrame.empty()) {
			auto s = m_KeyFrame.begin();
			animName = s->first;
		}
		else {
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

bool JSpriteAnimationSet::CreateContentFile()
{
	JContent::CreateContentFile();
	std::ofstream out{ m_FullFilePath, std::ios::binary };
	unsigned int numAni = m_KeyFrame.size();
	// 애니메이션 개수
	out.write(reinterpret_cast<const char*>(&numAni), sizeof(unsigned int));

	for (auto& adata : m_KeyFrame) {
		// key 문자열 길이 + 문자열
		const std::string& key = adata.first;
		unsigned int keylen = key.length();
		out.write(reinterpret_cast<const char*>(&keylen), sizeof(unsigned int));
		out.write(key.data(), keylen);
		// 애니메이션 총 시간
		float time = m_PlayTime[key];
		out.write(reinterpret_cast<const char*>(&time), sizeof(float));
		// JSAnimData 개수 + 값
		auto& vs = adata.second;
		unsigned int vssize = vs.size();
		out.write(reinterpret_cast<const char*>(&vssize), sizeof(unsigned int));
		out.write(reinterpret_cast<const char*>(vs.data()), sizeof(JSAnimData) * vssize);
	}
	return true;
}

bool JSpriteAnimationSet::LoadFromFile()
{
	std::ifstream in{ m_FullFilePath, std::ios::binary };
	if (not in) return false;
	unsigned int numAni{};
	in.read(reinterpret_cast<char*>(&numAni), sizeof(unsigned int));
	for (unsigned int i = 0; i < numAni; ++i) {
		unsigned int keylen{}; in.read(reinterpret_cast<char*>(&keylen), sizeof(unsigned int));
		std::string key; key.resize(keylen); in.read(key.data(), keylen);
		float anitime{}; in.read(reinterpret_cast<char*>(&anitime), sizeof(float));
		unsigned int numd{}; in.read(reinterpret_cast<char*>(&numd), sizeof(unsigned int));
		std::vector<JSAnimData> tempdata(numd);
		in.read(reinterpret_cast<char*>(tempdata.data()), sizeof(JSAnimData) * numd);

		m_KeyFrame.try_emplace(key, std::move(tempdata));
		m_PlayTime.try_emplace(key, anitime);
	}
	return true;
}

#if DEV_MODE 
void JSpriteAnimationSet::RenderGUI(ImVec2 startPos, void* imageref)
{
	auto& content = JGlobalContentManager::GetInstance()->GetManager()->GetContentTable();
	void* imagep = content.contains("Resource/JKEngine/Textures/SpriteAnimationSetFile.dds") ? content["Resource/JKEngine/Textures/SpriteAnimationSetFile.dds"]->GetDedicatedFactor() : nullptr;
	JContent::RenderGUI(startPos, imagep);
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
		std::wstring fpath = std::filesystem::absolute(L"Resource").wstring();
		ofn.lpstrInitialDir = fpath.c_str();
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

void JSpriteAnimationSet::ReadyTempData()
{
	JContent::ReadyTempData();
	m_TempKeyFrame = m_KeyFrame;
	m_TempPlayTime = m_PlayTime;
}

void JSpriteAnimationSet::ApplyTempDataToRealData()
{
	JContent::ApplyTempDataToRealData();
}

void JSpriteAnimationSet::CancelTempDataModify()
{
	m_KeyFrame = m_TempKeyFrame;
	m_PlayTime = m_TempPlayTime;
}
#endif 