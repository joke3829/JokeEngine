#pragma once

#include "Engine/JObject/JObject.h"


ComPtr<ID3DBlob> CompileHLSL(LPCWCHAR pFileName, D3D_SHADER_MACRO* pDefines, LPCSTR pEntryPoint, LPCSTR pTarget);



// Shader는 자신만의 RenderTarget을 가질 수도 있다.
class JEngineShader : public JCommon {
public:
	JEngineShader(const char* name = "DefaultShader") { m_name = name; }
	virtual void ReCompileShader() {}


	virtual void SetShader() {}
	virtual void SetPSOName(const char* name) {}
	virtual void SetSamplers(UINT parameter, JShaderStage stage = JS_NONE) {}

	bool empty() { return m_Objects.empty(); }

	virtual void RenderObjects() {}
	// 외부의 버퍼를 자신의 RenderTarget으로 이용
	virtual void RenderObjects(UINT currnetFrameIndex, void** rtv, UINT numRTV, void* dsv = nullptr) {}

	void AddObject(std::shared_ptr<JObject>& object) { m_Objects.emplace_back(object); }
	void ClearObject() { m_Objects.clear(); }
protected:
	// 직접 blob을 넣어 만들 수 도 있을것


	std::vector<std::shared_ptr<JObject>> m_Objects{};
};