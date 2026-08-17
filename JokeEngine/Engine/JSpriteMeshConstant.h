#pragma once
#include "stdafx.h"

class JSAnimData;


struct CB_Sprite {
	XMFLOAT4X4 uvMatrix{};		// uv변환 행렬
};


class JSpriteMeshConstant {
public:
	JSpriteMeshConstant() {};

	// 상수버퍼 값 최신화(Map)
	virtual void Update(float elapsedTime) {};
	virtual void UpdateFromData(const JSAnimData& data, bool mirror = false);
	virtual void UpdateBuffer(UINT currentFrameIndex) {}
	virtual void SetDXBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage) {};

	CB_Sprite m_CBSprite{};		// public으로 그냥 뽑아 쓰자
};