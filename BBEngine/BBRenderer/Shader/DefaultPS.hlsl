
Texture2D mainTex : register(t0);
SamplerState texSampler : register(s0);

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 texCoords : TEXCOORD;
};

float4 main(VS_OUT psin) : SV_TARGET
{
    return mainTex.Sample(texSampler, psin.texCoords.xy);
}