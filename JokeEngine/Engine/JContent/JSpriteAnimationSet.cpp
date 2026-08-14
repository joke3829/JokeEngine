#include "JSpriteAnimationSet.h"


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
#if defined(_DEBUG) || defined(DEBUG)
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
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("{0} Load Failed - File Not Exist(JSpriteAnimationSet)", filepath);
#endif
		return;
	}

	try {
		auto node = fkyaml::node::deserialize(inFile);

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
#if defined(_DEBUG) || defined(DEBUG)
			if (not inserted) ShowInsertedFailed(trackName, "m_KeyFrame");
#endif
			m_PlayTime.try_emplace(trackName, playtime);
		}
	}
	catch (fkyaml::exception& e) {
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("{0} Load Failed", filepath);
		spdlog::error("{0}", e.what());
#endif
		return;
	}
}
