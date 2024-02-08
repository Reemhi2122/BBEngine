Texture2D tex;
SamplerState splr;

struct VSOut
{
    float4 pos : SV_Position;
    float2 tex : TexCoord;
    float3 normal : Normal;
};


float4 main(VSOut psin) : SV_Target
{
    psin.normal = normalize(psin.normal);

    return tex.Sample(splr, psin.tex);
}