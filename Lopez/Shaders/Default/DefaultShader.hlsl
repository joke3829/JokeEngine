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

struct SceneConstant
{
    uint camera;
};

cbuffer cbSceneConstant : register(b1)
{
    SceneConstant g_SceneConstant;
}


void DefaultVS(DefaultVSInput input)
{
    DefaultPSInput output;
    uint nodeindex = g_MeshConstant.nodeIndex;
    uint cameraindex = g_SceneConstant.camera;
    
    output.PosW = mul(float4(input.Position, 1.f), g_WorldMatrices[nodeindex]).xyz;

}