#include "JokeEngineConfig.h"

JEngineDefaultGlobalConfig* JEngineDefaultGlobalConfig::m_Instance = nullptr;

void JEngineConfigDefaultFactor::InitFactorsByFile(nlohmann::json json)
{
	if (json.empty()) {
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("InitFactorsByFile을 실행하던 중 json에 문제가 있어 실패했습니다. 프로그램을 종료합니다.");
#endif
		assert(0);
	}

	DirectX_Version = json.value("DX", 11);
	LimitFPS = json.value("FPS", 120.f);
	VerticalSYNC = json.value("V_SYNC", false);
	if (json.contains("Windows"))
	{
		auto& windowJson = json["Windows"];

		WindowsFullscreenState = windowJson.value("Fullscreen", false);

		// 3. "Windows" -> "Resolution" 깊은 계층 구조 파싱
		if (windowJson.contains("Resolution"))
		{
			auto& resJson = windowJson["Resolution"];
			WindowsWidth = resJson.value("Width", 1280);
			WindowsHeight = resJson.value("Height", 720);
		}
	}
}

// =====================================================================

JEngineDefaultGlobalConfig* JEngineDefaultGlobalConfig::GetInstance()
{
	if (!m_Instance)
		m_Instance = new JEngineDefaultGlobalConfig;
	return m_Instance;
}

JEngineConfigDefaultFactor* JEngineDefaultGlobalConfig::GetConfigFactor()
{
	return m_ConfigFactor;
}

void JEngineDefaultGlobalConfig::OptionSaveAndFileGenerate()
{
	const char* optionFileName = "JEngine_Config.json";
	std::ofstream outFile{ optionFileName };
	nlohmann::json outJson;
	outJson["DX"] = m_ConfigFactor->DirectX_Version;
	outJson["Windows"]["Fullscreen"] = m_ConfigFactor->WindowsFullscreenState;
	outJson["Windows"]["Resolution"]["Width"] = m_ConfigFactor->WindowsWidth;
	outJson["Windows"]["Resolution"]["Height"] = m_ConfigFactor->WindowsHeight;

	outJson["FPS"] = m_ConfigFactor->LimitFPS;			// 30, 60, 120, 144, 240, 999(Unlimit)

	outFile << outJson;
#if defined(_DEBUG) || defined(DEBUG)
	spdlog::error("'{0}' Save And Generate Success", optionFileName);
#endif
}

JEngineDefaultGlobalConfig::JEngineDefaultGlobalConfig()
{
	m_ConfigFactor = new JEngineConfigDefaultFactor;

	const char* optionFileName = "JEngine_Config.json";
	std::ifstream configFile{ optionFileName };
	if (!configFile.is_open()) {
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("'{0}' Read Failed, New '{0}' Generate Start", optionFileName);
#endif
		{
			std::ofstream outFile{ optionFileName };
			nlohmann::json outJson;
			outJson["DX"] = m_ConfigFactor->DirectX_Version;
			outJson["Windows"]["Fullscreen"] = m_ConfigFactor->WindowsFullscreenState;
			outJson["Windows"]["Resolution"]["Width"] = m_ConfigFactor->WindowsWidth;
			outJson["Windows"]["Resolution"]["Height"] = m_ConfigFactor->WindowsHeight;

			outJson["V_SYNC"] = m_ConfigFactor->VerticalSYNC;
			outJson["FPS"] = m_ConfigFactor->LimitFPS;			// 30, 60, 120, 144, 240, 999(Unlimit)

			outFile << outJson;

			outFile.close();
		}


		configFile.close();
		configFile.open(optionFileName);
	}

	nlohmann::json configjson = nlohmann::json::parse(configFile);

	m_ConfigFactor->InitFactorsByFile(configjson);

#if defined(_DEBUG) || defined(DEBUG)
	spdlog::info("'{0}' Read Success", optionFileName);
#endif
}

JEngineDefaultGlobalConfig::~JEngineDefaultGlobalConfig()
{
	if (m_ConfigFactor) delete m_ConfigFactor;
	if (m_Instance) delete m_Instance;
}


