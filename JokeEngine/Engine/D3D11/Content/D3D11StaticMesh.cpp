#include "D3D11StaticMesh.h"
#include "../D3D11GlobalFactor.h"



JStaticMeshDX11::JStaticMeshDX11(const char* name, bool bloadFromFile)
	: JStaticMesh(name, bloadFromFile)
{
}

JStaticMeshDX11::JStaticMeshDX11(XMFLOAT3 center, XMFLOAT3 extent, const char* name)
	: JStaticMesh(center, extent, name)
{
	m_d3dTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	BuffersReady();
}

JStaticMeshDX11::JStaticMeshDX11(XMFLOAT3 center, float radius, UINT sliceCount, UINT stackCount, int sorry, const char* name)
	: JStaticMesh(center, radius, sliceCount, stackCount, sorry, name)
{
	m_d3dTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	BuffersReady();
}

JStaticMeshDX11::JStaticMeshDX11(XMFLOAT3 center, float width, float height, JSMPlane quad, const char* name)
	: JStaticMesh(center, width, height, quad, name)
{
	m_d3dTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	BuffersReady();
}

void JStaticMeshDX11::BuffersReady()
{
	auto* gFactor = JD3D11GlobalFactor::GetInstance();
	auto* device = gFactor->GetDevice();

	CreateBufferByInfo(device, D3D11_BIND_VERTEX_BUFFER, m_VertexBuffer, m_Vertices);
	CreateBufferByInfo(device, D3D11_BIND_VERTEX_BUFFER, m_ColorBuffer, m_Colors);
	CreateBufferByInfo(device, D3D11_BIND_VERTEX_BUFFER, m_NormalBuffer, m_Normals);
	CreateBufferByInfo(device, D3D11_BIND_VERTEX_BUFFER, m_TangentBuffer, m_Tangents);
	CreateBufferByInfo(device, D3D11_BIND_VERTEX_BUFFER, m_BiTangentBuffer, m_BiTangents);
	CreateBufferByInfo(device, D3D11_BIND_VERTEX_BUFFER, m_TexCoord0Buffer, m_TexCoord0);
	CreateBufferByInfo(device, D3D11_BIND_VERTEX_BUFFER, m_TexCoord1Buffer, m_TexCoord1);

	m_strides[0] = m_Vertices.size() > 1 ? sizeof(XMFLOAT3) : 0;
	m_strides[1] = m_Colors.size() > 1 ? sizeof(XMFLOAT4) : 0;
	m_strides[2] = m_Normals.size() > 1 ? sizeof(XMFLOAT3) : 0;
	m_strides[3] = m_Tangents.size() > 1 ? sizeof(XMFLOAT3) : 0;
	m_strides[4] = m_BiTangents.size() > 1 ? sizeof(XMFLOAT3) : 0;
	m_strides[5] = m_TexCoord0.size() > 1 ? sizeof(XMFLOAT2) : 0;
	m_strides[6] = m_TexCoord1.size() > 1 ? sizeof(XMFLOAT2) : 0;

	// index 버퍼 ready
	for (size_t i = 0; i < m_vIndices.size(); ++i) {
		ComPtr<ID3D11Buffer> indexbuffer{};
		CreateBufferByInfo(device, D3D11_BIND_INDEX_BUFFER, indexbuffer, m_vIndices[i]);
		m_vIndexBuffers.emplace_back(indexbuffer);
	}
}

void JStaticMeshDX11::Render()
{
	auto* gFactor = JD3D11GlobalFactor::GetInstance();
	auto* context = gFactor->GetDeviceContext();

	context->IASetPrimitiveTopology(m_d3dTopology);

	ID3D11Buffer* buffers[] = {
		m_VertexBuffer.Get(),
		m_ColorBuffer.Get(),
		m_NormalBuffer.Get(),
		m_TangentBuffer.Get(),
		m_BiTangentBuffer.Get(),
		m_TexCoord0Buffer.Get(),
		m_TexCoord1Buffer.Get()
	};

	context->IASetVertexBuffers(0, 7, buffers, m_strides, m_offsets);

	for (size_t i = 0; i < m_vIndexBuffers.size(); ++i) {
		context->IASetIndexBuffer(m_vIndexBuffers[i].Get(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(m_vIndices[i].size(), 0, 0);
	}
}

// parameter 'i' is submesh index
void JStaticMeshDX11::Render(UINT i)
{
	auto* gFactor = JD3D11GlobalFactor::GetInstance();
	auto* context = gFactor->GetDeviceContext();

	context->IASetPrimitiveTopology(m_d3dTopology);

	ID3D11Buffer* buffers[] = {
		m_VertexBuffer.Get(),
		m_ColorBuffer.Get(),
		m_NormalBuffer.Get(),
		m_TangentBuffer.Get(),
		m_BiTangentBuffer.Get(),
		m_TexCoord0Buffer.Get(),
		m_TexCoord1Buffer.Get()
	};

	context->IASetVertexBuffers(0, 7, buffers, m_strides, m_offsets);

	context->IASetIndexBuffer(m_vIndexBuffers[i].Get(), DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(m_vIndices[i].size(), 0, 0);
}

