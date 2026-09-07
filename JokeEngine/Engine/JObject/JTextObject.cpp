#include "JTextObject.h"

JTextObject::JTextObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name)
	: JObject(vWorld, vWorldTP, nodeIndex, name)
{
	MakeDirtyFlag();
}

void JTextObject::MakeDirtyFlag()
{
	for (UINT i = 0; i < kNumRenderTarget; ++i)
		m_Dirty[i] = true;
}
