#include "Vertexconstant.h"

cbuffer CBuf : register(b0)
{
    matrix WVP;
    matrix transform;
};

struct VSOut
{
    float4 pos : SV_Position;
    float4 worldPos : POSITION;
    float2 tex : TexCoord;
    float3 normal : Normal;
    //float4 FragPosLightSpace : FragPos;
};

VSOut main(float3 pos : Position, float2 tex : TexCoord, float3 normal : Normal)
{
    VSOut vso;
    
    matrix MVP = mul(transform, WVP);
    
    vso.pos = mul(float4(pos, 1.0f), MVP);
    vso.worldPos = mul(float4(pos, 1.0f), transform);
    vso.tex = tex;
    vso.normal = mul(normal, transform);
    //const float4 test = mul(vso.worldPos, lightMatrix[0]);
    //vso.FragPosLightSpace = test * float4(0.5f, -0.5f, 1.0f, 1.0f) + (float4(0.5f, 0.5f, 0.0f, 0.0f) * test.w);
    return vso;
}

//VSOut main(float3 pos : Position, float2 tex : TexCoord, float3 normal : Normal, matrix instanceTransform : InstanceTransform)
//{
//    VSOut vso;
//    
//    matrix finalTransfom = mul(transform, instanceTransform);
//    matrix MVP = mul(finalTransfom, WVP);
//    
//    vso.pos = mul(float4(pos, 1.0f), MVP);
//    vso.worldPos = mul(float4(pos, 1.0f), finalTransfom);
//    vso.tex = tex;
//    vso.normal = mul(normal, finalTransfom);
//    const float4 test = mul(vso.worldPos, lightMatrix);
//    vso.FragPosLightSpace = test * float4(0.5f, -0.5f, 1.0f, 1.0f) + (float4(0.5f, 0.5f, 0.0f, 0.0f) * test.w);
//    return vso;
//}