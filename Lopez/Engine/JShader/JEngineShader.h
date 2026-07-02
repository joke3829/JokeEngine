#pragma once

#include "stdafx.h"


class JEngineShader {
public:
	virtual void Initialize() {}
	virtual void SetShader() {}

	virtual void RenderObjects() {}

	void SetName(const char* name) { m_name = name; }
protected:
	std::string					m_name{};


};