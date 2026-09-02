#pragma once

#include "Engine/GResource/JTextResource.h"


class JTextResourceD2D : public JTextResource {
public:

	// TextEngine 불러서 Brush각각 브러시 만들기 지원

protected:
	ComPtr<IDWriteTextFormat> m_TextFormat{};
	ComPtr<IDWriteTextLayout> m_TextLayout{};

	ComPtr<ID2D1Brush> m_Brush{};
};