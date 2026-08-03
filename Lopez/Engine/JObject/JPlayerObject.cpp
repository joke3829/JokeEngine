#include "JPlayerObject.h"

JPlayerObject::JPlayerObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name)
	: JObject(vWorld, vWorldTP, nodeIndex, name)
{

}

void JPlayerObject::UpdateInput(JKeyState& input)
{
	float elapsed = input.elapsedTime;
	float speed = 70.f;
	if (input.KeyState[VK_LBUTTON] & 0x8000) {
		m_Rotation.y += static_cast<float>(input.deltaX) * speed * elapsed;
		m_Rotation.x += static_cast<float>(input.deltaY) * speed * elapsed;
	}

	MakeLocalTransform();
	XMFLOAT3 lookd{ m_LocalTransform._31, m_LocalTransform._32, m_LocalTransform._33 };
	XMFLOAT3 rightd{ m_LocalTransform._11, m_LocalTransform._12, m_LocalTransform._13 };
	XMVECTOR look = XMVector3Normalize(XMLoadFloat3(&lookd));
	XMVECTOR right = XMVector3Normalize(XMLoadFloat3(&rightd));
	if (input.KeyState['W'] & 0x8000) {
		XMStoreFloat3(&m_Position,  XMLoadFloat3(&m_Position) + (look * 5.f * elapsed));
	}
	if (input.KeyState['S'] & 0x8000) {
		XMStoreFloat3(&m_Position, XMLoadFloat3(&m_Position) + (-1 * look * 5.f * elapsed));
	}

	if (input.KeyState['A'] & 0x8000) {
		XMStoreFloat3(&m_Position, XMLoadFloat3(&m_Position) + (-1 * right * 5.f * elapsed));
	}
	if (input.KeyState['D'] & 0x8000) {
		XMStoreFloat3(&m_Position, XMLoadFloat3(&m_Position) + (right * 5.f * elapsed));
	}
}
