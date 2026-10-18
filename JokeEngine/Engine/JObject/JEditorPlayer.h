#include "stdafx.h"

#if DEV_MODE
#include "JPlayerObject.h"
class JEditorPlayerObject final : public JPlayerObject {
public:
	JEditorPlayerObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name = nullptr);

	void UpdateInput(JKeyState& input);
	void UpdateInputWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	float m_MoveSpeed{ 20.f };
	float m_MouseRotSpeed{ 70.f };
};
#endif