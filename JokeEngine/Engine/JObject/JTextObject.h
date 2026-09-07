#pragma once

#include "JObject.h"

class JStaticMesh;

enum JText_Alignment : unsigned char {
	Left,
	Center,
	Right,
	Top,
	Bottom,
	Count
};

class JTextObject : public JObject {
public:
	JTextObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name = nullptr);

	void SetText(wchar_t* text) { m_Text = text; MakeDirtyFlag(); }

	void MakeDirtyFlag();
protected:

	XMFLOAT3 m_TextScale{};
	XMFLOAT3 m_MeshScale{};

	std::wstring m_Text{};
	float m_FontSize{};

	// 둘 중 하나가 center면 둘다 center
	JText_Alignment m_HorizonAlignment = JText_Alignment::Left;	
	JText_Alignment m_VerticalAlignment = JText_Alignment::Top;

	// staticMesh 5개(plane 방향별 5개를 미리 세팅 해 놓고 alignment에 따라 바꿔 사용)
	std::vector<std::shared_ptr<JStaticMesh>> m_PlaneMeshes{};

	bool m_Dirty[kNumRenderTarget]{};
};