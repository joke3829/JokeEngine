
struct MeshConstant
{
    uint nodeIndex;
    uint bSkinning;
    uint2 padding;
    uint4 BoneLookupTable[64];      // uint[256] = uint4[64]
};


// IA ===========================================================

struct DefaultVSInput
{
    float3 Position : POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 BiTangent : BITANGENT;
    float2 TexCoord0 : TEXCOORD0;
    float2 TexCoord1 : TEXCOORD1;
};

struct DefaultPSInput
{
    float4 Position : SV_Position;
    float3 PosW : POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 BiTangent : BITANGENT;
    float2 TexCoord0 : TEXCOORD0;
    float2 TexCoord1 : TEXCOORD1;
};

// ===============================================================

struct DefaultMaterial
{
    float4 Albedo;
    float4 Specular;
    float4 Ambient;
    float4 Emissive;
    int AlbedoTexIndex; // dx11: 0 or 1(true false)   dx12: -1 ~ g_TextureIndex
};



// register ==========================================================

cbuffer cbConstantBuffer : register(b0)
{
    MeshConstant g_MeshConstant;
}

cbuffer cbDefaultMaterial : register(b2)
{
    DefaultMaterial g_Material;
}

StructuredBuffer<float4x4> g_WorldMatrices : register(t0);
StructuredBuffer<float4x4> g_Cameras : register(t1);


// Texture 

#ifdef D3D12_TEXTURE_REGISTER
Texture2D g_Texture[] : register(t2, space1);
#else
Texture2D g_Texture[20] : register(t2); // 최대 20개의 텍스쳐 t2 ~ t21
#endif



// b0, b2
// t0, t1, t2~
