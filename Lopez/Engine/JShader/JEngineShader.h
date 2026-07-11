#pragma once

#include "Engine/JObject/JObject.h"


class JEngineShader : public JCommon {
public:
	virtual void Initialize() {}
	virtual void SetShader() {}

	virtual void RenderObjects() {}


	std::vector<std::shared_ptr<JObject>>& GetObjectList() { return m_Objects; }
protected:

	std::vector<std::shared_ptr<JObject>> m_Objects{};
};