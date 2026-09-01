#pragma once

#include "stdafx.h"

class JD2DTextEngine {
public:
	static JD2DTextEngine* GetInstance();

	void AddRTForD3D11Texture2D(const std::string& name, ID3D11Texture2D* texture2d);
	void AddRTForD3D12Resource() {}



protected:
	JD2DTextEngine();
	~JD2DTextEngine();

	static JD2DTextEngine* m_Instance;
	
	ComPtr<IDWriteFactory8> m_DWriteFactory{};
	ComPtr<ID2D1Factory8> m_D2DFactory{};

	std::unordered_map<std::string, ComPtr<ID2D1RenderTarget>> m_RTTable{};
};