#pragma once

#include "stdafx.h"

class JD3D11GlobalFactor {
public:
	static JD3D11GlobalFactor& GetInstance();

	ID3D11Device* GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() { return m_MainDeviceContext.Get(); }

	IDXGIFactory* GetFactory() { return m_Factory.Get(); }
private:
	JD3D11GlobalFactor();
	~JD3D11GlobalFactor() = default;

private:
	ComPtr<ID3D11Device5>				m_Device{};
	ComPtr<ID3D11DeviceContext4>		m_MainDeviceContext{};

	ComPtr<IDXGIFactory7>				m_Factory{};
};