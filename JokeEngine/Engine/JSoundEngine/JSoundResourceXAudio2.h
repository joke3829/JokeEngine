#pragma once
#include "JSoundResourceInterface.h"

class SourceVoicePtr {
public:
	SourceVoicePtr(JSound* sound);
	~SourceVoicePtr();

	void ReMakeVoice(JSound* sound);
	IXAudio2SourceVoice* m_Voice;
};

class JSoundResourceXAudio2 : public JSoundResource {
public:
	JSoundResourceXAudio2();

	virtual void SetSound(JContent* sound);
	virtual void SetVolume();
	virtual void Play(bool begin = false);
	virtual void Pause();
	virtual void Stop();

	virtual void SetSubMixGroup(std::string& submixname);
	virtual void SetMultiSource(UINT num);
protected:
	JSound*						m_Sound{};
	std::vector<SourceVoicePtr> m_SourceVoices{};

	std::string					m_SubMixGroupName{};
};