#pragma once
#include "JObject.h"

// DX11, 12에 구애받지 않는 viewport(11이랑 12랑 구조 똑같음, ScissorRect도 필요하면 추가를 고려하자)
struct common_viewport {
	common_viewport() = default;
	common_viewport(float tx, float ty, float w, float h, float minD, float maxD)
	{
		TopLeftX = tx; TopLeftY = ty; Width = w; Height = h; MinDepth = minD; MaxDepth = maxD;
	}
	FLOAT TopLeftX;
	FLOAT TopLeftY;
	FLOAT Width;
	FLOAT Height;
	FLOAT MinDepth;
	FLOAT MaxDepth;
};

struct JProjFactor {
	bool bOrtho = false;
	float Fov{};		// degree
	float Aspect{};
	float Near{};
	float Far{};

	// Orthographic Factor
	float Left{};
	float Right{};
	float Bottom{};
	float Top{};
};


// Active 여부로 멀티 뷰포트 여부 결정
class JCameraObject : public JObject {
public:
	JCameraObject(std::vector<XMFLOAT4X4>& vWorld, UINT nodeIndex, const char* name = nullptr);
	virtual ~JCameraObject() {}

	virtual void Update(float elapsedTime, XMFLOAT4X4* parent = nullptr);

public:
	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjMatrix();
	XMFLOAT4X4 GetViewProjMatrix();

	void SetViewport(float TopLeftX, float TopLeftY, float Width, float Height, float MinDepth, float MaxDepth) { m_Viewport = common_viewport{ TopLeftX, TopLeftY, Width, Height, MinDepth, MaxDepth }; }
	void SetViewport(common_viewport vp) { m_Viewport = vp; }
	const common_viewport& GetViewport() { return m_Viewport; }



	void SetProjFactorPerspective(float Fov, float Aspect, float Near, float Far);
	void SetProjFactorOrthographic(float Left, float Right, float Bottom, float Top, float Near, float Far);
	void SetProjFactor(JProjFactor factor) { m_ProjFactor = factor; }
	const JProjFactor& GetProjFactor() { return m_ProjFactor; }

protected:
	common_viewport						m_Viewport{};
	JProjFactor							m_ProjFactor{};
};