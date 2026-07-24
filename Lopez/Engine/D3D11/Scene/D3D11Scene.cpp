#include "D3D11Scene.h"
#include "Engine/D3D11/D3D11GlobalFactor.h"
#include "Engine/JObject/JCameraObject.h"

void JEngineSceneDX11::SetDXBuffer(UINT currentBufferIndex, UINT parameter, JShaderStage stage)
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
			.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
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

	if (!m_CameraMatricesBuffer[ci] || m_nAlignCameraMatrices[ci] != m_Cameras.size()) {
		D3D11_BUFFER_DESC desc{
			.ByteWidth = static_cast<UINT>(m_Cameras.size()) * sizeof(XMFLOAT4X4),
			.Usage = D3D11_USAGE_DYNAMIC,
			.BindFlags = D3D11_BIND_SHADER_RESOURCE,
			.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
			.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
			.StructureByteStride = sizeof(XMFLOAT4X4)
		};
		ThrowIfFailed(device->CreateBuffer(&desc, nullptr, m_CameraMatricesBuffer[ci].ReleaseAndGetAddressOf()));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{
			.Format = DXGI_FORMAT_UNKNOWN,
			.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX
		};
		srvDesc.BufferEx.FirstElement = 0;
		srvDesc.BufferEx.NumElements = m_Cameras.size();
		srvDesc.BufferEx.Flags = 0;
		ThrowIfFailed(device->CreateShaderResourceView(m_CameraMatricesBuffer[ci].Get(), &srvDesc, m_CameraMatricesBufferSRV[ci].ReleaseAndGetAddressOf()));
		m_nAlignCameraMatrices[ci] = m_Cameras.size();

		m_CameraMatrices.clear();
		m_CameraMatrices.assign(m_nAlignCameraMatrices[ci], {});
	}


	for (UINT i = 0; i < m_Cameras.size(); ++i) {
		m_CameraMatrices[i] = m_Cameras[i]->GetViewProjMatrix();
	}

	// 버퍼 업데이트
	D3D11_MAPPED_SUBRESOURCE mapdata{};
	ThrowIfFailed(context->Map(m_WorldMatricesBuffer[ci].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapdata));
	// test
	std::vector<XMFLOAT4X4> tworld{};
	{
		for (int i = 0; i < m_WorldMatrices.size(); ++i) {
			tworld.emplace_back();
			XMStoreFloat4x4(&tworld[i], XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrices[i])));
		}
	}

	memcpy(mapdata.pData, tworld.data(), sizeof(XMFLOAT4X4) * m_nAlignWorldMatrices[ci]);
	context->Unmap(m_WorldMatricesBuffer[ci].Get(), 0);

	ThrowIfFailed(context->Map(m_CameraMatricesBuffer[ci].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapdata));

	std::vector<XMFLOAT4X4> tcamera{};
	{
		for (int i = 0; i < m_CameraMatrices.size(); ++i) {
			tcamera.emplace_back();
			XMStoreFloat4x4(&tcamera[i], XMMatrixTranspose(XMLoadFloat4x4(&m_CameraMatrices[i])));
		}
	}
	memcpy(mapdata.pData, tcamera.data(), sizeof(XMFLOAT4X4) * m_nAlignCameraMatrices[ci]);
	context->Unmap(m_CameraMatricesBuffer[ci].Get(), 0);
}

// ============================================================


void JEngineDefaultSceneDX11::SetDXBuffer(UINT currentBufferIndex, UINT parameter, JShaderStage stage)
{
	JEngineSceneDX11::SetDXBuffer(currentBufferIndex, 0, stage);

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

	auto* device = JD3D11GlobalFactor::GetInstance()->GetDevice();
	auto* context = JD3D11GlobalFactor::GetInstance()->GetDeviceContext();
	UINT& ci = currentBufferIndex;
	if (!m_SceneConstantBuffer[ci]) {
		D3D11_BUFFER_DESC desc{
			.ByteWidth = Align(sizeof(DefaultSceneConstant), 16),
			.Usage = D3D11_USAGE_DYNAMIC,
			.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
			.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
		};
		ThrowIfFailed(device->CreateBuffer(&desc, nullptr, m_SceneConstantBuffer[ci].ReleaseAndGetAddressOf()));
	}

	// Scene Constant Update
	D3D11_MAPPED_SUBRESOURCE data{};
	context->Map(m_SceneConstantBuffer[ci].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	memcpy(data.pData, &m_SceneConstant, sizeof(DefaultSceneConstant));
	context->Unmap(m_SceneConstantBuffer[ci].Get(), 0);

	// 카메라에 대한 뷰포트를 지금 여기서 설정할까
	// 이게 이 씬은 멀티 뷰포트를 상정하지 않음

	std::vector<D3D11_VIEWPORT> viewports(m_Cameras.size(), {});
	std::vector<D3D11_RECT> scissorrect(m_Cameras.size(), {});
	for (UINT i = 0; i < viewports.size(); ++i) {
		auto& v = m_Cameras[i]->GetViewport();
		D3D11_VIEWPORT viewport = {
			.TopLeftX = v.TopLeftX,
			.TopLeftY = v.TopLeftY,
			.Width = v.Width,
			.Height = v.Height,
			.MinDepth = v.MinDepth,
			.MaxDepth = v.MaxDepth
		};
		viewports[i] = viewport;

		scissorrect[i].left = v.TopLeftX;
		scissorrect[i].top = v.TopLeftY;
		scissorrect[i].right = v.TopLeftX + v.Width;
		scissorrect[i].bottom = v.TopLeftY + v.Height;
	}

	context->RSSetViewports(viewports.size(), viewports.data());
	context->RSSetScissorRects(scissorrect.size(), scissorrect.data());
}