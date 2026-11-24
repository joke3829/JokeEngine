// ========================================
//
// Interface 클래스를 제공하는 .h
// 다음을 상속 받아 될 수 있으면 static 함수로 만들자
// 
// Content가 Audio Data를 가지고 있어야 + SourceVoice를 가지고 있어야 편하다.
// 
// ========================================

#pragma once
#include "stdafx.h"

class JSoundFileImporter {
public:
	// 뭐가 필요하냐?
	// 읽고 WAVEFORMATEX, WAVEFORMATEXTENSION을 줘야함, 그리고 pAudioData가 살아있어야함
	static WAVEFORMATEX ImportAudioFile(std::filesystem::path filepath, std::unique_ptr<BYTE[]>& audioData) {}
};