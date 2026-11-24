// =========================================
// 
// =========================================


#pragma once
#include "JContent.h"

class JSound : public JContent {
public:
	JSound(const char* name = nullptr) : JContent(name) {}
	JSound(std::filesystem::path filepath) : JContent(filepath) {}
	~JSound();

protected:
	// Sound는 만들 수 없음, 아무 파일도 만들지 않는다.
	void CreateContentFile() {}
	void LoadFromFile();

protected:
	// XAudio2 관련 자료 저장, 이거는 나중에 바꿀 수도 있음 (XAudio2가 Window전용임)
	
	std::unique_ptr<BYTE[]> m_RawAudioData{};
	IXAudio2SourceVoice* m_SourceVoice{};
};