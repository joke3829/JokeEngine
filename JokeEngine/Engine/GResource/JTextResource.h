#pragma once

#include "stdafx.h"

class JTextResource {
public:

	virtual void Update(float elapsedTime) {};
	virtual void UpdateBuffer(UINT currentFrameIndex) {}
	virtual void SetGPUBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage = JS_NONE) {};

	virtual void CreateSolidBrush() {}
	virtual void SetTextColor() {}

	void SetName(const char* name) { m_name = name; }
	const std::string& GetName() { return m_name; }
	
protected:
	std::string m_name{};
};