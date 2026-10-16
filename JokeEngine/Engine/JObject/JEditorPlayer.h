#include "stdafx.h"

#if DEV_MODE
#include "JPlayerObject.h"
class JEditorPlayerObject : public JPlayerObject {
public:
	JEditorPlayerObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name = nullptr);

	void UpdateInput(JKeyState& input);
};
#endif