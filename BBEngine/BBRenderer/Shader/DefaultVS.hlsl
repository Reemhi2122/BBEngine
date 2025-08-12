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
    float4x4 Transform;
};

VS_OUT main(VS_IN vsin)
{
    VS_OUT vsout;
    
    float4x4 pos = mul(Transform, WVPMatrix);
    
    vsout.pos = mul(float4(vsin.pos, 1.0f), pos);
    vsout.texCoords = vsin.texCoords;
    return vsout;
}