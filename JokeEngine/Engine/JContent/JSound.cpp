#include "JSound.h"
#include "Engine/JSoundEngine/SoundFileImporter.h"

void JSound::LoadFromFile()
{
	auto ext = m_FullFilePath.extension();
	if (".wav" == ext) {
		if (not WavTool::Import(m_FullFilePath, m_RawAudioDataSize, m_RawAudioData, m_WaveFormat))
			return;
	}
}
