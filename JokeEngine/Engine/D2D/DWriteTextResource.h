#pragma once

#include "Engine/GResource/JTextResource.h"
#include "JD2DTextEngine.h"

constexpr UINT kTextTexture2DSize = 256;

class JTextResourceD2D : public JTextResource {
public:

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
	// 상속 받은 DX12, DX11 생성자에서 만듬 -> 여기 생성자로 하는걸로 바꾸기 고려
	std::string m_ResourceNames[kNumRenderTarget]{};
};