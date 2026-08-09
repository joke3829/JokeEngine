
#include "JStaticMesh.h"

JStaticMesh::JStaticMesh(const char* name, bool bloadFromFile)
	: JContent(name)
{

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
}

