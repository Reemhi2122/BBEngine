
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
    float4 colorMultiplier;
};

VS_OUT main(VS_IN vsin)
{
    VS_OUT vsout;
    vsout.pos = float4(vsin.pos, 1.0f);
    vsout.color = vsin.color * colorMultiplier;
    return vsout;
}