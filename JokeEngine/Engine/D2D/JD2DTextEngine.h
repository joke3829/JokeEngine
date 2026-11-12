#pragma once

#include "stdafx.h"

class JD2DTextEngine {
public:
	static JD2DTextEngine* GetInstance();

	void AddRTsForD3D11Texture2D(UINT uid, ID3D11Texture2D** texture2d);
	void AddRTsForD3D12Resource() {}

	void DeleteInRTTable(UINT uid);


	void RenderText(UINT uid, UINT currentFrameIndex, IDWriteTextLayout* layout, ID2D1Brush* brush);

	void CreateTextFormat(ComPtr<IDWriteTextFormat3>& format, std::wstring& fontname, std::string& fontcollection, float fontsize, float fontweight, float fontwidth);
	void CreateTextLayout(ComPtr<IDWriteTextLayout>& layout, ComPtr<IDWriteTextFormat3>& format, std::wstring& text, float maxwidth, float maxheight);

	void CreateSolidColorBrush(ComPtr<ID2D1Brush>* brushes, UINT uid, XMFLOAT4 color = XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	void CreateSolidColorBrush(ComPtr<ID2D1Brush>* brushes, UINT uid, D2D1::ColorF color = D2D1::ColorF::White);


protected:
	JD2DTextEngine();
	~JD2DTextEngine();

	static JD2DTextEngine* m_Instance;
	
	ComPtr<IDWriteFactory8> m_DWriteFactory{};
	ComPtr<ID2D1Factory8> m_D2DFactory{};

	std::unordered_map<UINT, ComPtr<ID2D1RenderTarget>> m_RTTable{};
};