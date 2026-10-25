#include "JObject.h"
#include "Engine/JShader/JEngineShader.h"
#include "JObjectRegistry.h"

JObject::JObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name)
	: m_WorldTransform{ vWorld }, m_NodeIndex{ nodeIndex }, m_WorldTransformTP{ vWorldTP }
{
	if (name) m_name = name;
	else m_name = "JObject";

	if (nodeIndex >= m_WorldTransform.size()) {
#if DEV_MODE
		spdlog::error("'{0}' Invalid NodeIndex", m_name.c_str());
#endif
		assert(0);
	}

	XMStoreFloat4x4(&m_LocalTransform, XMMatrixIdentity());
}

void JObject::Reset()
{
	m_Active = m_InitialActive;
	m_Position = m_InitialPosition;
	m_Rotation = m_InitialRotation;
	m_Scale = m_InitialScale;
}

void JObject::Attach(std::shared_ptr<JObject> leaf)
{
	leaf->SetParent(this);
	m_LeafObjects.emplace_back(leaf);
}

// 내 자식에서 떼어내는것, Scene에서 삭제가 아니다.
void JObject::Dettach(std::string& name)
{
	for (unsigned int i = 0; i < m_LeafObjects.size(); ++i) {
		if (m_LeafObjects[i]->GetName() == name) {
			m_LeafObjects.erase(m_LeafObjects.begin() + i);
			break;
		}
	}
}

// 내가 Scene에서 사라질 때 해줘야하는 작업 (소멸자와 비슷하나 다르다)
void JObject::Delete()
{
	// 내 부모에게서 날 떼어내고
	m_ParentObject->Dettach(m_name);

	// 내 자식의 부모를 지운다.
	for (auto& leaf : m_LeafObjects)
		leaf->SetParent(nullptr);
}

void JObject::Update(float elapsedTime, XMFLOAT4X4* parent)
{
	MakeLocalTransform();

	XMMATRIX parentMatrix;

	if (parent) parentMatrix = XMLoadFloat4x4(parent);
	else parentMatrix = XMMatrixIdentity();

#if DEV_MODE
	XMStoreFloat4x4(&m_ParentWorldMatix, parentMatrix);
#endif

	XMMATRIX world = XMLoadFloat4x4(&m_LocalTransform) * parentMatrix;
	XMStoreFloat4x4(&m_WorldTransform[m_NodeIndex], world);
	XMStoreFloat4x4(&m_WorldTransformTP[m_NodeIndex], XMMatrixTranspose(world));

	for (auto& child : m_LeafObjects) {
		child->Update(elapsedTime, &m_WorldTransform[m_NodeIndex]);
	}
}

void JObject::SetRotation(float x, float y, float z, bool edit)
{
	m_Rotation = XMFLOAT3(x, y, z); if (edit) m_InitialRotation = m_Rotation;
	m_Quaternion = ConvertHelper::EulerToQuaternion(m_Rotation); if (edit) m_InitialQuaternion = m_Quaternion;
}

void JObject::SetRotation(XMFLOAT3 rotate, bool edit)
{
	m_Rotation = rotate; if (edit) m_InitialRotation = m_Rotation;
	m_Quaternion = ConvertHelper::EulerToQuaternion(m_Rotation); if (edit) m_InitialQuaternion = m_Quaternion;
}

void JObject::SetRotation(float x, float y, float z, float w, bool edit)
{
	m_Quaternion = XMFLOAT4(x, y, z, w); if (edit) m_InitialQuaternion = m_Quaternion;
	m_Rotation = ConvertHelper::QuaternionToEuler(m_Quaternion); if (edit) m_InitialRotation = m_Rotation;
}

void JObject::SetRotation(XMFLOAT4 quarternion, bool edit)
{
	m_Quaternion = quarternion; if (edit) m_InitialQuaternion = m_Quaternion;
	m_Rotation = ConvertHelper::QuaternionToEuler(m_Quaternion); if (edit) m_InitialRotation = m_Rotation;
}

//void JObject::Render()
//{
//	for (auto& child : m_LeafObjects)
//		child->Render();
//}

void JObject::MakeLocalTransform()
{
	XMVECTOR S, R, T;
	S = XMLoadFloat3(&m_Scale);
	T = XMLoadFloat3(&m_Position);
	R = XMLoadFloat4(&m_Quaternion);
	XMStoreFloat4x4(&m_LocalTransform, XMMatrixAffineTransformation(S, g_XMZero, R, T));
}

#if DEV_MODE
void JObject::SetPlayMode(bool mod)
{
	m_PlayMode = mod;
	for (auto& child : m_LeafObjects)
		child->SetPlayMode(m_PlayMode);
}

void JObject::RenderInspector()
{
	{	// General
		ImGui::SeparatorText("General");
		if (m_PlayMode)
			ImGui::BeginDisabled(true);
		ImGui::Text("Name"); ImGui::SameLine();
		char cname[128]{};
		strcpy(cname, m_name.c_str());

		if (ImGui::InputText("##InputName", cname, std::size(cname), ImGuiInputTextFlags_EnterReturnsTrue)) {
			if (IsValidString(cname) && not JObjectNameDuplicateManager::DuplicateCheck(cname)) {
				JObjectNameDuplicateManager::DeleteName(m_name); // 목록에서 현 이름을 빼고
				JObjectNameDuplicateManager::AddName(cname);	// 새 이름을 넣는다
				m_name = cname;
			}
		}
		ImGui::SameLine();
		ImGui::TextDisabled("(?)");
		if (ImGui::BeginItemTooltip()) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("Rule: Alphanumeric and '_' only. No spaces/special chars.");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
		if (m_PlayMode)
			ImGui::EndDisabled();
	}
	{	// Transform
		ImGui::SeparatorText("Transform");

		if (m_PlayMode)
			ImGui::BeginDisabled(true);

		XMFLOAT3 scale = m_Scale;
		XMFLOAT3 rotation = m_Rotation;
		XMFLOAT3 position = m_Position;

		auto resetbutton = [](const char* id, XMFLOAT3& out, XMFLOAT3 goal) {
			if (ImGui::Button(id, ImVec2(18, 18))) {
				out = goal;
			}
			ImGui::SetItemTooltip("Reset Factor");
			};

		float labelwidth = 70.f;

		ImGui::Text("Scale"); ImGui::SameLine();
		ImGui::SetCursorPosX(labelwidth);
		ImGui::DragFloat3("##Scale", reinterpret_cast<float*>(&scale), 0.05f,0.f, 0.f, "%.3f", ImGuiSliderFlags_ColorMarkers); ImGui::SameLine();
		resetbutton("##resetscale", scale, XMFLOAT3(1.f, 1.f, 1.f));

		ImGui::Text("Rotation"); ImGui::SameLine();
		ImGui::SetCursorPosX(labelwidth);
		ImGui::DragFloat3("##Rotation", reinterpret_cast<float*>(&rotation), 0.2f, 0.f, 0.f, "%.3f", ImGuiSliderFlags_ColorMarkers); ImGui::SameLine();
		resetbutton("##resetrotate", rotation, XMFLOAT3(0.f, 0.f, 0.f));

		ImGui::Text("Position"); ImGui::SameLine();
		ImGui::SetCursorPosX(labelwidth);
		ImGui::DragFloat3("##Position", reinterpret_cast<float*>(&position), 0.05f, 0.f, 0.f, "%.3f", ImGuiSliderFlags_ColorMarkers); ImGui::SameLine();
		resetbutton("##resetposition", position, XMFLOAT3(0.f, 0.f, 0.f));

		SetScale(scale, true);
		SetRotation(rotation, true);
		SetPosition(position, true);

		if (m_PlayMode)
			ImGui::EndDisabled();
	}
}
#endif



REGISTER_OBJECT(JObject);