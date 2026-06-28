#include "JCameraObject.h"

JCameraObject::JCameraObject(XMFLOAT4X4* WorldMatrixByScene, const char* name)
	: JObject(WorldMatrixByScene, name)
{

}

void JCameraObject::Update(float elapsedTime, XMFLOAT4X4* parent)
{
	// 카메라 개인 로직 

	JObject::Update(elapsedTime, parent);
}


XMFLOAT4X4 JCameraObject::GetViewMatrix()
{
	// 오차가 누적될 수 있다는데 일단은 지켜보기
	XMMATRIX world = XMLoadFloat4x4(m_WorldTransform);
	XMVECTOR UP = XMVector3Normalize(world.r[1]);
	XMVECTOR LOOK = XMVector3Normalize(world.r[2]);
	XMVECTOR EYE = world.r[3];

	XMFLOAT4X4 VM;

	XMStoreFloat4x4(&VM, XMMatrixLookToLH(EYE, LOOK, UP));
	return VM;
}