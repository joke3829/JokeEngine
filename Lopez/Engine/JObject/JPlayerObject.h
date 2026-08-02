#pragma once

#include "Engine/JInputThread.h"
#include "JObject.h"

// 말이 플레이어지 사실은 키입력으로 모든 상호작용이 가능한 것을 가지고 있는 객체?
class JPlayerObject : public JObject {
public:
	JPlayerObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name = nullptr);

	// Input에 따른 업데이트
	virtual void UpdateInput(JKeyState& input);
protected:
	// 기능의 따른 키 설정 상태
	std::unordered_map<int, int> dmdkr;
};