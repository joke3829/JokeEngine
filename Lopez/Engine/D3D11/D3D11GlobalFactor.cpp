#include "D3D11GlobalFactor.h"

JD3D11GlobalFactor* JD3D11GlobalFactor::m_Instance = nullptr;


JD3D11GlobalFactor* JD3D11GlobalFactor::GetInstance()
{
	if (!m_Instance)
		m_Instance = new JD3D11GlobalFactor;
	return m_Instance;
}


JD3D11GlobalFactor::JD3D11GlobalFactor()
{
	// 팩토리 생성
	CreateDXGIFactory(IID_PPV_ARGS(m_Factory.GetAddressOf()));

	// Device & MainDeviceContext(Immediate) 생성

	ComPtr<ID3D11Device> device{};
	ComPtr<ID3D11DeviceContext> context{};

	D3D_FEATURE_LEVEL featurelevel[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	UINT device_flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG) || defined(DEBUG)
	device_flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = D3D11CreateDevice(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		device_flag,
		featurelevel,
		ARRAYSIZE(featurelevel),
		D3D11_SDK_VERSION,
		device.GetAddressOf(),
		nullptr,
		context.GetAddressOf());

	if (FAILED(hr)) {	// 실패하면 가상 머신으로 한번 더 시도
		ThrowIfFailed(D3D11CreateDevice(nullptr,
			D3D_DRIVER_TYPE_WARP,
			nullptr,
			device_flag,
			featurelevel,
			ARRAYSIZE(featurelevel),
			D3D11_SDK_VERSION,
			device.GetAddressOf(),
			nullptr,
			context.GetAddressOf()));
	}

	ThrowIfFailed(device->QueryInterface(IID_PPV_ARGS(m_Device.GetAddressOf())));
	ThrowIfFailed(context->QueryInterface(IID_PPV_ARGS(m_MainDeviceContext.GetAddressOf())));
#if defined(_DEBUG) || defined(DEBUG)
	spdlog::info("ID3D11Device Create Success");
#endif
	
}

JD3D11GlobalFactor::~JD3D11GlobalFactor()
{
	if (m_Instance) delete m_Instance;
}