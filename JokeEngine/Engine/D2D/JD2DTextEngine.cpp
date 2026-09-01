#include "JD2DTextEngine.h"

JD2DTextEngine* JD2DTextEngine::m_Instance = nullptr;

JD2DTextEngine* JD2DTextEngine::GetInstance()
{
	if (!m_Instance)
		m_Instance = new JD2DTextEngine;
	return m_Instance;
}

void JD2DTextEngine::AddRTForD3D11Texture2D(const std::string& name, ID3D11Texture2D* texture2d)
{
	ComPtr<ID2D1RenderTarget> rt{};

	if (!texture2d) {
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("{0} 가 AddRTForD3D11Texture2D를 호출할 nullptr를 전달했습니다.", name.c_str());
#endif
		return;
	}

	ComPtr<IDXGISurface> dxgisurface{};
	ThrowIfFailed(texture2d->QueryInterface(IID_PPV_ARGS(dxgisurface.ReleaseAndGetAddressOf())));


	D2D1_RENDER_TARGET_PROPERTIES properties{
		.type = D2D1_RENDER_TARGET_TYPE_DEFAULT,
		.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
	};
	ThrowIfFailed(m_D2DFactory->CreateDxgiSurfaceRenderTarget(dxgisurface.Get(), properties, rt.ReleaseAndGetAddressOf()));

	auto [iter, inserted] = m_RTTable.try_emplace(name, rt);
#if defined(_DEBUG) || defined(DEBUG)
	if (not inserted) ShowInsertedFailed(name, "JD2DTextEngine");
#endif
}

JD2DTextEngine::JD2DTextEngine()
{
	ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(m_D2DFactory.ReleaseAndGetAddressOf())));
	ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory8), reinterpret_cast<IUnknown**>(m_DWriteFactory.ReleaseAndGetAddressOf())));
}

JD2DTextEngine::~JD2DTextEngine()
{
	if (m_Instance)
		delete m_Instance;
}