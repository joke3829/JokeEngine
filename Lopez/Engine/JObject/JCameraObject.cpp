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

XMFLOAT4X4 JCameraObject::GetProjMatrix()
{
	XMFLOAT4X4 PM{};

	if (m_ProjFactor.bOrtho)
		XMStoreFloat4x4(&PM, XMMatrixOrthographicOffCenterLH(
			m_ProjFactor.Left,
			m_ProjFactor.Right,
			m_ProjFactor.Bottom,
			m_ProjFactor.Top,
			m_ProjFactor.Near,
			m_ProjFactor.Far
		));
	else
		XMStoreFloat4x4(&PM, XMMatrixPerspectiveFovLH(
			XMConvertToRadians(m_ProjFactor.Fov),
			m_ProjFactor.Aspect,
			m_ProjFactor.Near,
			m_ProjFactor.Far
		));

	return PM;
}

XMFLOAT4X4 JCameraObject::GetViewProjMatrix()
{
	XMFLOAT4X4 VM = GetViewMatrix();
	XMFLOAT4X4 PM = GetProjMatrix();
	XMFLOAT4X4 VPM{};
	XMStoreFloat4x4(&VPM, XMLoadFloat4x4(&VM) * XMLoadFloat4x4(&PM));

	return VPM;
}


void JCameraObject::SetProjFactorPerspective(float Fov, float Aspect, float Near, float Far)
{
	m_ProjFactor.bOrtho = false;
	m_ProjFactor.Fov = Fov;
	m_ProjFactor.Aspect = Aspect;
	m_ProjFactor.Near = Near;
	m_ProjFactor.Far = Far;
}

void JCameraObject::SetProjFactorOrthographic(float Left, float Right, float Bottom, float Top, float Near, float Far)
{
	m_ProjFactor.bOrtho = true;
	m_ProjFactor.Left = Left;
	m_ProjFactor.Right = Right;
	m_ProjFactor.Bottom = Bottom;
	m_ProjFactor.Top = Top;
	m_ProjFactor.Near = Near;
	m_ProjFactor.Far = Far;
}
