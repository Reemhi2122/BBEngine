#include "Vertexconstant.h"

cbuffer CBuf : register(b0)
{
    matrix WVP;
    matrix transform;
};

struct VSOut
{
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float2 tex : TexCoord, float3 normal : Normal, matrix instanceTransform : InstanceTransform)
{
    matrix finalTransfom = mul(transform, instanceTransform);
    matrix MVP = mul(finalTransfom, WVP);

    VSOut vso;
    vso.pos = mul(float4(pos, 1.0), MVP);
    return vso;
}