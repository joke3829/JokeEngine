#pragma once

#include "stdafx.h"

class JSoundEngine {
public:
	static JSoundEngine* GetInstance();

protected:
	static JSoundEngine* m_Instance;

	JSoundEngine();
	~JSoundEngine();
protected:
	ComPtr<IXAudio2>		m_XAudio2{};
	IXAudio2MasteringVoice* m_XAudio2MasteringVoice{};
};