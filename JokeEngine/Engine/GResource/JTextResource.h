#pragma once

#include "stdafx.h"

struct JTextMetrics {
	JTextMetrics(float left, float top, float width, float height) : Left{ left }, Top{ top }, Width{ width }, Height{ height } {}
	float Left{};
	float Top{};
	float Width{};
	float Height{};
};

class JTextResource {
public:

	virtual void Update(float elapsedTime) {};
	virtual void UpdateBuffer(UINT currentFrameIndex) {}
	virtual void SetGPUBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage = JS_NONE) {};

	virtual void CreateDWriteTextFomat(std::wstring& fontname, std::string& fontcollection, float fontsize, float fontweight, float fontwidth) {}
	virtual void CreateDWriteTextLayout(std::wstring& text, float maxwidth, float maxheight) {}

	virtual void CreateSolidBrush(std::string& name, XMFLOAT4 color = XMFLOAT4(1.f, 1.f, 1.f, 1.f)) {}
	virtual void CreateSolidColorBrush(std::string& name, D2D1::ColorF color = D2D1::ColorF::White) {}
	virtual void SetTextColor() {}

	void SetName(const char* name) { m_name = name; }
	virtual JTextMetrics GetMetrics() = 0;

	const std::string& GetName() { return m_name; }
	
protected:
	std::string m_name{};
};