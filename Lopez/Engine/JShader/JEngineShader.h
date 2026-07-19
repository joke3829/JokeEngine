#pragma once

#include "Engine/JObject/JObject.h"


ComPtr<ID3DBlob> CompileHLSL(LPCWCHAR pFileName, D3D_SHADER_MACRO* pDefines, LPCSTR pEntryPoint, LPCSTR pTarget);



// Shader는 자신만의 RenderTarget을 가질 수도 있다.
class JEngineShader : public JCommon {
public:
	JEngineShader() = default;

	virtual void SetShader() {}
	virtual void SetSamplers(UINT parameter, JShaderStage stage = JS_NONE) {}


	virtual void RenderObjects() {}
	// 외부의 버퍼를 자신의 RenderTarget으로 이용
	virtual void RenderObjects(void** rtv, UINT numRTV, void* dsv = nullptr) {}


	std::vector<std::shared_ptr<JObject>>& GetObjectList() { return m_Objects; }
protected:
	// 직접 blob을 넣어 만들 수 도 있을것


	std::vector<std::shared_ptr<JObject>> m_Objects{};
};