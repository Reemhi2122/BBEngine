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

VSOut main(float3 pos : Position, matrix instanceTransform : InstanceTransform)
{
    matrix finalTransfom = mul(transform, instanceTransform);

    VSOut vso;
    vso.pos = mul(mul(float4(pos, 1.0), finalTransfom), lightMatrix);
    return vso;
}