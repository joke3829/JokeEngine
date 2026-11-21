
#include "JStaticMesh.h"
#include "JContentManager.h"

namespace {
    D3D_PRIMITIVE_TOPOLOGY ConvertPrimitiveTopologyIntToD3D(unsigned int pt)
    {
        return D3D_PRIMITIVE_TOPOLOGY(pt);
    }

    unsigned int ConvertPrimitiveTopologyD3DToInt(D3D_PRIMITIVE_TOPOLOGY pt) 
    {
        return static_cast<unsigned int>(pt);
    }
}



// 박스 생성자, 
JStaticMesh::JStaticMesh(XMFLOAT3 center, XMFLOAT3 extent, const char* name)
    : JContent(name)
{
    m_Vertices.reserve(24);
    m_Normals.reserve(24);
    m_Tangents.reserve(24);
    m_BiTangents.reserve(24);
    m_TexCoord0.reserve(24);
    auto& index = m_vIndices.emplace_back();
    index.reserve(36);

    float minX = center.x - extent.x;
    float maxX = center.x + extent.x;
    float minY = center.y - extent.y;
    float maxY = center.y + extent.y;
    float minZ = center.z - extent.z;
    float maxZ = center.z + extent.z;


    m_Vertices.emplace_back(XMFLOAT3(minX, minY, minZ)); m_Normals.emplace_back(XMFLOAT3(0.0f, 0.0f, -1.0f)); m_Tangents.emplace_back(XMFLOAT3(1.0f, 0.0f, 0.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_TexCoord0.emplace_back(XMFLOAT2(0.0f, 1.0f)); // 0: 좌하
    m_Vertices.emplace_back(XMFLOAT3(minX, maxY, minZ)); m_Normals.emplace_back(XMFLOAT3(0.0f, 0.0f, -1.0f)); m_Tangents.emplace_back(XMFLOAT3(1.0f, 0.0f, 0.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_TexCoord0.emplace_back(XMFLOAT2(0.0f, 0.0f)); // 1: 좌상
    m_Vertices.emplace_back(XMFLOAT3(maxX, maxY, minZ)); m_Normals.emplace_back(XMFLOAT3(0.0f, 0.0f, -1.0f)); m_Tangents.emplace_back(XMFLOAT3(1.0f, 0.0f, 0.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_TexCoord0.emplace_back(XMFLOAT2(1.0f, 0.0f)); // 2: 우상
    m_Vertices.emplace_back(XMFLOAT3(maxX, minY, minZ)); m_Normals.emplace_back(XMFLOAT3(0.0f, 0.0f, -1.0f)); m_Tangents.emplace_back(XMFLOAT3(1.0f, 0.0f, 0.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_TexCoord0.emplace_back(XMFLOAT2(1.0f, 1.0f)); // 3: 우하

    m_Vertices.emplace_back(XMFLOAT3(maxX, minY, minZ)); m_Normals.emplace_back(XMFLOAT3(1.0f, 0.0f, 0.0f)); m_Tangents.emplace_back(XMFLOAT3(0.0f, 0.0f, 1.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_TexCoord0.emplace_back(XMFLOAT2(0.0f, 1.0f)); // 0: 좌하
    m_Vertices.emplace_back(XMFLOAT3(maxX, maxY, minZ)); m_Normals.emplace_back(XMFLOAT3(1.0f, 0.0f, 0.0f)); m_Tangents.emplace_back(XMFLOAT3(0.0f, 0.0f, 1.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_TexCoord0.emplace_back(XMFLOAT2(0.0f, 0.0f)); // 1: 좌상
    m_Vertices.emplace_back(XMFLOAT3(maxX, maxY, maxZ)); m_Normals.emplace_back(XMFLOAT3(1.0f, 0.0f, 0.0f)); m_Tangents.emplace_back(XMFLOAT3(0.0f, 0.0f, 1.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_TexCoord0.emplace_back(XMFLOAT2(1.0f, 0.0f)); // 2: 우상
    m_Vertices.emplace_back(XMFLOAT3(maxX, minY, maxZ)); m_Normals.emplace_back(XMFLOAT3(1.0f, 0.0f, 0.0f)); m_Tangents.emplace_back(XMFLOAT3(0.0f, 0.0f, 1.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_TexCoord0.emplace_back(XMFLOAT2(1.0f, 1.0f)); // 3: 우하

    m_Vertices.emplace_back(XMFLOAT3(maxX, minY, maxZ)); m_Normals.emplace_back(XMFLOAT3(0.0f, 0.0f, 1.0f)); m_Tangents.emplace_back(XMFLOAT3(-1.0f, 0.0f, 0.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_TexCoord0.emplace_back(XMFLOAT2(0.0f, 1.0f)); // 0: 좌하
    m_Vertices.emplace_back(XMFLOAT3(maxX, maxY, maxZ)); m_Normals.emplace_back(XMFLOAT3(0.0f, 0.0f, 1.0f)); m_Tangents.emplace_back(XMFLOAT3(-1.0f, 0.0f, 0.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_TexCoord0.emplace_back(XMFLOAT2(0.0f, 0.0f)); // 1: 좌상
    m_Vertices.emplace_back(XMFLOAT3(minX, maxY, maxZ)); m_Normals.emplace_back(XMFLOAT3(0.0f, 0.0f, 1.0f)); m_Tangents.emplace_back(XMFLOAT3(-1.0f, 0.0f, 0.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_TexCoord0.emplace_back(XMFLOAT2(1.0f, 0.0f)); // 2: 우상
    m_Vertices.emplace_back(XMFLOAT3(minX, minY, maxZ)); m_Normals.emplace_back(XMFLOAT3(0.0f, 0.0f, 1.0f)); m_Tangents.emplace_back(XMFLOAT3(-1.0f, 0.0f, 0.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_TexCoord0.emplace_back(XMFLOAT2(1.0f, 1.0f)); // 3: 우하

    m_Vertices.emplace_back(XMFLOAT3(minX, minY, maxZ)); m_Normals.emplace_back(XMFLOAT3(-1.0f, 0.0f, 0.0f)); m_Tangents.emplace_back(XMFLOAT3(0.0f, 0.0f, -1.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_TexCoord0.emplace_back(XMFLOAT2(0.0f, 1.0f)); // 0: 좌하
    m_Vertices.emplace_back(XMFLOAT3(minX, maxY, maxZ)); m_Normals.emplace_back(XMFLOAT3(-1.0f, 0.0f, 0.0f)); m_Tangents.emplace_back(XMFLOAT3(0.0f, 0.0f, -1.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_TexCoord0.emplace_back(XMFLOAT2(0.0f, 0.0f)); // 1: 좌상
    m_Vertices.emplace_back(XMFLOAT3(minX, maxY, minZ)); m_Normals.emplace_back(XMFLOAT3(-1.0f, 0.0f, 0.0f)); m_Tangents.emplace_back(XMFLOAT3(0.0f, 0.0f, -1.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_TexCoord0.emplace_back(XMFLOAT2(1.0f, 0.0f)); // 2: 우상
    m_Vertices.emplace_back(XMFLOAT3(minX, minY, minZ)); m_Normals.emplace_back(XMFLOAT3(-1.0f, 0.0f, 0.0f)); m_Tangents.emplace_back(XMFLOAT3(0.0f, 0.0f, -1.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_TexCoord0.emplace_back(XMFLOAT2(1.0f, 1.0f)); // 3: 우하

    m_Vertices.emplace_back(XMFLOAT3(minX, maxY, minZ)); m_Normals.emplace_back(XMFLOAT3(0.0f, 1.0f, 0.0f)); m_Tangents.emplace_back(XMFLOAT3(1.0f, 0.0f, 0.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, 0.0f, -1.0f)); m_TexCoord0.emplace_back(XMFLOAT2(0.0f, 1.0f)); // 0: 좌하
    m_Vertices.emplace_back(XMFLOAT3(minX, maxY, maxZ)); m_Normals.emplace_back(XMFLOAT3(0.0f, 1.0f, 0.0f)); m_Tangents.emplace_back(XMFLOAT3(1.0f, 0.0f, 0.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, 0.0f, -1.0f)); m_TexCoord0.emplace_back(XMFLOAT2(0.0f, 0.0f)); // 1: 좌상
    m_Vertices.emplace_back(XMFLOAT3(maxX, maxY, maxZ)); m_Normals.emplace_back(XMFLOAT3(0.0f, 1.0f, 0.0f)); m_Tangents.emplace_back(XMFLOAT3(1.0f, 0.0f, 0.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, 0.0f, -1.0f)); m_TexCoord0.emplace_back(XMFLOAT2(1.0f, 0.0f)); // 2: 우상
    m_Vertices.emplace_back(XMFLOAT3(maxX, maxY, minZ)); m_Normals.emplace_back(XMFLOAT3(0.0f, 1.0f, 0.0f)); m_Tangents.emplace_back(XMFLOAT3(1.0f, 0.0f, 0.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, 0.0f, -1.0f)); m_TexCoord0.emplace_back(XMFLOAT2(1.0f, 1.0f)); // 3: 우하

    m_Vertices.emplace_back(XMFLOAT3(maxX, minY, minZ)); m_Normals.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_Tangents.emplace_back(XMFLOAT3(-1.0f, 0.0f, 0.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, 0.0f, -1.0f)); m_TexCoord0.emplace_back(XMFLOAT2(1.0f, 0.0f)); // 0: 좌하
    m_Vertices.emplace_back(XMFLOAT3(maxX, minY, maxZ)); m_Normals.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_Tangents.emplace_back(XMFLOAT3(-1.0f, 0.0f, 0.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, 0.0f, -1.0f)); m_TexCoord0.emplace_back(XMFLOAT2(1.0f, 1.0f)); // 1: 좌상
    m_Vertices.emplace_back(XMFLOAT3(minX, minY, maxZ)); m_Normals.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_Tangents.emplace_back(XMFLOAT3(-1.0f, 0.0f, 0.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, 0.0f, -1.0f)); m_TexCoord0.emplace_back(XMFLOAT2(0.0f, 1.0f)); // 2: 우상
    m_Vertices.emplace_back(XMFLOAT3(minX, minY, minZ)); m_Normals.emplace_back(XMFLOAT3(0.0f, -1.0f, 0.0f)); m_Tangents.emplace_back(XMFLOAT3(-1.0f, 0.0f, 0.0f)); m_BiTangents.emplace_back(XMFLOAT3(0.0f, 0.0f, -1.0f)); m_TexCoord0.emplace_back(XMFLOAT2(0.0f, 0.0f)); // 3: 우하


    for (UINT i = 0; i < 24; i += 4)
    {
        index.emplace_back(i + 0);
        index.emplace_back(i + 1);
        index.emplace_back(i + 3);

        index.emplace_back(i + 1);
        index.emplace_back(i + 2);
        index.emplace_back(i + 3);
    }

    m_Colors.emplace_back(XMFLOAT4(1.f, 1.f, 1.f, 1.f));
    m_TexCoord1.emplace_back(XMFLOAT2());
    m_MeshOwnerFlag =
        MeshOwnerFlag_HasVertex |
        MeshOwnerFlag_HasNormal |
        MeshOwnerFlag_HasTangent |
        MeshOwnerFlag_HasBiTangent |
        MeshOwnerFlag_HasTexCoord0 |
        MeshOwnerFlag_HasIndices;
}

// 구(Sphere) 생성자, 
JStaticMesh::JStaticMesh(XMFLOAT3 center, float radius, UINT sliceCount, UINT stackCount, int sorry, const char* name)
    : JContent(name)
{
    UINT numVertices = 2 + (stackCount - 1) * (sliceCount + 1);
    UINT numIndices = sliceCount * 3 + (stackCount - 2) * sliceCount * 6 + sliceCount * 3;

    m_Vertices.reserve(numVertices);
    m_Normals.reserve(numVertices);
    m_Tangents.reserve(numVertices);
    m_BiTangents.reserve(numVertices);
    m_TexCoord0.reserve(numVertices);
    auto& index = m_vIndices.emplace_back();
    index.reserve(numIndices);

    m_Vertices.emplace_back(center.x, center.y + radius, center.z);
    m_Normals.emplace_back(0.0f, 1.0f, 0.0f);
    m_Tangents.emplace_back(1.0f, 0.0f, 0.0f);
    m_BiTangents.emplace_back(0.0f, 0.0f, 1.0f);
    m_TexCoord0.emplace_back(0.0f, 0.0f);

    float phiStep = XM_PI / stackCount;
    float thetaStep = XM_2PI / sliceCount;

    for (UINT i = 1; i <= stackCount - 1; ++i)
    {
        float phi = i * phiStep;
        for (UINT j = 0; j <= sliceCount; ++j)
        {
            float theta = j * thetaStep;

            float x = sinf(phi) * sinf(theta);
            float y = cosf(phi);
            float z = sinf(phi) * cosf(theta);

            m_Vertices.emplace_back(center.x + radius * x, center.y + radius * y, center.z + radius * z);
            m_Normals.emplace_back(x, y, z);

            float tx = sinf(phi) * cosf(theta);
            float ty = 0.0f;
            float tz = -sinf(phi) * sinf(theta);
            XMVECTOR T = XMVector3Normalize(XMVectorSet(tx, ty, tz, 0.0f));
            XMFLOAT3 tangent;
            XMStoreFloat3(&tangent, T);
            m_Tangents.emplace_back(tangent);

            XMVECTOR N = XMVectorSet(x, y, z, 0.0f);
            XMVECTOR B = XMVector3Cross(N, T);
            XMFLOAT3 bitangent;
            XMStoreFloat3(&bitangent, B);
            m_BiTangents.emplace_back(bitangent);

            m_TexCoord0.emplace_back((float)j / sliceCount, (float)i / stackCount);
        }
    }

    m_Vertices.emplace_back(center.x, center.y - radius, center.z);
    m_Normals.emplace_back(0.0f, -1.0f, 0.0f);
    m_Tangents.emplace_back(1.0f, 0.0f, 0.0f);
    m_BiTangents.emplace_back(0.0f, 0.0f, -1.0f);
    m_TexCoord0.emplace_back(0.0f, 1.0f);

    // 북극
    for (UINT i = 1; i <= sliceCount; ++i)
    {
        index.emplace_back(0);
        index.emplace_back(i);
        index.emplace_back(i + 1);
    }

    UINT baseIndex = 1;
    UINT ringVertexCount = sliceCount + 1;
    for (UINT i = 0; i < stackCount - 2; ++i)
    {
        for (UINT j = 0; j < sliceCount; ++j)
        {
            index.emplace_back(baseIndex + i * ringVertexCount + j);
            index.emplace_back(baseIndex + (i + 1) * ringVertexCount + j);
            index.emplace_back(baseIndex + i * ringVertexCount + j + 1);

            index.emplace_back(baseIndex + i * ringVertexCount + j + 1);
            index.emplace_back(baseIndex + (i + 1) * ringVertexCount + j);
            index.emplace_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
        }
    }

    // 남극
    UINT southPoleIndex = (UINT)m_Vertices.size() - 1;
    baseIndex = southPoleIndex - ringVertexCount;
    for (UINT i = 0; i < sliceCount; ++i)
    {
        index.emplace_back(southPoleIndex);
        index.emplace_back(baseIndex + i + 1);
        index.emplace_back(baseIndex + i);
    }

    m_Colors.emplace_back(XMFLOAT4(1.f, 1.f, 1.f, 1.f));
    m_TexCoord1.emplace_back(XMFLOAT2());

    m_MeshOwnerFlag =
        MeshOwnerFlag_HasVertex |
        MeshOwnerFlag_HasNormal |
        MeshOwnerFlag_HasTangent |
        MeshOwnerFlag_HasBiTangent |
        MeshOwnerFlag_HasTexCoord0 |
        MeshOwnerFlag_HasIndices;
}

// 평면 생성, 
JStaticMesh::JStaticMesh(XMFLOAT3 center, float width, float height, JSMPlane quad, const char* name)
    : JContent(name)
{
    m_Vertices.reserve(4);
    m_Normals.reserve(4);
    m_Tangents.reserve(4);
    m_BiTangents.reserve(4);
    m_TexCoord0.reserve(4);
    auto& index = m_vIndices.emplace_back();
    index.reserve(6);

    float halfW = width * 0.5f;
    float halfH = height * 0.5f;

    XMFLOAT3 mcenter = center;

    if (quad == quad1) {
        mcenter.x += halfW;
        mcenter.y += halfH;
    }
    else if (quad == quad2) {
        mcenter.x -= halfW;
        mcenter.y += halfH;
    }
    else if (quad == quad3) {
        mcenter.x -= halfW;
        mcenter.y -= halfH;
    }
    else if (quad == quad4) {
        mcenter.x += halfW;
        mcenter.y -= halfH;
    }


    m_Vertices.emplace_back(mcenter.x - halfW, mcenter.y - halfH, mcenter.z);
    m_Normals.emplace_back(0.0f, 0.0f, -1.0f);
    m_Tangents.emplace_back(1.0f, 0.0f, 0.0f);
    m_BiTangents.emplace_back(0.0f, -1.0f, 0.0f);
    m_TexCoord0.emplace_back(0.0f, 1.0f);

    m_Vertices.emplace_back(mcenter.x - halfW, mcenter.y + halfH, mcenter.z);
    m_Normals.emplace_back(0.0f, 0.0f, -1.0f);
    m_Tangents.emplace_back(1.0f, 0.0f, 0.0f);
    m_BiTangents.emplace_back(0.0f, -1.0f, 0.0f);
    m_TexCoord0.emplace_back(0.0f, 0.0f);

    m_Vertices.emplace_back(mcenter.x + halfW, mcenter.y + halfH, mcenter.z);
    m_Normals.emplace_back(0.0f, 0.0f, -1.0f);
    m_Tangents.emplace_back(1.0f, 0.0f, 0.0f);
    m_BiTangents.emplace_back(0.0f, -1.0f, 0.0f);
    m_TexCoord0.emplace_back(1.0f, 0.0f);

    m_Vertices.emplace_back(mcenter.x + halfW, mcenter.y - halfH, mcenter.z);
    m_Normals.emplace_back(0.0f, 0.0f, -1.0f);
    m_Tangents.emplace_back(1.0f, 0.0f, 0.0f);
    m_BiTangents.emplace_back(0.0f, -1.0f, 0.0f);
    m_TexCoord0.emplace_back(1.0f, 1.0f);

    index.emplace_back(0);
    index.emplace_back(1);
    index.emplace_back(3);

    index.emplace_back(1);
    index.emplace_back(2);
    index.emplace_back(3);

    m_Colors.emplace_back(XMFLOAT4(1.f, 1.f, 1.f, 1.f));
    m_TexCoord1.emplace_back(XMFLOAT2());

    m_MeshOwnerFlag =
        MeshOwnerFlag_HasVertex |
        MeshOwnerFlag_HasNormal |
        MeshOwnerFlag_HasTangent |
        MeshOwnerFlag_HasBiTangent |
        MeshOwnerFlag_HasTexCoord0 |
        MeshOwnerFlag_HasIndices;
}

void JStaticMesh::LoadFromFile()
{
    std::ifstream in{ m_FullFilePath, std::ios::binary };
    unsigned int num{};

    // Primitive Topology
    in.read(reinterpret_cast<char*>(&num), sizeof(unsigned int));
    m_d3dTopology = ConvertPrimitiveTopologyIntToD3D(num);

    // vertex
    in.read(reinterpret_cast<char*>(&num), sizeof(unsigned int));
    if (num > 0) {
        m_Vertices.assign(num, {});
        in.read(reinterpret_cast<char*>(m_Vertices.data()), sizeof(XMFLOAT3) * num);
        m_MeshOwnerFlag |= MeshOwnerFlag_HasVertex;
    }
    else { // 사실 정점이 없으면 메시 자체가 의미가 없는거긴 한데 일단 방어 넣기
        m_Vertices.emplace_back();
    }
    // color
    in.read(reinterpret_cast<char*>(&num), sizeof(unsigned int));
    if (num > 0) {
        m_Colors.assign(num, {});
        in.read(reinterpret_cast<char*>(m_Colors.data()), sizeof(XMFLOAT4) * num);
        m_MeshOwnerFlag |= MeshOwnerFlag_HasColor;
    }
    else {
        m_Colors.emplace_back();
    }
    // normal
    in.read(reinterpret_cast<char*>(&num), sizeof(unsigned int));
    if (num > 0) {
        m_Normals.assign(num, {});
        in.read(reinterpret_cast<char*>(m_Normals.data()), sizeof(XMFLOAT3) * num);
        m_MeshOwnerFlag |= MeshOwnerFlag_HasNormal;
    }
    else {
        m_Normals.emplace_back();
    }
    // tangent
    in.read(reinterpret_cast<char*>(&num), sizeof(unsigned int));
    if (num > 0) {
        m_Tangents.assign(num, {});
        in.read(reinterpret_cast<char*>(m_Tangents.data()), sizeof(XMFLOAT3) * num);
        m_MeshOwnerFlag |= MeshOwnerFlag_HasTangent;
    }
    else {
        m_Tangents.emplace_back();
    }
    // bitangent
    in.read(reinterpret_cast<char*>(&num), sizeof(unsigned int));
    if (num > 0) {
        m_BiTangents.assign(num, {});
        in.read(reinterpret_cast<char*>(m_BiTangents.data()), sizeof(XMFLOAT3) * num);
        m_MeshOwnerFlag |= MeshOwnerFlag_HasBiTangent;
    }
    else {
        m_BiTangents.emplace_back();
    }
    // tex0
    in.read(reinterpret_cast<char*>(&num), sizeof(unsigned int));
    if (num > 0) {
        m_TexCoord0.assign(num, {});
        in.read(reinterpret_cast<char*>(m_TexCoord0.data()), sizeof(XMFLOAT2) * num);
        m_MeshOwnerFlag |= MeshOwnerFlag_HasTexCoord0;
    }
    else {
        m_TexCoord0.emplace_back();
    }
    // tex1
    in.read(reinterpret_cast<char*>(&num), sizeof(unsigned int));
    if (num > 0) {
        m_TexCoord1.assign(num, {});
        in.read(reinterpret_cast<char*>(m_TexCoord1.data()), sizeof(XMFLOAT2) * num);
        m_MeshOwnerFlag |= MeshOwnerFlag_HasTexCoord1;
    }
    else {
        m_TexCoord1.emplace_back();
    }

    // .jkmh는 무조건 index가 기록되어있다. index가 없을 땐 만들어서 저장하기 때문이다.
    in.read(reinterpret_cast<char*>(&num), sizeof(unsigned int));
    m_vIndices.assign(num, {});
    m_MeshOwnerFlag |= MeshOwnerFlag_HasIndices;
    for (unsigned int i = 0; i < num; ++i) {
        unsigned int vnum{};
        in.read(reinterpret_cast<char*>(&vnum), sizeof(unsigned int));
        m_vIndices[i].assign(vnum, 0);
        in.read(reinterpret_cast<char*>(m_vIndices[i].data()), sizeof(UINT) * vnum);
    }

    BuffersReady();
}

void JStaticMesh::CreateContentFile()
{
    JContent::CreateContentFile();
    std::ofstream out{ m_FullFilePath, std::ios::binary };
    // Primitive Topology
    unsigned int num = ConvertPrimitiveTopologyD3DToInt(m_d3dTopology);
    out.write(reinterpret_cast<const char*>(&num), sizeof(unsigned int));

    // 4: vertex 개수
    if (m_MeshOwnerFlag & MeshOwnerFlag_HasVertex) num = m_Vertices.size();
    else num = 0;
    out.write(reinterpret_cast<const char*>(&num), sizeof(unsigned int));
    // sizeof(XMFLOAT3) * num: vertex
    out.write(reinterpret_cast<const char*>(m_Vertices.data()), sizeof(XMFLOAT3) * num);

    // 4: Color 개수
    if (m_MeshOwnerFlag & MeshOwnerFlag_HasColor) num = m_Colors.size();
    else num = 0;
    out.write(reinterpret_cast<const char*>(&num), sizeof(unsigned int));
    // sizeof(XMFLOAT4) * num: color
    out.write(reinterpret_cast<const char*>(m_Colors.data()), sizeof(XMFLOAT4) * num);

    // 4: Normal 개수
    if (m_MeshOwnerFlag & MeshOwnerFlag_HasNormal) num = m_Normals.size();
    else num = 0;
    out.write(reinterpret_cast<const char*>(&num), sizeof(unsigned int));
    // sizeof(XMFLOAT3) * num: normal
    out.write(reinterpret_cast<const char*>(m_Normals.data()), sizeof(XMFLOAT3) * num);

    // 4: Tangent 개수
    if (m_MeshOwnerFlag & MeshOwnerFlag_HasTangent) num = m_Tangents.size();
    else num = 0;
    out.write(reinterpret_cast<const char*>(&num), sizeof(unsigned int));
    // sizeof(XMFLOAT3) * num: tangent
    out.write(reinterpret_cast<const char*>(m_Tangents.data()), sizeof(XMFLOAT3) * num);

    // 4: BiTangent 개수
    if (m_MeshOwnerFlag & MeshOwnerFlag_HasBiTangent) num = m_BiTangents.size();
    else num = 0;
    out.write(reinterpret_cast<const char*>(&num), sizeof(unsigned int));
    // sizeof(XMFLOAT3) * num: bitangent
    out.write(reinterpret_cast<const char*>(m_BiTangents.data()), sizeof(XMFLOAT3) * num);

    // 4: TexCoord0 개수
    if (m_MeshOwnerFlag & MeshOwnerFlag_HasTexCoord0) num = m_TexCoord0.size();
    else num = 0;
    out.write(reinterpret_cast<const char*>(&num), sizeof(unsigned int));
    // sizeof(XMFLOAT2) * num: texcoord0
    out.write(reinterpret_cast<const char*>(m_TexCoord0.data()), sizeof(XMFLOAT2) * num);

    // 4: TexCoord1 개수
    if (m_MeshOwnerFlag & MeshOwnerFlag_HasTexCoord1) num = m_TexCoord1.size();
    else num = 0;
    out.write(reinterpret_cast<const char*>(&num), sizeof(unsigned int));
    // sizeof(XMFLOAT2) * num: texcoord0
    out.write(reinterpret_cast<const char*>(m_TexCoord1.data()), sizeof(XMFLOAT2) * num);

    // 4: SubMesh 개수 index는 필수로 존재
    num = m_vIndices.size();
    out.write(reinterpret_cast<const char*>(&num), sizeof(unsigned int));
    for (unsigned int i = 0; i < num; ++i) {
        unsigned int vnum = m_vIndices[i].size();
        out.write(reinterpret_cast<const char*>(&vnum), sizeof(unsigned int));
        out.write(reinterpret_cast<const char*>(m_vIndices[i].data()), sizeof(UINT) * vnum);
    }
}



#if DEV_MODE
void JStaticMesh::RenderGUI(ImVec2 startPos, void* imageref)
{
    auto& content = JGlobalContentManager::GetInstance()->GetManager()->GetContentTable();
    void* imagep = content.contains("Resource/JKEngine/Textures/StaticMeshFile.dds") ? content["Resource/JKEngine/Textures/StaticMeshFile.dds"]->GetDedicatedFactor() : nullptr;
    JContent::RenderGUI(startPos, imagep);
}

void JStaticMesh::RenderDetailPanel()
{
    JContent::RenderDetailPanel();

    ImGui::SeparatorText("Mesh Info");
    ImGui::Text("Vertices:    %d", m_Vertices.size());
    ImGui::Text("SubMeshes:  %d", (m_MeshOwnerFlag & MeshOwnerFlag_HasIndices) ? m_vIndices.size() : 1);
    ImGui::Separator();
    if (m_MeshOwnerFlag & MeshOwnerFlag_HasIndices) {
        for (int i = 0; i < m_vIndices.size(); ++i) {
            ImGui::Text("SubMesh%d Triangles: %d", i, m_vIndices[i].size() / 3);
        }
    }
    else {
        ImGui::Text("SubMesh Triangles: %d", m_Vertices.size() / 3);
    }

    const char* tags[] = { "Vertex", "Color", "Normal", "Tangent", "BiTangent", "TexCoord0", "TexCoord1" };
    ImGui::Separator();
    ImGui::Text("Has Data: ");
    for (int i = 0; i < std::size(tags); ++i) {
        if (m_MeshOwnerFlag & (1 << i)) {
            ImGui::Text(tags[i]);
        }
    }
}
#endif 