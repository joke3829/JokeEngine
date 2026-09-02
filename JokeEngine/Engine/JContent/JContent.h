#pragma once

#include "Engine/JCommon.h"

// name은 사실상 filepath

class JContent : public JCommon {
public:
	JContent(const char* name = nullptr);

	// 11: slot, 12: root parameter, 12는 parameter만 채우면 된다.
	virtual void SetGPUBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage = JS_NONE) {}

	virtual void Update(float elapsedTime) {}
	virtual void UpdateBuffer(UINT currentFrameIndex) {}

	virtual void Render(UINT currentFrameIndex) {}

protected:
	// filepath로 처리하는거 안될 수 있다.(수정 필요)
	virtual void LoadFromFile(const char* name) {}
};