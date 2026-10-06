#include "DWriteTextResource.h"


void JTextResourceD2D::UpdateBuffer(UINT currentFrameIndex)
{
	auto* p = JD2DTextEngine::GetInstance();
	p->RenderText(m_ResourceNames[currentFrameIndex], m_TextLayout.Get(), m_Brush[currentFrameIndex].Get());
}

void JTextResourceD2D::CreateDWriteTextFomat(std::wstring& fontname, std::string& fontcollection, float fontsize, float fontweight, float fontwidth)
{
	JD2DTextEngine::GetInstance()->CreateTextFormat(m_TextFormat, fontname, fontcollection, fontsize, fontweight, fontwidth);
}

void JTextResourceD2D::CreateDWriteTextLayout(std::wstring& text, float maxwidth, float maxheight)
{
	JD2DTextEngine::GetInstance()->CreateTextLayout(m_TextLayout, m_TextFormat, text, maxwidth, maxheight);
}

void JTextResourceD2D::CreateSolidColorBrush(XMFLOAT4 color)
{
	auto* e = JD2DTextEngine::GetInstance();
	for(int i = 0 ; i < kNumRenderTarget; ++i)
		m_Brush[i] = e->CreateSolidColorBrush(m_ResourceNames[i], color);
}

void JTextResourceD2D::CreateSolidColorBrush(D2D1::ColorF color)
{
	auto* e = JD2DTextEngine::GetInstance();
	for (int i = 0; i < kNumRenderTarget; ++i)
		m_Brush[i] = e->CreateSolidColorBrush(m_ResourceNames[i], color);
}

JTextMetrics JTextResourceD2D::GetMetrics()
{
	DWRITE_TEXT_METRICS met{};
	m_TextLayout->GetMetrics(&met);
	return JTextMetrics(met.left, met.top, met.width, met.height);
}
