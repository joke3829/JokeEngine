// =========================================
// 
// =========================================


#pragma once
#include "JContent.h"

class JSound : public JContent {
public:
	JSound(const char* name = nullptr) : JContent(name) {}
	JSound(std::filesystem::path filepath) : JContent(filepath) {}

	const WAVEFORMATEX* GetWaveFormatEX() { return reinterpret_cast<WAVEFORMATEX*>(m_WaveFormat.get()); }
	const BYTE* GetRawAudioData() { return m_RawAudioData.get(); }
	const uint32_t& GetRawAudioDataSize() { return m_RawAudioDataSize; }

protected:
	// Sound는 만들 수 없음, 아무 파일도 만들지 않는다.
	bool CreateContentFile() { return false; }
	bool LoadFromFile();

protected:
	uint32_t m_RawAudioDataSize{};
	std::unique_ptr<BYTE[]> m_RawAudioData{};
	std::unique_ptr<BYTE[]> m_WaveFormat{};
};