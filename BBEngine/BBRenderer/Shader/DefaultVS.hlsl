
struct VS_IN
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

cbuffer ConstantBuffer : register(b0)
{
    float4x4 WVPMatrix;
};

VS_OUT main(VS_IN vsin)
{
    VS_OUT vsout;
    vsout.pos = mul(float4(vsin.pos, 1.0f), WVPMatrix);
    vsout.color = vsin.color;
    return vsout;
}