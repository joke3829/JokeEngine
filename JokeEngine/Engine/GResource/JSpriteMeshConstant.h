#pragma once
#include "stdafx.h"

class JSAnimData;


struct CB_Sprite {
	bool operator==(const CB_Sprite& other) const {
		return !static_cast<bool>(memcmp(&uvMatrix, &other.uvMatrix, sizeof(XMFLOAT4X4)));
	}

	XMFLOAT4X4 uvMatrix{};		// uv변환 행렬
};


class JSpriteMeshConstant {
public:
	JSpriteMeshConstant() {};

	// 상수버퍼 값 최신화(Map)
	virtual void Update(float elapsedTime) {};
	virtual void UpdateFromData(const JSAnimData& data, bool mirror = false);
	virtual void UpdateBuffer(UINT currentFrameIndex) {}
	virtual void SetDXBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage = JS_NONE) {};

	CB_Sprite m_CBSprite{};		// public으로 그냥 뽑아 쓰자
	CB_Sprite m_CurrentSetCB[g_NumRenderTarget]{};	// 현재 GPU에 Set된 값
};