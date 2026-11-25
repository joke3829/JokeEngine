// ========================================
// SoundFileImporter.h
// 파일 포맷에 맞는 namespace를 선택해 부른다.
// ========================================

#pragma once
#include "stdafx.h"

namespace WavTool {
#pragma pack(1)
	struct RIFFChunk {
		char8_t id[4]{};
		uint32_t cbsize{};
		char8_t wave[4]{};
	};
#pragma pop
	bool Import(std::filesystem::path filepath, uint32_t& datasize, std::unique_ptr<BYTE[]>& rawData, std::unique_ptr<BYTE[]>& waveformat);
}