#include "JEditorPlayer.h"

#if DEV_MODE
JEditorPlayerObject::JEditorPlayerObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name)
	: JPlayerObject(vWorld, vWorldTP, nodeIndex, name)
{

}

void JEditorPlayerObject::UpdateInput(JKeyState& input)
{
	float elapsed = input.elapsedTime;
	XMFLOAT3 rot = m_Rotation;
	rot.y += static_cast<float>(input.deltaX) * m_MouseRotSpeed * elapsed;
	rot.x += static_cast<float>(input.deltaY) * m_MouseRotSpeed * elapsed;
	SetRotation(rot);

	MakeLocalTransform();
	XMFLOAT3 lookd{ m_LocalTransform._31, m_LocalTransform._32, m_LocalTransform._33 };
	XMFLOAT3 rightd{ m_LocalTransform._11, m_LocalTransform._12, m_LocalTransform._13 };
	XMVECTOR look = XMVector3Normalize(XMLoadFloat3(&lookd));
	XMVECTOR right = XMVector3Normalize(XMLoadFloat3(&rightd));

	if (input.KeyState['W'] & 0x8000) {
		XMStoreFloat3(&m_Position, XMLoadFloat3(&m_Position) + (look * m_MoveSpeed * elapsed));
	}
	if (input.KeyState['S'] & 0x8000) {
		XMStoreFloat3(&m_Position, XMLoadFloat3(&m_Position) + (-1 * look * m_MoveSpeed  * elapsed));
	}

	if (input.KeyState['A'] & 0x8000) {
		XMStoreFloat3(&m_Position, XMLoadFloat3(&m_Position) + (-1 * right * m_MoveSpeed  * elapsed));
	}
	if (input.KeyState['D'] & 0x8000) {
		XMStoreFloat3(&m_Position, XMLoadFloat3(&m_Position) + (right * m_MoveSpeed  * elapsed));
	}
	if (input.KeyState['Q'] & 0x8000) {
		XMStoreFloat3(&m_Position, XMLoadFloat3(&m_Position) + (XMVectorSet(0.f, -1.f, 0.f, 0.f) * m_MoveSpeed  * elapsed));
	}
	if (input.KeyState['E'] & 0x8000) {
		XMStoreFloat3(&m_Position, XMLoadFloat3(&m_Position) + (XMVectorSet(0.f, 1.f, 0.f, 0.f) * m_MoveSpeed  * elapsed));
	}

	// 이건 에디터때만 쓰니까
	m_InitialPosition = m_Position;
	m_InitialRotation = m_Rotation;
	m_InitialQuaternion = m_Quaternion;
}

void JEditorPlayerObject::UpdateInputWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_MOUSEWHEEL:
	{
		float wDelta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / static_cast<float>(WHEEL_DELTA);
		float deltaScale = 2.f;
			m_MoveSpeed += (wDelta * deltaScale);

		if (m_MoveSpeed < 0.5) m_MoveSpeed = 0.5;
	}
	break;
	}
}

#endif