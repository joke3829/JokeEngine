#pragma once

#include "Engine/GResource/JTextResource.h"
#include "JD2DTextEngine.h"

// fontSize 32기준 줄3 열60
constexpr UINT kTextTexture2DWidth = 1920;
constexpr UINT kTextTexture2DHeight = 96;

class JTextResourceD2D : public JTextResource {
public:
	JTextResourceD2D(UINT uid) : JTextResource(uid) {}
	virtual ~JTextResourceD2D();

	virtual void Update(float elapsedTime) {};
	virtual void UpdateBuffer(UINT currentFrameIndex);
	virtual void SetGPUBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage = JS_NONE) {};

	void CreateDWriteTextFomat(std::wstring& text, std::string& fontcollection, float fontsize, float fontweight, float fontwidth);
	void CreateDWriteTextLayout(std::wstring& text, float maxwidth, float maxheight);

	// TextEngine 불러서 Brush각각 브러시 만들기 지원
	void CreateSolidColorBrush(XMFLOAT4 color = XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	void CreateSolidColorBrush(D2D1::ColorF color = D2D1::ColorF::White);

	JTextMetrics GetMetrics();

protected:
	ComPtr<IDWriteTextFormat3> m_TextFormat{};
	ComPtr<IDWriteTextLayout> m_TextLayout{};

	ComPtr<ID2D1Brush> m_Brush[kNumRenderTarget]{};
};