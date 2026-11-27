#include "SoundFileImporter.h"

bool WavTool::Import(std::filesystem::path filepath, uint32_t& datasize, std::unique_ptr<BYTE[]>& rawData, std::unique_ptr<BYTE[]>& waveformat)
{
	std::ifstream inFile{ filepath, std::ios::binary };
	if (not inFile) {
		return false;
	}

	auto conptr = [](void* ptr) {
		return reinterpret_cast<char*>(ptr);
		};

	{ // RIFF
		RIFFChunk riff{};
		inFile.read(conptr(&riff), sizeof(RIFFChunk));
		if (std::memcmp("RIFF", riff.id, 4)) {
			return false;
		}
		if (std::memcmp("WAVE", riff.wave, 4)) {
			return false;
		}
	}
	// RIFF 통과
	char8_t chunkID[4]{};
	uint32_t remainSize{};
	bool inFmt{};
	bool inData{};
	bool data64{};
	

	while (inFile.read(conptr(chunkID), sizeof(char8_t) * 4)) {
		inFile.read(conptr(&remainSize), sizeof(uint32_t));
		if (not std::memcmp("fmt ", chunkID, 4)) {
			if (not WavTool::VaildFmtSize.contains(remainSize)) return false;
			waveformat.reset();
			waveformat = std::make_unique<BYTE[]>(remainSize < 18 ? 18 : remainSize);
			inFile.read(conptr(waveformat.get()), remainSize);

			WAVEFORMATEXTENSIBLE* temp = reinterpret_cast<WAVEFORMATEXTENSIBLE*>(waveformat.get());
			
			// IEEE일 때 64bit면 처리
			if (temp->Format.wFormatTag == 3 && temp->Format.wBitsPerSample == 64) {
				data64 = true;
				temp->Format.wBitsPerSample = 32;
				temp->Format.nBlockAlign = temp->Format.nChannels * 4;
				temp->Format.nAvgBytesPerSec = temp->Format.nSamplesPerSec * temp->Format.nBlockAlign;
				if (temp->Format.cbSize >= 22) {
					temp->Samples.wValidBitsPerSample = 32;
				}
			}

			// law 들 제외
			if(temp->Format.wFormatTag == 6 || temp->Format.wFormatTag == 7) return false;

			// Extensible에 law인것 제외
			if (temp->Format.wFormatTag == 65534 && temp->Format.cbSize >= 22) {
				if (temp->SubFormat.Data1 == 6 || temp->SubFormat.Data1 == 7) return false;
				else if (temp->SubFormat.Data1 == 3 && temp->Format.wBitsPerSample == 64) {
					data64 = true;
					temp->Format.wBitsPerSample = 32;
					temp->Format.nBlockAlign = temp->Format.nChannels * 4;
					temp->Format.nAvgBytesPerSec = temp->Format.nSamplesPerSec * temp->Format.nBlockAlign;
					if (temp->Format.cbSize >= 22) {
						temp->Samples.wValidBitsPerSample = 32;
					}
				}
			}

			inFmt = true;
		}
		else if (not std::memcmp("data", chunkID, 4)) {
			rawData.reset();
			datasize = remainSize;
			rawData = std::make_unique<BYTE[]>(data64 ? remainSize / 2 : remainSize);
			if (data64) {
				std::vector<double> dd(remainSize/8);
				std::vector<float> ff(remainSize/8);
				inFile.read(conptr(dd.data()), sizeof(double) * dd.size());
				for (int i = 0; i < ff.size(); ++i) {
					ff[i] = static_cast<float>(dd[i]);
				}
				memcpy(rawData.get(), ff.data(), sizeof(uint32_t) * ff.size());
			}
			else
				inFile.read(conptr(rawData.get()), remainSize);
			inData = true;
		}
		else {
			// remainSIze만큼 건너뛰기
			inFile.seekg(remainSize + (remainSize & 1), std::ios::cur);
		}
	}

	// fmt 또는 data가 없음
	if (not inData || not inFmt) return false;

	return true;
}
