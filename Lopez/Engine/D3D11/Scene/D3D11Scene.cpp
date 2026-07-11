#include "D3D11Scene.h"
#include "Engine/D3D11/D3D11GlobalFactor.h"

void JEngineSceneDX11::SetDXBuffer(UINT parameter, JShaderStage stage, UINT currentBufferIndex)
{
	auto* gFactor = JD3D11GlobalFactor::GetInstance();
	auto* context = gFactor->GetDeviceContext();

	ID3D11ShaderResourceView* srvs[] = { 
		m_WorldMatricesBufferSRV[currentBufferIndex].Get(), 
		m_CameraMatricesBufferSRV[currentBufferIndex].Get()
	};

	switch (stage) {
	case JS_VS:
		context->VSSetShaderResources(parameter, 2, srvs);
		break;
	case JS_PS:
		context->PSSetShaderResources(parameter, 2, srvs);
		break;
	case JS_GS:
		context->GSSetShaderResources(parameter, 2, srvs);
		break;
	case JS_HS:
		context->HSSetShaderResources(parameter, 2, srvs);
		break;
	case JS_DS:
		context->DSSetShaderResources(parameter, 2, srvs);
		break;
	case JS_CS:
		context->CSSetShaderResources(parameter, 2, srvs);
		break;
	default:
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("[DX11] {0}에서 잘못된 SetDXBuffer를 호출했습니다.", m_name.c_str());
#endif
		assert(0);
	}
}

void JEngineSceneDX11::UpdateBuffers(UINT currentBufferIndex)
{
	auto* device = JD3D11GlobalFactor::GetInstance()->GetDevice();
	auto* context = JD3D11GlobalFactor::GetInstance()->GetDeviceContext();

	UINT& ci = currentBufferIndex;
	// 버퍼 크기 최신화
	if (!m_WorldMatricesBuffer[ci] || m_nAlignWorldMatrices[ci] != m_WorldMatrices.size()) {
		D3D11_BUFFER_DESC desc{
			.ByteWidth = static_cast<UINT>(m_WorldMatrices.size()) * sizeof(XMFLOAT4X4),
			.Usage = D3D11_USAGE_DYNAMIC,
			.BindFlags = D3D11_BIND_SHADER_RESOURCE,
			.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
			.StructureByteStride = sizeof(XMFLOAT4X4)
		};
		ThrowIfFailed(device->CreateBuffer(&desc, nullptr, m_WorldMatricesBuffer[ci].ReleaseAndGetAddressOf()));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{
			.Format = DXGI_FORMAT_UNKNOWN,
			.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX
		};
		srvDesc.BufferEx.FirstElement = 0;
		srvDesc.BufferEx.NumElements = m_WorldMatrices.size();
		srvDesc.BufferEx.Flags = 0;
		ThrowIfFailed(device->CreateShaderResourceView(m_WorldMatricesBuffer[ci].Get(), &srvDesc, m_WorldMatricesBufferSRV[ci].ReleaseAndGetAddressOf()));
		m_nAlignWorldMatrices[ci] = m_WorldMatrices.size();
	}

	if (!m_CameraMatricesBuffer[ci] || m_nAlignCameraMatrices[ci] != m_CameraMatrices.size()) {
		D3D11_BUFFER_DESC desc{
			.ByteWidth = static_cast<UINT>(m_CameraMatrices.size()) * sizeof(XMFLOAT4X4),
			.Usage = D3D11_USAGE_DYNAMIC,
			.BindFlags = D3D11_BIND_SHADER_RESOURCE,
			.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
			.StructureByteStride = sizeof(XMFLOAT4X4)
		};
		ThrowIfFailed(device->CreateBuffer(&desc, nullptr, m_CameraMatricesBuffer[ci].ReleaseAndGetAddressOf()));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{
			.Format = DXGI_FORMAT_UNKNOWN,
			.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX
		};
		srvDesc.BufferEx.FirstElement = 0;
		srvDesc.BufferEx.NumElements = m_CameraMatrices.size();
		srvDesc.BufferEx.Flags = 0;
		ThrowIfFailed(device->CreateShaderResourceView(m_CameraMatricesBuffer[ci].Get(), &srvDesc, m_CameraMatricesBufferSRV[ci].ReleaseAndGetAddressOf()));
		m_nAlignCameraMatrices[ci] = m_CameraMatrices.size();

		m_CameraMatrices.clear();
		m_CameraMatrices.assign(m_nAlignCameraMatrices[ci], {});
	}


	for (UINT i : m_CameraIndex) {
		std::shared_ptr<JCameraObject> p = std::dynamic_pointer_cast<JCameraObject>(m_Objects[i]);
#if defined(_DEBUG) || defined(DEBUG)
		if (!p) {
			spdlog::error("{0}에서 잘못된 JCameraObject를 호출했습니다.", m_name.c_str());
			assert(0);
		}
#endif
		m_CameraMatrices[i] = p->GetViewProjMatrix();
	}

	// 버퍼 업데이트
	D3D11_MAPPED_SUBRESOURCE mapdata{};
	ThrowIfFailed(context->Map(m_WorldMatricesBuffer[ci].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapdata));
	memcpy(mapdata.pData, m_WorldMatrices.data(), sizeof(XMFLOAT4X4) * m_nAlignWorldMatrices[ci]);
	context->Unmap(m_WorldMatricesBuffer[ci].Get(), 0);

	ThrowIfFailed(context->Map(m_CameraMatricesBuffer[ci].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapdata));
	memcpy(mapdata.pData, m_CameraMatrices.data(), sizeof(XMFLOAT4X4) * m_nAlignCameraMatrices[ci]);
	context->Unmap(m_CameraMatricesBuffer[ci].Get(), 0);
}

// ============================================================

void JEngineDefaultSceneDX11::Initialize()
{
	BuildDefaultScene();
}

void JEngineDefaultSceneDX11::SetDXBuffer(UINT parameter, JShaderStage stage, UINT currentBufferIndex)
{
	JEngineSceneDX11::SetDXBuffer(parameter, stage, currentBufferIndex);

	auto* gFactor = JD3D11GlobalFactor::GetInstance();
	auto* context = gFactor->GetDeviceContext();

	switch (stage) {
	case JS_VS:
		context->VSSetConstantBuffers(parameter, 1, m_SceneConstantBuffer[currentBufferIndex].GetAddressOf());
		break;
	case JS_PS:
		context->PSSetConstantBuffers(parameter, 1, m_SceneConstantBuffer[currentBufferIndex].GetAddressOf());
		break;
	case JS_GS:
		context->GSSetConstantBuffers(parameter, 1, m_SceneConstantBuffer[currentBufferIndex].GetAddressOf());
		break;
	case JS_HS:
		context->HSSetConstantBuffers(parameter, 1, m_SceneConstantBuffer[currentBufferIndex].GetAddressOf());
		break;
	case JS_DS:
		context->DSSetConstantBuffers(parameter, 1, m_SceneConstantBuffer[currentBufferIndex].GetAddressOf());
		break;
	case JS_CS:
		context->CSSetConstantBuffers(parameter, 1, m_SceneConstantBuffer[currentBufferIndex].GetAddressOf());
		break;
	default:
#if defined(_DEBUG) || defined(DEBUG)
		spdlog::error("[DX11] {0}에서 잘못된 SetDXBuffer를 호출했습니다.", m_name.c_str());
#endif
		assert(0);
	}
}

void JEngineDefaultSceneDX11::UpdateBuffers(UINT currentBufferIndex)
{
	JEngineSceneDX11::UpdateBuffers(currentBufferIndex);
}