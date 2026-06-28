#pragma once

#include "JContent.h"


class JStaticMesh : public JContent {
public:
	JStaticMesh(const char* name = nullptr, bool bloadFromFile = false);
	JStaticMesh(XMFLOAT3 center = XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3 extent = XMFLOAT3(5.f, 5.f, 5.f), const char* name = nullptr);
	JStaticMesh(XMFLOAT3 center = XMFLOAT3(0.f, 0.f, 0.f), float radius = 5.f, UINT sliceCount = 20, UINT stackCount = 20, const char* name = nullptr);
	JStaticMesh(XMFLOAT3 center = XMFLOAT3(0.f, 0.f, 0.f), float width = 1.f, float height = 1.f, bool bViewport = false, const char* name = nullptr);

	// 11: slot, 12: root parameter, 12는 parameter만 채우면 된다.
	virtual void SetDXBuffer(UINT parameter, JShaderStage stage = JS_NONE) {}

	virtual void Render() {}
	virtual void Render(void* command) {}


protected:
	virtual void LoadFromFile(const char* name) {}

protected:
	// cpu상에선 필요없을 수 있으나 현재는 메모리가 부족할 일은 없다 판단하기에 보류
	// 하나로 묶을까?
	std::vector<XMFLOAT3>			m_Vertices{};
	std::vector<XMFLOAT4>			m_Colors{};
	std::vector<XMFLOAT3>			m_Normals{};
	std::vector<XMFLOAT3>			m_Tangents{};
	std::vector<XMFLOAT3>			m_BiTangents{};
	std::vector<XMFLOAT2>			m_TexCoord0{};
	std::vector<XMFLOAT2>			m_TexCoord1{};

	std::vector<std::vector<UINT>>	m_vIndices{};		// submeshes
};