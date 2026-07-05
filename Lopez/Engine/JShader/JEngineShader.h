#pragma once

#include "stdafx.h"
#include "Engine/JObject/JObject.h"


class JEngineShader {
public:
	virtual void Initialize() {}
	virtual void SetShader() {}

	virtual void RenderObjects() {}

	void SetName(const char* name) { m_name = name; }
	const std::string& GetName() { return m_name; }


	std::vector<std::shared_ptr<JObject>>& GetObjectList() { return m_Objects; }
protected:
	std::string					m_name{};

	std::vector<std::shared_ptr<JObject>> m_Objects{};
};