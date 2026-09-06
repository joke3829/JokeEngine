#include "DWriteTextResource.h"


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
