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

void JD2DTextEngine::CreateTextFormat(ComPtr<IDWriteTextFormat3>& format)
{
	DWRITE_FONT_AXIS_VALUE d[2];
	m_DWriteFactory->CreateTextFormat(L"바탕", nullptr, d, 2, 12, L"ko-kr", format.ReleaseAndGetAddressOf());
	// IDWriteInMemoryFontFileLoader 이용해서 fontcollection 만드는거 고안하기
}

void JD2DTextEngine::CreateTextLayout(ComPtr<IDWriteTextLayout>& layout)
{
	ComPtr<IDWriteTextFormat3> test{};
	m_DWriteFactory->CreateTextLayout(L"테스트", 3, test.Get(), 10, 10, layout.ReleaseAndGetAddressOf());
}

ComPtr<ID2D1SolidColorBrush> JD2DTextEngine::CreateSolidColorBrush(std::string& name, XMFLOAT4 color)
{
	ComPtr<ID2D1SolidColorBrush> brush;
	if (!m_RTTable.contains(name)) {
#if defined(DEBUG) || defined(_DEBUG)
		spdlog::error("{0} 의 이름을 가진 RT가 없습니다. D2DTextEngine", name);
#endif 
		return brush;
	}
	ThrowIfFailed(m_RTTable[name]->CreateSolidColorBrush(D2D1::ColorF(color.x, color.y, color.z, color.w), brush.ReleaseAndGetAddressOf()));
	return brush;
}

ComPtr<ID2D1SolidColorBrush> JD2DTextEngine::CreateSolidColorBrush(std::string& name, D2D1::ColorF color)
{
	ComPtr<ID2D1SolidColorBrush> brush;
	if (!m_RTTable.contains(name)) {
#if defined(DEBUG) || defined(_DEBUG)
		spdlog::error("{0} 의 이름을 가진 RT가 없습니다. D2DTextEngine", name);
#endif 
		return brush;
	}
	ThrowIfFailed(m_RTTable[name]->CreateSolidColorBrush(color, brush.ReleaseAndGetAddressOf()));
	return brush;
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