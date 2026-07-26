
struct MeshConstant
{
    uint nodeIndex;
    uint bSkinning;
    uint2 padding;
    uint4 BoneLookupTable[64];      // uint[256] = uint4[64]
};

cbuffer cbConstantBuffer : register(b0)
{
    MeshConstant g_MeshConstant;
}

StructuredBuffer<float4x4> g_WorldMatrices : register(t0);
StructuredBuffer<float4x4> g_Cameras : register(t1);


// Texture 

#ifdef D3D12_TEXTURE_REGISTER
Texture2D g_Texture[] : register(t2, space1);
#else
Texture2D g_Texture[20] : register(t2);     // 최대 20개의 텍스쳐 t2 ~ t21
#endif