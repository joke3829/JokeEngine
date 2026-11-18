#pragma once

#include "Engine/GResource/JTextResource.h"
#include "JD2DTextEngine.h"


class JTextResourceD2D : public JTextResource {
public:
	JTextResourceD2D(UINT uid) : JTextResource(uid) {}
	virtual ~JTextResourceD2D();

	virtual void Update(float elapsedTime) {};
	virtual void UpdateBuffer(UINT currentFrameIndex);
	virtual void SetGPUBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage = JS_NONE) {};

	void CreateDWriteTextFomat(std::wstring& text, float fontsize, float fontweight, float fontwidth);
	void CreateDWriteTextLayout(std::wstring& text, float maxwidth, float maxheight, JSpacingFactor space = {});

	// TextEngine 불러서 Brush각각 브러시 만들기 지원
	void CreateSolidColorBrush();

	JTextMetrics GetMetrics();

	XMUINT2 GetRTSize() { return m_RTSize; }
	virtual void ResizeRT(XMUINT2 size) { m_RTSize = size; }

	virtual void SetBrushColor(XMFLOAT4 color) { m_BrushColor = color; }

protected:
	XMUINT2					m_RTSize{ 1920, 96 };

	ComPtr<IDWriteTextFormat3> m_TextFormat{};
	ComPtr<IDWriteTextLayout4> m_TextLayout{};

	ComPtr<ID2D1Brush> m_Brush[kNumRenderTarget]{};
	XMFLOAT4 m_BrushColor{};
};