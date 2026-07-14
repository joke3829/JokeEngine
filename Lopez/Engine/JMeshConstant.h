#pragma once
// MeshConstant.h ===========================================================
// 
// MeshObject(Static, Skeletal)가 가져야 하는 상수버퍼를 정의
// DX11, DX12 버젼으로 각각 나늬며 Base를 이곳에 정의한다.
// 
// ==========================================================================

#include "stdafx.h"


struct CB_Mesh {
	UINT nodeIndex{};
	UINT bSkinning{};		// hlsl은 bool도 4byte로 본다. 
	XMFLOAT2 padding{};
	UINT BoneLookupTable[256]{};	// 일단은 
};


class JMeshConstant {
public:
	JMeshConstant() {};
	JMeshConstant(CB_Mesh cb) : m_CBMesh{ cb } {};

	// 바꾸면 Dirty를 업데이트 하는 그런거 있으면 좋음

	// 상수버퍼 값 최신화(Map)
	virtual void Update() {};
	virtual void SetDXBuffer(UINT parameter, JShaderStage stage) {};

	CB_Mesh m_CBMesh{};		// public으로 그냥 뽑아 쓰자
	bool	m_Dirty{};		// 항상 업데이트하지마라
};