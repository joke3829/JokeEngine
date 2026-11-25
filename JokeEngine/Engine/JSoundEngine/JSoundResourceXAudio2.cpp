#include "JSoundResourceXAudio2.h"
#include "Engine/JContent/JSound.h"
#include "JSoundEngine.h"

SourceVoicePtr::SourceVoicePtr(JSound* sound)
{
	if (sound) {
		auto p =JSoundEngine::GetInstance()->GetXAudio2();
		WAVEFORMATEXTENSIBLE wf = sound->GetWaveFormatEX();
		p->CreateSourceVoice(&m_Voice, reinterpret_cast<WAVEFORMATEX*>(&wf));
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
		WAVEFORMATEXTENSIBLE wf = sound->GetWaveFormatEX();
		p->CreateSourceVoice(&m_Voice, reinterpret_cast<WAVEFORMATEX*>(&wf));
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

}

void JSoundResourceXAudio2::SetVolume()
{
}

void JSoundResourceXAudio2::Play(bool begin)
{
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

}

