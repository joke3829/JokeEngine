#include "JSoundEngine.h"

JSoundEngine* JSoundEngine::m_Instance = nullptr;

JSoundEngine* JSoundEngine::GetInstance()
{
	if (not m_Instance) m_Instance = new JSoundEngine;
	return m_Instance;
}

JSoundEngine::JSoundEngine()
{
	ThrowIfFailed(XAudio2Create(m_XAudio2.ReleaseAndGetAddressOf()));
	m_XAudio2->CreateMasteringVoice(&m_XAudio2MasteringVoice);
	spdlog::info("JSoundEngine 초기화 완료");
}

JSoundEngine::~JSoundEngine()
{
	if (m_XAudio2MasteringVoice) delete m_XAudio2MasteringVoice;
	delete m_Instance;
}
