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
    float4 Albedo;
    float4 Specular;
    float4 Ambient;
    float4 Emissive;
};

cbuffer cbDefaultMaterial : register(b2)
{
    DefaultMaterial g_Material;
}

// ================================

struct SceneConstant
{
    uint camera;
};

cbuffer cbSceneConstant : register(b1)
{
    SceneConstant g_SceneConstant;
}


DefaultPSInput DefaultVS(DefaultVSInput input)
{
    DefaultPSInput output;
    uint nodeindex = g_MeshConstant.nodeIndex;
    uint cameraindex = g_SceneConstant.camera;
    
    output.PosW = mul(float4(input.Position, 1.f), g_WorldMatrices[nodeindex]).xyz;
    output.Position = mul(float4(output.PosW, 1.f), g_Cameras[cameraindex]);
    output.Color = input.Color;
    output.Normal = mul(input.Normal, (float3x3) g_WorldMatrices[nodeindex]);
    output.Tangent = mul(input.Tangent, (float3x3) g_WorldMatrices[nodeindex]);
    output.BiTangent = mul(input.BiTangent, (float3x3) g_WorldMatrices[nodeindex]);
    output.TexCoord0 = input.TexCoord0;
    output.TexCoord1 = input.TexCoord1;
    
    return output;
}

float4 DefaultPS(DefaultPSInput input) : SV_Target{
    return g_Material.Albedo;
}

