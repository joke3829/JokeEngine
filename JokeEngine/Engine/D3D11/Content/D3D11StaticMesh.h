#pragma once
#include "Engine/JContent/JStaticMesh.h"


class JStaticMeshDX11 : public JStaticMesh {
public:
	JStaticMeshDX11(const char* name = nullptr, bool bloadFromFile = false);
	JStaticMeshDX11(XMFLOAT3 center = XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3 extent = XMFLOAT3(5.f, 5.f, 5.f), const char* name = nullptr);
	JStaticMeshDX11(XMFLOAT3 center = XMFLOAT3(0.f, 0.f, 0.f), float radius = 5.f, UINT sliceCount = 20, UINT stackCount = 20, int sorry = 0, const char* name = nullptr);
	JStaticMeshDX11(XMFLOAT3 center = XMFLOAT3(0.f, 0.f, 0.f), float width = 1.f, float height = 1.f, JSMPlane quad = quad_no, const char* name = nullptr);

	// 11: slot, 12: root parameter, 12는 parameter만 채우면 된다.
	void SetGPUBuffer(UINT parameter, JShaderStage stage = JS_NONE) {}

	void Render();
	void Render(void* command) {}
	void Render(UINT i);


protected:
	void LoadFromFile(const char* name) {}

private:
	template<class T>
	void CreateBufferByInfo(ID3D11Device* device, D3D11_BIND_FLAG bindflag, ComPtr<ID3D11Buffer>& outBuffer, T& data)
	{
		D3D11_BUFFER_DESC desc{
			.ByteWidth = static_cast<UINT>(data.size()) * sizeof(data[0]),
			.Usage = D3D11_USAGE_IMMUTABLE,
			.BindFlags = static_cast<UINT>(bindflag),
		};
		D3D11_SUBRESOURCE_DATA sdata{
			.pSysMem = data.data()
		};

		ThrowIfFailed(device->CreateBuffer(&desc, &sdata, outBuffer.GetAddressOf()));
	}

	void BuffersReady();

private:
	D3D11_PRIMITIVE_TOPOLOGY			m_d3dTopology{};

	UINT								m_strides[7]{};
	UINT								m_offsets[7]{};

	ComPtr<ID3D11Buffer>				m_VertexBuffer{};
	ComPtr<ID3D11Buffer>				m_ColorBuffer{};
	ComPtr<ID3D11Buffer>				m_NormalBuffer{};
	ComPtr<ID3D11Buffer>				m_TangentBuffer{};
	ComPtr<ID3D11Buffer>				m_BiTangentBuffer{};
	ComPtr<ID3D11Buffer>				m_TexCoord0Buffer{};
	ComPtr<ID3D11Buffer>				m_TexCoord1Buffer{};

	std::vector<ComPtr<ID3D11Buffer>>	m_vIndexBuffers{};
};