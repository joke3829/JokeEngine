#include "D3D11Renderer.h"
#include "Engine/D3D11/D3D11GlobalFactor.h"
#include "Engine/D3D11/Shader/D3D11Shader.h"

JEngineRendererDX11::JEngineRendererDX11(UINT width, UINT height, const char* name)
	: JEngineRenderer(width, height, name)
{
	CreateRTV_DSV(width, height);
	// Shader 정의
	{
		std::shared_ptr<JEngineDefaultShaderDX11> shader = std::make_shared<JEngineDefaultShaderDX11>("DefaultShader");
		auto[iter, inserted] = m_PSOMap.try_emplace(shader->GetName(), shader);
#if defined(_DEBUG) || defined(DEBUG)
		if (not inserted)  ShowInsertedFailed(shader->GetName(), "Renderer");
#endif
	}
	{
		std::shared_ptr<JEngineDefaultSpriteShaderDX11> shader = std::make_shared<JEngineDefaultSpriteShaderDX11>();
		auto [iter, inserted] = m_PSOMap.try_emplace(shader->GetName(), shader);
#if defined(_DEBUG) || defined(DEBUG)
		if (not inserted)  ShowInsertedFailed(shader->GetName(), "Renderer");
#endif
	}
}

void JEngineRendererDX11::ResizeTarget(UINT width, UINT height)
{
	// 이것저것 뭐 문제 없이 동기 작업 하고

	CreateRTV_DSV(width, height);
}

void JEngineRendererDX11::RenderFrame()
{
	AdvanceBufferIndex();

	auto* context = JD3D11GlobalFactor::GetInstance()->GetDeviceContext();

	float clearcolor[] = { 0.f, 1.f, 0.f, 1.f };
	context->ClearRenderTargetView(m_RenderTargetView[m_CurrentFrameIndex].Get(), clearcolor);
	context->ClearDepthStencilView(m_DepthStencilView[m_CurrentFrameIndex].Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	// Scene Set
	m_Scene->UpdateBuffers(m_CurrentFrameIndex);
	m_Scene->SetDXBuffer(m_CurrentFrameIndex, 1, JS_VS);

	// PSO
	void* rtv[] = { m_RenderTargetView[m_CurrentFrameIndex].Get() };
	m_PSOMap["DefaultShader"]->RenderObjects(m_CurrentFrameIndex, rtv, 1, m_DepthStencilView[m_CurrentFrameIndex].Get());
	m_PSOMap["DefaultSpriteShader"]->RenderObjects(m_CurrentFrameIndex, rtv, 1, m_DepthStencilView[m_CurrentFrameIndex].Get());
}

void JEngineRendererDX11::CopyResult(void* outBuffer)
{
	auto* context = JD3D11GlobalFactor::GetInstance()->GetDeviceContext();
	
	ID3D11Texture2D* dest = reinterpret_cast<ID3D11Texture2D*>(outBuffer);
	context->CopyResource(dest, m_RenderTarget[m_CurrentFrameIndex].Get());
}

void JEngineRendererDX11::SetScene(std::shared_ptr<JEngineScene> scene)
{
	JEngineRenderer::SetScene(scene);
	// shader들 object 지정

	for (auto& shader : m_PSOMap)
		shader.second->ClearObject();
	
	auto TravelAndSetObject = [&](auto&& self, std::vector<std::shared_ptr<JObject>>& v) -> void
		{
			for (auto& p : v) {
				self(self, p->GetLeafObjects());
				if (m_PSOMap.contains(p->GetShaderName())) {
					m_PSOMap[p->GetShaderName()]->AddObject(p);
				}
			}
		};

	auto& objects = m_Scene->GetObjects();
	TravelAndSetObject(TravelAndSetObject, objects);
}

void JEngineRendererDX11::CreateRTV_DSV(UINT width, UINT height)
{
	auto* device = JD3D11GlobalFactor::GetInstance()->GetDevice();

	D3D11_TEXTURE2D_DESC desc{
		.Width = width,
		.Height = height,
		.MipLevels = 1,
		.ArraySize = 1,
		.SampleDesc = {.Count = 1},
		.Usage = D3D11_USAGE_DEFAULT,
	};

	for (UINT i = 0; i < g_NumRenderTarget; ++i) {
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET;
		ThrowIfFailed(device->CreateTexture2D(&desc, nullptr, m_RenderTarget[i].ReleaseAndGetAddressOf()));

		// ReverseZ를 고려해보자
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		ThrowIfFailed(device->CreateTexture2D(&desc, nullptr, m_DepthStencil[i].ReleaseAndGetAddressOf()));


		ThrowIfFailed(device->CreateRenderTargetView(m_RenderTarget[i].Get(), nullptr, m_RenderTargetView[i].ReleaseAndGetAddressOf()));
		ThrowIfFailed(device->CreateDepthStencilView(m_DepthStencil[i].Get(), nullptr, m_DepthStencilView[i].ReleaseAndGetAddressOf()));
	}
#if defined(_DEBUG) || define(DEBUG)
	spdlog::info("{} RTV_DSV Ready", m_name.c_str());
#endif
}

