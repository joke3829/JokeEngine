#include "JD2DTextEngine.h"

JD2DTextEngine* JD2DTextEngine::m_Instance = nullptr;

JD2DTextEngine* JD2DTextEngine::GetInstance()
{
	if (!m_Instance)
		m_Instance = new JD2DTextEngine;
	return m_Instance;
}

// 한 디렉토리 안에 .ttf .otf파일을 넣어두세요
void JD2DTextEngine::CreateFontCollectionFromDirectory(std::filesystem::path filepath)
{
	ComPtr<IDWriteFontSetBuilder2> builder{};
	ThrowIfFailed(m_DWriteFactory->CreateFontSetBuilder(builder.ReleaseAndGetAddressOf()));

	// Window 폰트부터
	ComPtr<IDWriteFontSet> systemfont{};
	ThrowIfFailed(m_DWriteFactory->GetSystemFontSet(systemfont.ReleaseAndGetAddressOf()));
	ThrowIfFailed(builder->AddFontSet(systemfont.Get()));

	// 파일 재귀로 돌며 폰트 파일 찾아 넣기
	for (auto& entry : std::filesystem::recursive_directory_iterator(filepath)) {
		if (entry.path().extension() == ".ttf" || entry.path().extension() == ".otf") {
			std::filesystem::path abpath = std::filesystem::absolute(entry.path());
			ThrowIfFailed(builder->AddFontFile(abpath.wstring().c_str()));
		}
	}
	ComPtr<IDWriteFontSet> fset{};
	ThrowIfFailed(builder->CreateFontSet(fset.ReleaseAndGetAddressOf()));

	ThrowIfFailed(m_DWriteFactory->CreateFontCollectionFromFontSet(fset.Get(), reinterpret_cast<IDWriteFontCollection1**>(m_DWriteFontCollection.ReleaseAndGetAddressOf())));
}

void JD2DTextEngine::AddRTsForD3D11Texture2D(UINT uid, ID3D11Texture2D** texture2d)
{
	for (UINT i = 0; i < kNumRenderTarget; ++i) {
		UINT id = (uid * kNumRenderTarget) + i;
		ComPtr<ID2D1RenderTarget> rt{};
		ComPtr<IDXGISurface> dxgisurface{};
		ThrowIfFailed(texture2d[i]->QueryInterface(IID_PPV_ARGS(dxgisurface.ReleaseAndGetAddressOf())));


		D2D1_RENDER_TARGET_PROPERTIES properties{
			.type = D2D1_RENDER_TARGET_TYPE_DEFAULT,
			.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
		};
		ThrowIfFailed(m_D2DFactory->CreateDxgiSurfaceRenderTarget(dxgisurface.Get(), properties, rt.ReleaseAndGetAddressOf()));

		auto [iter, inserted] = m_RTTable.try_emplace(id, rt);
#if DEV_MODE
		if (not inserted)
			spdlog::error("D2DEngine: {} - AddRTsForD3D11Texture2D Failed", uid);
#endif
	}
}

void JD2DTextEngine::AddRTsForD3D11Texture2DOne(UINT uid, UINT currentFrameIndex, ID3D11Texture2D* texture2d)
{
	UINT id = (uid * kNumRenderTarget) + currentFrameIndex;
	ComPtr<ID2D1RenderTarget> rt{};
	ComPtr<IDXGISurface> dxgisurface{};
	ThrowIfFailed(texture2d->QueryInterface(IID_PPV_ARGS(dxgisurface.ReleaseAndGetAddressOf())));


	D2D1_RENDER_TARGET_PROPERTIES properties{
		.type = D2D1_RENDER_TARGET_TYPE_DEFAULT,
		.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
	};
	ThrowIfFailed(m_D2DFactory->CreateDxgiSurfaceRenderTarget(dxgisurface.Get(), properties, rt.ReleaseAndGetAddressOf()));

	auto [iter, inserted] = m_RTTable.try_emplace(id, rt);
#if DEV_MODE
	if (not inserted)
		spdlog::error("D2DEngine: {} - AddRTsForD3D11Texture2D Failed", uid);
#endif
}

void JD2DTextEngine::DeleteInRTTable(UINT uid)
{
	for (UINT i = 0; i < kNumRenderTarget; ++i) {
		UINT id = (uid * kNumRenderTarget) + i;
		if (m_RTTable.contains(id)) m_RTTable.erase(id);
	}
}

void JD2DTextEngine::DeleteInRTTableOne(UINT uid, UINT currentFrameIndex)
{
	UINT id = (uid * kNumRenderTarget) + currentFrameIndex;
	if (m_RTTable.contains(id)) m_RTTable.erase(id);
}

void JD2DTextEngine::RenderText(UINT uid, UINT currentFrameIndex, IDWriteTextLayout* layout, ID2D1Brush* brush)
{
	UINT id = (uid * kNumRenderTarget) + currentFrameIndex;
	if (!m_RTTable.contains(id))
		return;
	ComPtr<ID2D1RenderTarget>& rt = m_RTTable[id];
	rt->BeginDraw();
	rt->Clear(D2D1::ColorF(0.f, 0.f, 0.f, 0.f));
	D2D1_POINT_2F origin{};
	rt->DrawTextLayout(origin, layout, brush);
	rt->EndDraw();
}

void JD2DTextEngine::CreateTextFormat(ComPtr<IDWriteTextFormat3>& format, std::wstring& fontfamily, float fontsize, float fontweight, float fontwidth)
{
	DWRITE_FONT_AXIS_VALUE avalue[2] = 
	{ 
		{.axisTag = DWRITE_FONT_AXIS_TAG_WEIGHT, .value = fontweight}, 
		{.axisTag = DWRITE_FONT_AXIS_TAG_WIDTH, .value = fontwidth}
	};

	// locale 고려하기(option에 언어같은거 하게되면 그때 하기)
	ThrowIfFailed(m_DWriteFactory->CreateTextFormat(fontfamily.c_str(), m_DWriteFontCollection.Get(), avalue, std::size(avalue), fontsize, L"ko-KR", format.ReleaseAndGetAddressOf()));
}

void JD2DTextEngine::CreateTextLayout(ComPtr<IDWriteTextLayout4>& layout, ComPtr<IDWriteTextFormat3>& format, std::wstring& text, float maxwidth, float maxheight)
{
	ComPtr<IDWriteTextLayout> tlayout{};
	ThrowIfFailed(m_DWriteFactory->CreateTextLayout(text.c_str(), text.size(), format.Get(), maxwidth, maxheight, tlayout.ReleaseAndGetAddressOf()));
	tlayout->QueryInterface(IID_PPV_ARGS(layout.ReleaseAndGetAddressOf()));
}


void JD2DTextEngine::CreateSolidColorBrush(ComPtr<ID2D1Brush>* brushes, UINT uid, XMFLOAT4 color)
{
	for (UINT i = 0; i < kNumRenderTarget; ++i) {
		UINT id = (uid * kNumRenderTarget) + i;
		ComPtr<ID2D1SolidColorBrush> brush;
		if (m_RTTable.contains(id)) {
			ThrowIfFailed(m_RTTable[id]->CreateSolidColorBrush(D2D1::ColorF(color.x, color.y, color.z, color.w), brush.ReleaseAndGetAddressOf()));
			brushes[i] = brush;
		}
	}
}

void JD2DTextEngine::CreateSolidColorBrush(ComPtr<ID2D1Brush>* brushes, UINT uid, D2D1::ColorF color)
{
	for (UINT i = 0; i < kNumRenderTarget; ++i) {
		UINT id = (uid * kNumRenderTarget) + i;
		ComPtr<ID2D1SolidColorBrush> brush;
		if (m_RTTable.contains(id)) {
			ThrowIfFailed(m_RTTable[id]->CreateSolidColorBrush(color, brush.ReleaseAndGetAddressOf()));
			brushes[i] = brush;
		}
	}
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