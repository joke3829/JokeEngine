#pragma once

#include "Engine/GResource/JTextResource.h"
#include "JD2DTextEngine.h"

class JTextResourceD2D : public JTextResource {
public:

	// TextEngine 불러서 Brush각각 브러시 만들기 지원
	void CreateSolidColorBrush(std::string& name, XMFLOAT4 color = XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	void CreateSolidColorBrush(std::string& name, D2D1::ColorF color = D2D1::ColorF::White);

protected:
	ComPtr<IDWriteTextFormat> m_TextFormat{};
	ComPtr<IDWriteTextLayout> m_TextLayout{};

	ComPtr<ID2D1Brush> m_Brush{};
};