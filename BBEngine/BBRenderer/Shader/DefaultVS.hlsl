
struct VS_IN
{
    float3 pos : POSITION;
    float2 texCoords : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 texCoords : TEXCOORD;
};

cbuffer ConstantBuffer : register(b0)
{
    float4x4 WVPMatrix;
};

VS_OUT main(VS_IN vsin)
{
    VS_OUT vsout;
    vsout.pos = mul(float4(vsin.pos, 1.0f), WVPMatrix);
    vsout.texCoords = vsin.texCoords;
    return vsout;
}