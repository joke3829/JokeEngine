#include "JSoundResourceXAudio2.h"
#include "Engine/JContent/JSound.h"
#include "JSoundEngine.h"

SourceVoicePtr::SourceVoicePtr(JSound* sound)
{
	if (sound) {
		auto p =JSoundEngine::GetInstance()->GetXAudio2();
		p->CreateSourceVoice(&m_Voice, sound->GetWaveFormatEX());
	}
}

SourceVoicePtr::~SourceVoicePtr()
{
	if (m_Voice) m_Voice->DestroyVoice();
}

void SourceVoicePtr::ReMakeVoice(JSound* sound)
{
	if (sound) {
		if (m_Voice) m_Voice->DestroyVoice();
		auto p = JSoundEngine::GetInstance()->GetXAudio2();
		ThrowIfFailed(p->CreateSourceVoice(&m_Voice, sound->GetWaveFormatEX()));
	}
}



JSoundResourceXAudio2::JSoundResourceXAudio2()
{
	m_SourceVoices.reserve(128);
	// 1개는 준비해놓기
	m_SourceVoices.emplace_back(m_Sound);
}

void JSoundResourceXAudio2::SetSound(JContent* sound)
{
	if (auto p = dynamic_cast<JSound*>(sound)) {
		m_Sound = p;
		for (auto& ptr : m_SourceVoices)
			ptr.ReMakeVoice(m_Sound);
	}
}

void JSoundResourceXAudio2::SetVolume()
{
}

void JSoundResourceXAudio2::Play(bool begin)
{
	if (m_Sound) {
		XAUDIO2_BUFFER buffer{};
		buffer.AudioBytes = m_Sound->GetRawAudioDataSize();
		buffer.pAudioData = m_Sound->GetRawAudioData();
		m_SourceVoices[0].m_Voice->SubmitSourceBuffer(&buffer);
		m_SourceVoices[0].m_Voice->Start();
	}
}

void JSoundResourceXAudio2::Pause()
{
}

void JSoundResourceXAudio2::Stop()
{
}

void JSoundResourceXAudio2::SetSubMixGroup(std::string& submixname)
{
}

void JSoundResourceXAudio2::SetMultiSource(UINT num)
{
	size_t s = m_SourceVoices.size();

	if (num > 128) num = 128;
	m_SourceVoices.resize(num);
	for (int i = s; i < m_SourceVoices.size(); ++i) {
		m_SourceVoices[i].ReMakeVoice(m_Sound);
	}
}

