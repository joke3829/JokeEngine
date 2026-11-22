#pragma once

#include "Engine/JInputThread.h"
#include "JObject.h"

// 말이 플레이어지 사실은 키입력으로 모든 상호작용이 가능한 것을 가지고 있는 객체?
class JPlayerObject : public JObject {
public:
	JPlayerObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT uid, const char* name = nullptr);

	// Input에 따른 업데이트
	virtual void UpdateInput(JKeyState& input);

	virtual void UpdateInputWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam);
#if DEV_MODE
	virtual void SaveObjectToFile(nlohmann::json& json, std::unordered_map<UINT, UINT>& IDtable);
	virtual bool RenderInspector(UINT currentFrameIndex = 0);
#endif
protected:
	// 기능의 따른 키 설정 상태
	std::unordered_map<int, int> dmdkr;
};