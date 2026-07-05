#include "Common.hlsl"


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


// ================================

struct DefaultMaterial
{
    
};

// ================================


struct MeshConstant
{
    
};


cbuffer cbMeshConstant : register(b0)
{
    MeshConstant l_MeshConstant;
}


void DefaultVS(DefaultVSInput input)
{
    DefaultPSInput output;
    output.PosW = mul(m)

}