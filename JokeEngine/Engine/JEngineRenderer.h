#pragma once

#include "stdafx.h"
#include "JEngineShader.h"
#include "JEngineScene.h"

class JEngineRenderer {
public:
	virtual void Initialize() {}
	void SetScene(std::shared_ptr<JEngineScene> scene) { m_Scene = scene; }


	virtual void ResizeTarget(UINT width, UINT height) {}

	virtual void RenderFrame() {}

	virtual void CopyResult(void* outBuffer) {}

protected:
	std::string															m_name{};
	
	std::shared_ptr<JEngineScene>										m_Scene{};

	std::unordered_map<std::string, std::shared_ptr<JEngineShader>>		m_PSOMap{};
	//렌더 타겟
};