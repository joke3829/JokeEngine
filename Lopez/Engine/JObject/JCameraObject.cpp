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
	XMMATRIX world = XMLoadFloat4x4(m_WorldTransform);
	XMVECTOR UP = XMVector3Normalize(world.r[1]);
	XMVECTOR LOOK = XMVector3Normalize(world.r[2]);

}