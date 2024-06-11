#include "Vertexconstant.h"

cbuffer CBuf
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
    float4 FragPosLightSpace : FragPos;
};

VSOut main(float3 pos : Position, float2 tex : TexCoord, float3 normal : Normal, matrix instanceTransform : InstanceTransform)
{
    VSOut vso;
    
    matrix finalTransfom = mul(transform, instanceTransform);
    matrix MVP = mul(finalTransfom, WVP);
    
    vso.pos = mul(float4(pos, 1.0f), MVP);
    vso.worldPos = mul(float4(pos, 1.0f), finalTransfom);
    vso.tex = tex;
    vso.normal = mul(normal, finalTransfom);
    vso.FragPosLightSpace = float4(1.f, 1.f, 1.f, 1.f);
    return vso;
}