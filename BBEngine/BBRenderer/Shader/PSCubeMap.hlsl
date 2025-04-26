TextureCube tex : register(t0);
SamplerState sam : register(s0);

struct VSOut
{
    float4 pos : SV_Position;
    float3 worldPos : POSITION;
};

float4 main(VSOut psin) : SV_Target
{
    // return tex.Sample(sam, psin.worldPos);
    return float4(1, 1, 1, 1);
}