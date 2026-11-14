#include "Common.hlsl"


// b0, b2
// t0, t1, t2~


struct SceneConstant
{
    uint camera;
    float3 cameraPos;
};

cbuffer cbSceneConstant : register(b1)
{
    SceneConstant g_SceneConstant;
}

sampler g_Sampler : register(s0);

struct cLight
{
    float3 color;
    float3 dir;
};

// temp
static const cLight g_Light = { float3(1.f, 0.98, 0.95f) * 0.7f, normalize(float3(-1.f, -1.f, 1.f)) };

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
    output.ViewportIdx = cameraindex;
    
    return output;
}

float4 DefaultPS(DefaultPSInput input) : SV_Target
{
    float4 finalColor;
#ifdef D3D12_TEXTURE_REGISTER
    return float4(1.f, 1.f, 0.f, 1.f);
#endif
    if (g_Material.BaseColorTexIndex != -1)
    {
        finalColor = g_Material.BaseColor * g_Texture[0].Sample(g_Sampler, input.TexCoord0);
    }
    else
    {
        finalColor = g_Material.BaseColor;
    }
    
    if (finalColor.a <= 0.1f)
        discard;
    
    float3 VPos = g_SceneConstant.cameraPos;
    float3 N = normalize(input.Normal);
    float3 L = normalize(g_Light.dir * -1);
    float3 V = normalize(VPos - input.PosW);
    float3 H = normalize(L + V);
    
    float NdotL = saturate(dot(N, L));
    float NdotH = saturate(dot(N, H));
    
    
    float3 realfinalcolor =
    (float3(finalColor.rgb) * g_Light.color.rgb * NdotL) +
    (float3(1.f, 1.f, 1.f) * g_Light.color.rgb * pow(NdotH, 16.f) * (NdotL > 0.f)) +
    (float3(finalColor.rgb) * 0.4f);
    return float4(realfinalcolor, finalColor.a);
}

