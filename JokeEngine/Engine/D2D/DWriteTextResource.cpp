#include "DWriteTextResource.h"


void JTextResourceD2D::CreateDWriteTextFomat(std::wstring& fontname, std::string& fontcollection, float fontsize, float fontweight, float fontwidth)
{
	JD2DTextEngine::GetInstance()->CreateTextFormat(m_TextFormat, fontname, fontcollection, fontsize, fontweight, fontwidth);
}

void JTextResourceD2D::CreateDWriteTextLayout(std::wstring& text, float maxwidth, float maxheight)
{
	JD2DTextEngine::GetInstance()->CreateTextLayout(m_TextLayout, m_TextFormat, text, maxwidth, maxheight);
}

void JTextResourceD2D::CreateSolidColorBrush(std::string& name, XMFLOAT4 color)
{
	auto* e = JD2DTextEngine::GetInstance();
	m_Brush =  e->CreateSolidColorBrush(name, color);
}

void JTextResourceD2D::CreateSolidColorBrush(std::string& name, D2D1::ColorF color)
{
	auto* e = JD2DTextEngine::GetInstance();
	m_Brush = e->CreateSolidColorBrush(name, color);
}
