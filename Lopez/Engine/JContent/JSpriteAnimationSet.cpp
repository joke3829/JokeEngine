#include "JSpriteAnimationSet.h"


const JSAnimData& JSpriteAnimationSet::GetKeyFrameData(const char* animName, float& time)
{
	JSAnimData test;
	return test;
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
