#include "JCameraObject.h"
#include "Engine/JokeEngineConfig.h"
#include "JObjectRegistry.h"

JCameraObject::JCameraObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT uid, const char* name)
	: JObject(vWorld, vWorldTP, uid, name)
{
	auto* o = JEngineDefaultGlobalConfig::GetInstance()->GetConfigFactor();
	float width = static_cast<float>(o->WindowsWidth);
	float height = static_cast<float>(o->WindowsHeight);
	// 뷰포트
	SetViewport(0.f, 0.f, width, height, 0.f, 1.f);
	
	// projection Factor
	float aspect = width / height;
	float gc = static_cast<float>(std::gcd(static_cast<UINT>(width), static_cast<UINT>(height)));
	SetProjFactorOrthographic(width / gc / 2 * -10, width / gc / 2 * 10, height / gc / 2 * -10, height / gc / 2 * 10, 0.1f, 500.f, true);
	SetProjFactorPerspective(60.f, width, height, 0.01f, 500.0f, true);
}


void JCameraObject::Reset()
{
	JObject::Reset();

	m_ProjFactor = m_InitialProjFactor;
	if (m_ProjFactor.bOrtho) MakeOBBFromProjFactor();
	else MakeFrustumFromProjFactor();
}

XMFLOAT4X4 JCameraObject::GetViewMatrix()
{
	// 오차가 누적될 수 있다는데 일단은 지켜보기
	XMMATRIX world = XMLoadFloat4x4(&m_WorldTransform[m_UID]);
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
	else {
		float aspect = static_cast<float>(m_ProjFactor.asWidth) / static_cast<float>(m_ProjFactor.asHeight);
		XMStoreFloat4x4(&PM, XMMatrixPerspectiveFovLH(
			XMConvertToRadians(m_ProjFactor.Fov),
			aspect,
			m_ProjFactor.Near,
			m_ProjFactor.Far
		));
	}

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


void JCameraObject::SetProjFactorPerspective(float Fov, UINT width, UINT height, float Near, float Far, bool edit)
{
	m_ProjFactor.bOrtho = false;
	m_ProjFactor.Fov = Fov;
	m_ProjFactor.asWidth = width;
	m_ProjFactor.asHeight = height;
	m_ProjFactor.Near = Near;
	m_ProjFactor.Far = Far;

	MakeFrustumFromProjFactor();

	if (edit) {
		m_InitialProjFactor.bOrtho = false;
		m_InitialProjFactor.Fov = Fov;
		m_InitialProjFactor.asWidth = width;
		m_InitialProjFactor.asHeight = height;
		m_InitialProjFactor.Near = Near;
		m_InitialProjFactor.Far = Far;
	}

}

void JCameraObject::SetProjFactorOrthographic(float Left, float Right, float Bottom, float Top, float Near, float Far, bool edit)
{
	m_ProjFactor.bOrtho = true;
	m_ProjFactor.Left = Left;
	m_ProjFactor.Right = Right;
	m_ProjFactor.Bottom = Bottom;
	m_ProjFactor.Top = Top;
	m_ProjFactor.Near = Near;
	m_ProjFactor.Far = Far;

	MakeOBBFromProjFactor();

	if (edit) {
		m_InitialProjFactor.bOrtho = true;
		m_InitialProjFactor.Left = Left;
		m_InitialProjFactor.Right = Right;
		m_InitialProjFactor.Bottom = Bottom;
		m_InitialProjFactor.Top = Top;
		m_InitialProjFactor.Near = Near;
		m_InitialProjFactor.Far = Far;
	}
}

void JCameraObject::SetProjFactor(JProjFactor factor, bool edit)
{
	m_ProjFactor = factor; 
	if (edit) m_InitialProjFactor = factor;
	if (m_ProjFactor.bOrtho) MakeOBBFromProjFactor();
	else MakeFrustumFromProjFactor();
}

void JCameraObject::MakeOBBFromProjFactor()
{
	XMFLOAT3 center{};
	XMFLOAT3 extent{};
	center.x = (m_ProjFactor.Right + m_ProjFactor.Left) / 2;
	center.y = (m_ProjFactor.Top + m_ProjFactor.Bottom) / 2;
	center.z = (m_ProjFactor.Far + m_ProjFactor.Near) / 2;
	extent.x = m_ProjFactor.Right - center.x;
	extent.y = m_ProjFactor.Top - center.y;
	extent.z = m_ProjFactor.Far - extent.z;
	XMFLOAT4 quat{ 0.f, 0.f, 0.f, 1.f };
	m_ProjOBBBox = BoundingOrientedBox(center, extent, quat);
}

void JCameraObject::MakeFrustumFromProjFactor()
{
	if (m_ProjFactor.bOrtho) return;
	XMFLOAT4X4 proj = GetProjMatrix();
	m_ProjFrustum = BoundingFrustum(XMLoadFloat4x4(&proj));
}

#if DEV_MODE


extern TempGlobalSceneInfo g_GlobalSceneInfo;


bool JCameraObject::RenderInspector(UINT currentFrameIndex)
{
	bool ret = JObject::RenderInspector(currentFrameIndex);

	if (m_PlayMode)
		ImGui::BeginDisabled(true);

	float cursorx = 94.f;
	ImGui::SeparatorText("Camera");
	const char* tag[] = { "Perspective", "Orthograph" };
	int num = (m_ProjFactor.bOrtho) ? 1 : 0;
	ImGui::Text("CameraType"); ImGui::SameLine(); ImGui::SetCursorPosX(cursorx);
	if (ImGui::BeginCombo("##CameraType", tag[num])) {
		for (int i = 0; i < 2; ++i) {
			bool is_selected = num == i;
			if (ImGui::Selectable((std::string(tag[i]) + "##CameraCombo").c_str(), &is_selected)) {
				if (i == 1) m_ProjFactor.bOrtho = true;
				else m_ProjFactor.bOrtho = false;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::Separator();
	JProjFactor temp = m_ProjFactor;
	bool modified = false;
	if (m_ProjFactor.bOrtho) {
		ImGui::Text("Left"); ImGui::SameLine(); ImGui::SetCursorPosX(cursorx);
		if (ImGui::DragFloat("##OrthoLeft", &temp.Left, 0.1f, -FLT_MAX, temp.Right - 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) modified = true;
		ImGui::Text("Right"); ImGui::SameLine(); ImGui::SetCursorPosX(cursorx);
		if (ImGui::DragFloat("##OrthoRight", &temp.Right, 0.1f, temp.Left + 1.f, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp)) modified = true;
		ImGui::Text("Bottom"); ImGui::SameLine(); ImGui::SetCursorPosX(cursorx);
		if (ImGui::DragFloat("##OrthoBottom", &temp.Bottom, 0.1f, -FLT_MAX, temp.Top - 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) modified = true;
		ImGui::Text("Top"); ImGui::SameLine(); ImGui::SetCursorPosX(cursorx);
		if (ImGui::DragFloat("##OrthoTop", &temp.Top, 0.1f, temp.Bottom + 1.f, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp)) modified = true;
		ImGui::Text("Near"); ImGui::SameLine(); ImGui::SetCursorPosX(cursorx);
		if (ImGui::DragFloat("##OrthoNear", &temp.Near, 0.1f, -FLT_MAX, temp.Far - 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) modified = true;
		ImGui::Text("Far"); ImGui::SameLine(); ImGui::SetCursorPosX(cursorx);
		if (ImGui::DragFloat("##OrthoFar", &temp.Far, 0.1f, temp.Near + 1.f, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp)) modified = true;

		if (modified)
			SetProjFactor(temp, true);
	}
	else {
		ImGui::Text("Fov"); ImGui::SameLine(); ImGui::SetCursorPosX(cursorx);
		if (ImGui::DragFloat("##PespectFov", &temp.Fov, 0.1f, 179.f, 0.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) modified = true;
		ImGui::Text("AspectWidth"); ImGui::SameLine(); ImGui::SetCursorPosX(cursorx);
		if (ImGui::DragInt("##PespectWidth", reinterpret_cast<int*>(&temp.asWidth), 1, 1, 9999, "%d", ImGuiSliderFlags_AlwaysClamp)) modified = true;
		ImGui::Text("AspectHeight"); ImGui::SameLine(); ImGui::SetCursorPosX(cursorx);
		if (ImGui::DragInt("##PespectHeight", reinterpret_cast<int*>(&temp.asHeight), 1, 1, 9999, "%d", ImGuiSliderFlags_AlwaysClamp)) modified = true;
		ImGui::Text("Near"); ImGui::SameLine(); ImGui::SetCursorPosX(cursorx);
		if (ImGui::DragFloat("##PerspectNear", &temp.Near, 0.1f, -FLT_MAX, temp.Far - 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) modified = true;
		ImGui::Text("Far"); ImGui::SameLine(); ImGui::SetCursorPosX(cursorx);
		if (ImGui::DragFloat("##PerspectFar", &temp.Far, 0.1f, temp.Near + 1.f, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp)) modified = true;
		if (modified) SetProjFactor(temp, true);
	}

	// view range
	if (not m_PlayMode) {
		if (ImGui::Begin("Scene")) {
			auto* dl = ImGui::GetWindowDrawList();
			XMFLOAT4X4 world = GetWorldTransform();
			XMMATRIX fworld = XMLoadFloat4x4(&world);
			if (m_ProjFactor.bOrtho) {
				BoundingOrientedBox obb;
				m_ProjOBBBox.Transform(obb, fworld);

			}
			else {
				BoundingFrustum frustum;
				m_ProjFrustum.Transform(frustum, fworld);
				XMFLOAT3 wPos = frustum.Origin;
				XMFLOAT3 fwpos[8];
				frustum.GetCorners(fwpos);
				// 월드(현재) -> (메인 카메라)뷰프로 -> 뷰포트(imgui 위치)
				if (JCameraObject* mainCam = reinterpret_cast<JCameraObject*>(g_GlobalSceneInfo.camera)) {
					XMFLOAT4X4 viewproj = mainCam->GetViewProjMatrix(); XMMATRIX fvp = XMLoadFloat4x4(&viewproj);
					XMFLOAT4 vpos[8], vori;
					XMStoreFloat4(&vori, XMVector4Transform(XMLoadFloat3(&wPos), fvp));
					for (int i = 0; i < 8; ++i) XMStoreFloat4(&vpos[i], XMVector4Transform(XMLoadFloat3(&fwpos[i]), fvp));

					auto DrawLineWithClipping = [&](XMFLOAT4& pos1, XMFLOAT4& pos2) {
						if (pos1.w <= 0 && pos2.w <= 0) return;	// 둘다 뒤에
						if (pos1.w <= 0) pos1;
						// R(t) = tB + (1 - t)A 이걸로 w가 0보다 커지는 순간의 t를 찾는다. (w는 4차원이지만 현재는 방향이 정해져 있기에 괜찮다)
						};


					ImVec2 avail = g_GlobalSceneInfo.avail;
					ImVec2 offset = g_GlobalSceneInfo.screenpos;
					vori.x = (vori.x + 1) * (avail.x / 2) + offset.x; vori.y = ((vori.y * -1) + 1) * (avail.y / 2) + offset.y;
					for (int i = 0; i < 8; ++i) {
						vpos[i].x = (vpos[i].x + 1) * (avail.x / 2) + offset.x; vpos[i].y = ((vpos[i].y * -1) + 1) * (avail.y / 2) + offset.y;
					}

					dl->AddLine(ImVec2(vori.x, vori.y), ImVec2(vpos[4].x, vpos[4].y), IM_COL32(255, 255, 0, 255));
					dl->AddLine(ImVec2(vori.x, vori.y), ImVec2(vpos[5].x, vpos[5].y), IM_COL32(255, 255, 0, 255));
					dl->AddLine(ImVec2(vori.x, vori.y), ImVec2(vpos[6].x, vpos[6].y), IM_COL32(255, 255, 0, 255));
					dl->AddLine(ImVec2(vori.x, vori.y), ImVec2(vpos[7].x, vpos[7].y), IM_COL32(255, 255, 0, 255));
				}
			}
		}
		ImGui::End();
	}

	if (m_PlayMode)
		ImGui::EndDisabled();
	return ret;
}
#endif

REGISTER_OBJECT(JCameraObject)