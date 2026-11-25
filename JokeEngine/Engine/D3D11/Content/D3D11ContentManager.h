#pragma once

#include "Engine/JContent/JContentManager.h"

class JContentManagerDX11 final : public JContentManager {
public:
	bool AddContentFromFile(std::filesystem::path filepath);
#if DEV_MODE
public:
	void AddMeshWithFlag(BYTE& flag, std::filesystem::path& filepath);
protected:
	struct MeshCreateFactor {
		XMFLOAT3 center{};
		XMFLOAT3 extent{5.f, 5.f, 5.f};
	}meshCreateFactor;
#endif
private:
	
};