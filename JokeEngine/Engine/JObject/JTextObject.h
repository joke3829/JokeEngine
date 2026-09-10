#pragma once

#include "JObject.h"

class JStaticMesh;
class JTextResource;

enum JText_Alignment : unsigned char {
	Left,
	Center,
	Right,
	Top,
	Bottom,
	Count
};

// 1*1 plane을 사용하기 때문에 scale 행렬을 조작해야하는건 맞다만, 이게 위치상으로 옳은가?
// 이거는 렌더링용 행렬이랑 실제 위치용 행렬을 따로 둘 수 있나?

class JTextObject : public JObject {
public:
	JTextObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name = nullptr);
	~JTextObject();


	void Update(float elapsedTime, XMFLOAT4X4* parent = nullptr);
	void Render(UINT currentFrameIndex);

public:
	void SetText(wchar_t* text) { m_Text = text; MakeDirtyFlag(); }
	void SetFontSize(float size);
	void SetFontCollectionName(const char* collectionname) { m_FontCollectionName = collectionname; }
	void SetFontWeight(float weight);
	void SetFontWidth(float width);


	void MakeDirtyFlag();


protected:


	std::wstring m_Text;
	std::wstring m_FontName;
	std::string m_FontCollectionName{};
	float m_FontSize;
	float m_FontWeight{ 400.f };		// 폰트 두께 (1~1000, 기본값 400)
	float m_FontWidth{ 100.f };			// 폰트 너비(간격, > 0, 기본값 100)


	// 둘 중 하나가 center면 둘다 center
	JText_Alignment m_HorizonAlignment = JText_Alignment::Left;	
	JText_Alignment m_VerticalAlignment = JText_Alignment::Top;

protected:
	std::unique_ptr<JTextResource> m_TextResource;

	XMFLOAT3 m_TextScale{};
	XMFLOAT3 m_MeshScale{};

	// staticMesh 5개(plane 방향별 5개를 미리 세팅 해 놓고 alignment에 따라 바꿔 사용)
	std::vector<std::shared_ptr<JStaticMesh>> m_PlaneMeshes{};

	bool m_GPUDirty[kNumRenderTarget]{};
	bool m_CPUDirty{};
};