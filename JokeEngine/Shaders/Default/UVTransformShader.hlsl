// ==========================================
// TextureMappingShader.hlsl
// 
// 2DTextureMapping 관련 shadercode 부
// ==========================================
#include "DefaultMaterialShader.hlsl"


// b0, b2
// t0, t1, t2~


cbuffer cbSpriteConstant : register(b3)
{
    float4x4 g_uvMatrix;
}

DefaultPSInput UVTransformVS(DefaultVSInput input)
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
    output.TexCoord0 = mul(float4(input.TexCoord0, 0.f, 1.f), g_uvMatrix).xy;
    output.TexCoord1 = input.TexCoord1;
    
    return output;
}


float4 AlbedoTextureMappingPS(DefaultPSInput input) : SV_Target
{
    float4 finalColor;
#ifdef D3D12_TEXTURE_REGISTER
    return float4(1.f, 1.f, 0.f, 1.f);
#endif

    finalColor = g_Material.Albedo * g_Texture[0].Sample(g_Sampler, input.TexCoord0);

    if (finalColor.a <= 0.1f)
        discard;
    return float4(finalColor);
}

