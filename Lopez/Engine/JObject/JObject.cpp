#include "JObject.h"

JObject::JObject(XMFLOAT4X4* WorldMatrixByScene, const char* name)
	: m_WorldTransform{ WorldMatrixByScene }
{
	if (name) m_name = name;
	else m_name = "JObject";

	if (!WorldMatrixByScene) {
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("'{0}' Not Aquired WorldMatrixByScene", m_name);
#endif
		assert(0);
	}

	XMStoreFloat4x4(&m_LocalTransform, XMMatrixIdentity());
}

void JObject::Update(float elapsedTime, XMFLOAT4X4* parent)
{
	MakeLocalTransform();

	XMMATRIX parentMatrix;

	if (parent) parentMatrix = XMLoadFloat4x4(parent);
	else parentMatrix = XMMatrixIdentity();

	XMStoreFloat4x4(m_WorldTransform, XMLoadFloat4x4(&m_LocalTransform) * parentMatrix);

	for (auto& child : m_LeafObjects) {
		child->Update(elapsedTime, m_WorldTransform);
	}
}

void JObject::MakeLocalTransform()
{
	XMVECTOR S, R, T;
	S = XMLoadFloat3(&m_Scale);
	T = XMLoadFloat3(&m_Position);
	R = XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(m_Rotation.x), 
		XMConvertToRadians(m_Rotation.y), 
		XMConvertToRadians(m_Rotation.z)
	);
	XMStoreFloat4x4(&m_LocalTransform, XMMatrixAffineTransformation(S, g_XMZero, R, T));
}