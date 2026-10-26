#include "JTextObject.h"
#include "Engine/D3D11/GResource/D3D11MeshConstant.h"
#include "Engine/JokeEngineConfig.h"
#include "JObjectRegistry.h"

constexpr float kTextScale = 0.05f;

JTextObject::JTextObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name)
	: JObject(vWorld, vWorldTP, nodeIndex, name)
{
	SetText(L"Text", true);
	SetFontName(L"Bahnschrift", true);
	SetFontSize(32.f, true);
	SetFontWeight(400.f, true);
	SetFontWidth(100.f, true);
	SetHorizonAlignment(JText_Alignment::Left, true);
	SetVerticalAlignment(JText_Alignment::Top, true);

	auto* opt = JEngineDefaultGlobalConfig::GetInstance()->GetConfigFactor();
	switch (opt->DirectX_Version) {
	case 11:
		m_MeshCB = std::make_unique<JMeshConstantDX11>();
		m_MeshCB->m_CBMesh.bSkinning = false;
		m_MeshCB->m_CBMesh.nodeIndex = nodeIndex;
		m_TextResource = std::make_unique<JTextResourceDX11>(name);
		break;
	case 12:
		break;
	default:
		// 문제 예외 처리
		break;
	}

	// temp
	m_TextResource->CreateSolidColorBrush(XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	MakeDirtyFlag();
}

JTextObject::~JTextObject() = default;

void JTextObject::Update(float elapsedTime, XMFLOAT4X4* parent)
{
	// 개별 행동, 딱히 뭐 없음
	m_TextResource->Update(elapsedTime);

	// 여기서 CPUDirty -> IDWrite, Brush 등 최신화
	if (m_CPUDirty) {
		m_TextResource->CreateDWriteTextFomat(m_FontName, m_FontCollectionName, m_FontSize, m_FontWeight, m_FontWidth);
		float maxwidth = static_cast<float>(kTextTexture2DWidth);
		float maxheight = static_cast<float>(kTextTexture2DHeight);
		m_TextResource->CreateDWriteTextLayout(m_Text, maxwidth, maxheight);
		m_CPUDirty = false;

		m_TextMetrics = m_TextResource->GetMetricsWithUVMatrixUpdate();
		// uv 변환 행렬 제작
	}
	// 행렬 만들 재료 준비 혹은 여기서 직접 만들어주기
	// 월드 행렬 배열에 들어갈 저거랑 부모 행렬로 줄 저거는 다른거다.

	// JObject::Update()

	{
		XMMATRIX parentMatrix;

		if (parent) parentMatrix = XMLoadFloat4x4(parent);
		else parentMatrix = XMMatrixIdentity();

#if DEV_MODE
		XMStoreFloat4x4(&m_ParentWorldMatix, parentMatrix);
#endif

		MakeLocalTransform();
		XMMATRIX realworld = XMLoadFloat4x4(&m_LocalTransform) * parentMatrix;
		XMFLOAT4X4 realworld_f{};
		XMStoreFloat4x4(&realworld_f, realworld);

		for (auto& child : m_LeafObjects) {
			child->Update(elapsedTime, &realworld_f);
		}

		// 렌더링용 월드 행렬을 만들어 배열에 넣기
		XMFLOAT2 textscale{};

		textscale.x = m_TextMetrics.Width * kTextScale;
		textscale.y = m_TextMetrics.Height * kTextScale;

		XMMATRIX tscale = XMMatrixScaling(textscale.x, textscale.y, 1.f);
		XMMATRIX result = tscale * realworld;
		// 연산용
		XMStoreFloat4x4(&m_WorldTransform[m_NodeIndex], realworld);
		// 아래는 렌더링용
		XMStoreFloat4x4(&m_WorldTransformTP[m_NodeIndex], XMMatrixTranspose(result));
	}
	m_MeshCB->m_CBMesh.nodeIndex = m_NodeIndex;
}

void JTextObject::Render(UINT currentFrameIndex)
{
	// updatebuffer = 렌더타겟에 텍스트 쓰기
	if (m_GPUDirty[currentFrameIndex]) {
		// 이거 조금 고민해보기 (updateBuffer라는 이름이 texture 업데이트만 하면 되는가?
		m_TextResource->UpdateBuffer(currentFrameIndex);
		m_GPUDirty[currentFrameIndex] = false;
	}
	// setGPUBuffer = SRV로 올리기
	m_TextResource->SetGPUBuffer(currentFrameIndex, 2, JS_PS);
	// render 

	m_MeshCB->UpdateBuffer(currentFrameIndex);
	m_MeshCB->SetGPUBuffer(currentFrameIndex, 0, JS_VS);

	m_PlaneMeshes[m_RenderPlaneIndex]->Render((UINT)0);
}

void JTextObject::ReadyPlanes(std::unordered_map<std::string, std::shared_ptr<JContent>>& contents)
{
	bool bError{};
	{
		std::shared_ptr<JStaticMesh> mesh = std::dynamic_pointer_cast<JStaticMesh>(contents["DefaultPlane"]);
		if (mesh)
			m_PlaneMeshes.emplace_back(mesh);
		else bError = true;
	}
	{
		std::shared_ptr<JStaticMesh> mesh = std::dynamic_pointer_cast<JStaticMesh>(contents["DefaultPlaneq1"]);
		if (mesh)
			m_PlaneMeshes.emplace_back(mesh);
		else bError = true;
	}
	{
		std::shared_ptr<JStaticMesh> mesh = std::dynamic_pointer_cast<JStaticMesh>(contents["DefaultPlaneq2"]);
		if (mesh)
			m_PlaneMeshes.emplace_back(mesh);
		else bError = true;
	}
	{
		std::shared_ptr<JStaticMesh> mesh = std::dynamic_pointer_cast<JStaticMesh>(contents["DefaultPlaneq3"]);
		if (mesh)
			m_PlaneMeshes.emplace_back(mesh);
		else bError = true;
	}
	{
		std::shared_ptr<JStaticMesh> mesh = std::dynamic_pointer_cast<JStaticMesh>(contents["DefaultPlaneq4"]);
		if (mesh)
			m_PlaneMeshes.emplace_back(mesh);
		else bError = true;
	}

#if DEV_MODE
	if (bError) {
		spdlog::warn("JTextObject::ReadyPlanes에서 Plane초기화를 실패했습니다.(contextmanager에 JStaticMesh가 없음)");
	}
#endif
}

void JTextObject::UpdateRenderPlaneIndex()
{
	switch (m_HorizonAlignment) {
	case JText_Alignment::Left:
		switch (m_VerticalAlignment) {
		case JText_Alignment::Bottom:
			m_RenderPlaneIndex = 0x01;
			break;
		case JText_Alignment::Top:
			m_RenderPlaneIndex = 0x04;
			break;
		}
		break;
	case JText_Alignment::Center:
		m_RenderPlaneIndex = 0x00;
		break;
	case JText_Alignment::Right:
		switch (m_VerticalAlignment) {
		case JText_Alignment::Bottom:
			m_RenderPlaneIndex = 0x02;
			break;
		case JText_Alignment::Top:
			m_RenderPlaneIndex = 0x03;
			break;
		}
		break;
	}
}

void JTextObject::SetFontSize(float size, bool edit)
{
	if (size < 0) {
		size = 0; 
	}
	m_FontSize = size;
	if (edit)
		m_InitialFontSize = size;
	MakeDirtyFlag();
}

void JTextObject::SetFontWeight(float weight, bool edit)
{
	if (weight < 1) weight = 1;
	else if (weight > 1000)	weight = 1000;

	m_FontWeight = weight;
	if (edit) m_InitialFontWeight = weight;
	MakeDirtyFlag();
}

void JTextObject::SetFontWidth(float width, bool edit)
{
	m_FontWidth = width <= 0 ? 1 : width;
	if (edit) m_InitialFontWeight = m_FontWidth;
	MakeDirtyFlag();
}

void JTextObject::SetHorizonAlignment(JText_Alignment align, bool edit)
{
	switch (align) {
	case JText_Alignment::Center: // 한쪽이 center면 나머지도 center여야함
		m_HorizonAlignment = align;
		m_VerticalAlignment = align;
		if (edit) { m_InitialHorizonAlignment = align; m_InitialVerticalAlignment = align; }
#if DEV_MODE
		m_Selected_VAlign = 0;
		m_Selected_HAlign = 1;
#endif
		break;
	case JText_Alignment::Left:
	case JText_Alignment::Right:
		m_HorizonAlignment = align;
		if (edit) m_InitialHorizonAlignment = align;
#if DEV_MODE
		m_Selected_HAlign = align;
#endif
		break;
	default:
#if DEV_MODE
		spdlog::warn("{}: Invalid align in SetHorizonAlignment", m_name.c_str());
#endif
		break;
	}
	UpdateRenderPlaneIndex();
}

void JTextObject::SetVerticalAlignment(JText_Alignment align, bool edit)
{
	switch (align) {
	case JText_Alignment::Center: // 한쪽이 center면 나머지도 center여야함
		m_HorizonAlignment = align;
		m_VerticalAlignment = align;
		if (edit) { m_InitialHorizonAlignment = align; m_InitialVerticalAlignment = align; }
#if DEV_MODE
		m_Selected_VAlign = 0;
		m_Selected_HAlign = 1;
#endif
		break;
	case JText_Alignment::Top:
	case JText_Alignment::Bottom:
		m_VerticalAlignment = align;
		if (edit) m_InitialVerticalAlignment = align;
#if DEV_MODE
		if (align == 1) m_Selected_VAlign = align - 1;
		else m_Selected_VAlign = align - 2;
#endif
		break;
	default:
#if DEV_MODE
		spdlog::warn("{}: Invalid align in SetVerticalAlignment", m_name.c_str());
#endif
		break;
	}
	UpdateRenderPlaneIndex();
}

void JTextObject::MakeDirtyFlag()
{
	m_CPUDirty = true;
	for (UINT i = 0; i < kNumRenderTarget; ++i)
		m_GPUDirty[i] = true;
}


#if DEV_MODE
void JTextObject::RenderInspector()
{
	JObject::RenderInspector();

	float cursorx = 94.f;
	ImGui::SeparatorText("TextOption");
	
	// fontcollection과 fontname 설정
	const char* tempitems[] = { "1", "2" };
	ImGui::Text("FontCollection"); ImGui::SameLine();
	ImGui::SetCursorPosX(cursorx);
	if (ImGui::BeginCombo("##FontCollection", tempitems[m_Selected_FontCollection])) {
		for (int i = 0; i < std::size(tempitems); ++i) {
			bool is_selected = (m_Selected_FontCollection == i);
			if (ImGui::Selectable(tempitems[i], is_selected))
				m_Selected_FontCollection = i;
		}
		ImGui::EndCombo();
	}

	// text
	ImGui::Text("Text"); ImGui::SameLine();
	ImGui::SetCursorPosX(cursorx);
	char ctext[128]{};
	strcpy(ctext, ConvertHelper::WideToUtf8(m_Text).c_str());
	if (ImGui::InputText("##Text", ctext, std::size(ctext), ImGuiInputTextFlags_EnterReturnsTrue)) {
		SetText(ConvertHelper::Utf8ToWide(std::string(ctext)).c_str(), true);
	}

	// font size
	ImGui::Separator();
	ImGui::Text("FontSize"); ImGui::SameLine();
	ImGui::SetCursorPosX(cursorx);
	float size = m_FontSize;
	if (ImGui::DragFloat("##FontSize", &size, 0.05f, 0.f, FLT_MAX, "%.2f", ImGuiSliderFlags_AlwaysClamp))
		SetFontSize(size, true);
	// font weight
	ImGui::Text("FontWeight"); ImGui::SameLine();
	ImGui::SetCursorPosX(cursorx);
	float weight = m_FontWeight;
	if (ImGui::DragFloat("##FontWeight", &weight, 1.0f, 1.f, 1000.f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
		SetFontWeight(weight, true);
	// font width
	ImGui::Text("FontWidth"); ImGui::SameLine();
	ImGui::SetCursorPosX(cursorx);
	float width = m_FontWidth;
	if (ImGui::DragFloat("##FontWidth", &width, 0.05f, 1.f, FLT_MAX, "%.2f", ImGuiSliderFlags_AlwaysClamp))
		SetFontWidth(width, true);
	
	// align
	ImGui::Separator();
	{
		const char* halign[] = { "Left", "Center", "Right" };
		ImGui::Text("Horizon"); ImGui::SameLine();
		ImGui::SetCursorPosX(cursorx);
		if (ImGui::BeginCombo("##Horizon", halign[m_Selected_HAlign])) {
			for (int i = 0; i < std::size(halign); ++i) {
				bool is_selected = (m_Selected_HAlign == i);
				if (ImGui::Selectable(halign[i], is_selected)) {
					SetHorizonAlignment(JText_Alignment(i), true);
					m_Selected_HAlign = i;
				}
			}
			ImGui::EndCombo();
		}
	}
	{
		const char* valign[] = { "Center", "Top", "Bottom" };
		ImGui::Text("Vertical"); ImGui::SameLine();
		ImGui::SetCursorPosX(cursorx);
		if (ImGui::BeginCombo("##Vertical", valign[m_Selected_VAlign])) {
			for (int i = 0; i < std::size(valign); ++i) {
				bool is_selected = (m_Selected_VAlign == i);
				if (ImGui::Selectable(valign[i], is_selected)) {
					SetVerticalAlignment(JText_Alignment(i == 0 ? i + 1 : i + 2), true);
					m_Selected_VAlign = i;
				}
			}
			ImGui::EndCombo();
		}
	}
}
#endif



REGISTER_OBJECT(JTextObject)