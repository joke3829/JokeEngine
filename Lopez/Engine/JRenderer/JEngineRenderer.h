#pragma once

#include "Engine/JShader/JEngineShader.h"
#include "Engine/JScene/JEngineScene.h"

class JEngineRenderer : public JCommon {
public:
	virtual void Initialize(UINT width, UINT height);

	virtual void ResizeTarget(UINT width, UINT height);

	virtual void RenderFrame() {}

	virtual void CopyResult(void* outBuffer) {}

	virtual void AdvanceBufferIndex() {}

	void SetScene(std::shared_ptr<JEngineScene> scene) { m_Scene = scene; }
protected:
	virtual void CreateRTV_DSV(UINT width, UINT height) {}

protected:

	std::shared_ptr<JEngineScene>										m_Scene{};

	std::unordered_map<std::string, std::shared_ptr<JEngineShader>>		m_PSOMap{};
	//렌더 타겟

	// 지금 그릴 버퍼
	UINT																m_CurrentBufferIndex{};
};