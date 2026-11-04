#include "JPlayerObject.h"
#include "JObjectRegistry.h"

JPlayerObject::JPlayerObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name)
	: JObject(vWorld, vWorldTP, nodeIndex, name)
{

}

void JPlayerObject::UpdateInput(JKeyState& input)
{
	float elapsed = input.elapsedTime;
	float speed = 70.f;
	XMFLOAT3 rot = m_Rotation;
	if (input.KeyState[VK_LBUTTON] & 0x8000) {
		rot.y += static_cast<float>(input.deltaX) * speed * elapsed;
		rot.x += static_cast<float>(input.deltaY) * speed * elapsed;
	}
	SetRotation(rot);

	MakeLocalTransform();
	XMFLOAT3 lookd{ m_LocalTransform._31, m_LocalTransform._32, m_LocalTransform._33 };
	XMFLOAT3 rightd{ m_LocalTransform._11, m_LocalTransform._12, m_LocalTransform._13 };
	XMVECTOR look = XMVector3Normalize(XMLoadFloat3(&lookd));
	XMVECTOR right = XMVector3Normalize(XMLoadFloat3(&rightd));
	XMFLOAT3 pos = m_Position;

	float mspeed = 20.f;
	if (input.KeyState['W'] & 0x8000) {
		XMStoreFloat3(&pos,  XMLoadFloat3(&pos) + (look * mspeed * elapsed));
	}
	if (input.KeyState['S'] & 0x8000) {
		XMStoreFloat3(&pos, XMLoadFloat3(&pos) + (-1 * look * mspeed * elapsed));
	}

	if (input.KeyState['A'] & 0x8000) {
		XMStoreFloat3(&pos, XMLoadFloat3(&pos) + (-1 * right * mspeed * elapsed));
	}
	if (input.KeyState['D'] & 0x8000) {
		XMStoreFloat3(&pos, XMLoadFloat3(&pos) + (right * mspeed * elapsed));
	}
	if (input.KeyState['Q'] & 0x8000) {
		XMStoreFloat3(&pos, XMLoadFloat3(&pos) + (XMVectorSet(0.f, -1.f, 0.f, 0.f) * mspeed * elapsed));
	}
	if (input.KeyState['E'] & 0x8000) {
		XMStoreFloat3(&pos, XMLoadFloat3(&pos) + (XMVectorSet(0.f, 1.f, 0.f, 0.f) * mspeed * elapsed));
	}

	SetPosition(pos);
}

void JPlayerObject::UpdateInputWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{

}


REGISTER_OBJECT(JPlayerObject)