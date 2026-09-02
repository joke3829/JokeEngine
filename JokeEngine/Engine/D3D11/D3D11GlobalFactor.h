#pragma once

#include "stdafx.h"

class JD3D11GlobalFactor {
public:
	static JD3D11GlobalFactor* GetInstance();

	ID3D11Device5* GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext4* GetDeviceContext() { return m_MainDeviceContext.Get(); }

	IDXGIFactory7* GetFactory() { return m_Factory.Get(); }
private:
	JD3D11GlobalFactor();
	~JD3D11GlobalFactor();

private:
	static JD3D11GlobalFactor*			m_Instance;

	ComPtr<ID3D11Device5>				m_Device{};
	ComPtr<ID3D11DeviceContext4>		m_MainDeviceContext{};

	ComPtr<IDXGIFactory7>				m_Factory{};
};