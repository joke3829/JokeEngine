// =========================================
// 
// =========================================


#pragma once
#include "JContent.h"

class JSound : public JContent {
public:
	JSound(const char* name = nullptr) : JContent(name) {}
	JSound(std::filesystem::path filepath) : JContent(filepath) {}

	const WAVEFORMATEXTENSIBLE& GetWaveFormatEX() { return m_WaveFormatEX; }
	const BYTE* GetRawAudioData() { return m_RawAudioData.get(); }

protected:
	// Sound는 만들 수 없음, 아무 파일도 만들지 않는다.
	void CreateContentFile() {}
	void LoadFromFile();

protected:
	std::unique_ptr<BYTE[]> m_RawAudioData{};
	WAVEFORMATEXTENSIBLE	m_WaveFormatEX{};
};