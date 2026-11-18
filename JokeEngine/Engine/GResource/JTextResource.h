#pragma once

// 텍스트가 변할일이 없다면 나중엔 사이즈 받아서 rendertarget 지정하게 하는 기능을 넣어보자

#include "stdafx.h"

struct JTextMetrics {
	JTextMetrics() = default;
	JTextMetrics(float left, float top, float width, float height) : Left(left), Top(top), Width(width), Height(height) {}
	float Left{};
	float Top{};
	float Width{};
	float Height{};
};

struct JSpacingFactor {
	float LeadingSpacing{};
	float TrailingSpacing{};
	float MinimumAdvanceWidth{};
};

class JTextResource {
public:
	JTextResource(UINT uid) : m_OwnerUID{ uid } {}
	virtual ~JTextResource() {}

	virtual void Update(float elapsedTime) {};
	virtual void UpdateBuffer(UINT currentFrameIndex) {}
	virtual void SetGPUBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage = JS_NONE) {};

	virtual void CreateDWriteTextFomat(std::wstring& fontfamily, float fontsize, float fontweight, float fontwidth) {}
	virtual void CreateDWriteTextLayout(std::wstring& text, float maxwidth, float maxheight, JSpacingFactor space = {}) {}

	virtual void CreateSolidColorBrush() {}
	virtual void SetTextColor() {}

	virtual JTextMetrics GetMetrics() = 0;
	virtual JTextMetrics GetMetricsWithUVMatrixUpdate() = 0;

	virtual XMUINT2 GetRTSize() { return XMUINT2(); }
	virtual void ResizeRT(XMUINT2 size) {}
	virtual void* GetDedicatedFactor(UINT currentFrameIndex) { return nullptr; }

	virtual void SetBrushColor(XMFLOAT4 color) {}
protected:
	UINT m_OwnerUID{};
};