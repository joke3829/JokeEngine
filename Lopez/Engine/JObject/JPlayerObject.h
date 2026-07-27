#pragma once

#include "JObject.h"

class JPlayerObject : public JObject {
public:
	JPlayerObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name = nullptr);


};