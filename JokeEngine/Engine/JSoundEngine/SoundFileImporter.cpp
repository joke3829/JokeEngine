#include "SoundFileImporter.h"

bool WavTool::Import(std::filesystem::path filepath, uint32_t& datasize, std::unique_ptr<BYTE[]>& rawData, std::unique_ptr<BYTE[]>& waveformat)
{
	std::ifstream inFile{ filepath, std::ios::binary };
	if (not inFile) {
#if DEV_MODE
		spdlog::error("{} 파일이 존재하지 않아 Load에 실패했습니다.", filepath.string());
#endif
		return false;
	}

	auto conptr = [](void* ptr) {
		return reinterpret_cast<char*>(ptr);
		};

	{ // RIFF
		RIFFChunk riff{};
		inFile.read(conptr(&riff), sizeof(RIFFChunk));
		if (std::memcmp("RIFF", riff.id, 4)) {
#if DEV_MODE
			spdlog::error("{} 잘못된 .wav 파일로 Load에 실패했습니다.", filepath.string());
#endif
			return false;
		}
		if (std::memcmp("WAVE", riff.wave, 4)) {
#if DEV_MODE
			spdlog::error("{} 잘못된 .wav 파일로 Load에 실패했습니다.", filepath.string());
#endif
			return false;
		}
	}
	// RIFF 통과
	char8_t chunkID[4]{};
	uint32_t remainSize{};

	while (inFile.read(conptr(chunkID), sizeof(char8_t) * 4)) {
		inFile.read(conptr(&remainSize), sizeof(uint32_t));
		if (not std::memcmp("fmt ", chunkID, 4)) {
			waveformat.reset();
			waveformat = std::make_unique<BYTE[]>(remainSize < 18 ? 18 : remainSize);
			inFile.read(conptr(waveformat.get()), remainSize);
		}
		else if (not std::memcmp("data", chunkID, 4)) {
			rawData.reset();
			datasize = remainSize;
			rawData = std::make_unique<BYTE[]>(remainSize);
			inFile.read(conptr(rawData.get()), remainSize);
		}
		else {
			// remainSIze만큼 건너뛰기
			inFile.seekg(remainSize + (remainSize & 1), std::ios::cur);
		}
	}



	return true;
}
