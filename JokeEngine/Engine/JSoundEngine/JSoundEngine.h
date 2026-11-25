#pragma once

#include "stdafx.h"

class JSoundEngine {
public:
	static JSoundEngine* GetInstance();

	IXAudio2* GetXAudio2() { return m_XAudio2.Get(); }
protected:
	static JSoundEngine* m_Instance;

	JSoundEngine();
	~JSoundEngine();
protected:
	ComPtr<IXAudio2>		m_XAudio2{};
	IXAudio2MasteringVoice* m_XAudio2MasteringVoice{};
};