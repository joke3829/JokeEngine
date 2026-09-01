#pragma once
// MeshConstant.h ===========================================================
// 
// MeshObject(Static, Skeletal)가 가져야 하는 상수버퍼를 정의
// DX11, DX12 버젼으로 각각 나늬며 Base를 이곳에 정의한다.
// 
// ==========================================================================

#include "stdafx.h"


struct CB_Mesh {
	bool operator==(const CB_Mesh& other) const {
		if (nodeIndex != other.nodeIndex) return false;
		return !static_cast<bool>(memcmp(BoneLookupTable, other.BoneLookupTable, sizeof(BoneLookupTable)));
	}
	UINT nodeIndex{};
	UINT bSkinning{};		// hlsl은 bool도 4byte로 본다. 이건 아마 바뀔일이 없겠지?
	XMFLOAT2 padding{};
	UINT BoneLookupTable[256]{};	// 일단은 
};


class JMeshConstant {
public:
	JMeshConstant() {};
	JMeshConstant(CB_Mesh cb) : m_CBMesh{ cb } {};


	// 상수버퍼 값 최신화(Map)
	virtual void Update(float elapsedTime) {};
	virtual void UpdateBuffer(UINT currentFrameIndex) {}
	virtual void SetDXBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage = JS_NONE) {};

	CB_Mesh m_CBMesh{};									// public으로 그냥 뽑아 쓰자
	CB_Mesh m_CurrentSetCB[g_NumRenderTarget]{};		// 현재 GPU에 Set된 값
};