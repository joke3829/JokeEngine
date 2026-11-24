#include "JSound.h"

JSound::~JSound()
{
	if (m_SourceVoice) m_SourceVoice->DestroyVoice();
}
