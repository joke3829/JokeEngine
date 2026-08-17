#include "JSpriteMeshConstant.h"
#include "Engine/JContent/JSpriteAnimationSet.h"

void JSpriteMeshConstant::UpdateFromData(const JSAnimData& data, bool mirror)
{
	// 1. 스프라이트 1개 셀이 전체 UV(0~1)에서 차지하는 비율(Scale) 계산
	float scaleX = data.perWidth / data.ImageWidth;
	float scaleY = data.perHeight / data.ImageHeight;

	// 2. 행/열 위치에 따른 기본 UV 시작 오프셋(Translation) 계산
	float offsetX = data.col * scaleX;
	float offsetY = data.row * scaleY;

	XMMATRIX transformMatrix;

	if (mirror)
	{
		// [좌우 반전 시]
		// X축 스케일을 - scaleX 로 뒤집고, 
		// 오프셋을 col 위치가 아닌 (col + 1) 셀의 우측 끝 지점으로 이동시킵니다.
		XMMATRIX scaleMat = XMMatrixScaling(-scaleX, scaleY, 1.0f);
		XMMATRIX transMat = XMMatrixTranslation(offsetX + scaleX, offsetY, 0.0f);

		transformMatrix = scaleMat * transMat;
	}
	else
	{
		// [일반 상태]
		XMMATRIX scaleMat = XMMatrixScaling(scaleX, scaleY, 1.0f);
		XMMATRIX transMat = XMMatrixTranslation(offsetX, offsetY, 0.0f);

		transformMatrix = scaleMat * transMat;
	}

	XMStoreFloat4x4(&m_CBSprite.uvMatrix, XMMatrixTranspose(transformMatrix));
}
