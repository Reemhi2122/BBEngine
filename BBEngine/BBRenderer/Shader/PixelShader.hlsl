Texture2D tex;
SamplerState splr;

struct VSOut
{
    float4 pos : SV_Position;
    float2 tex : TexCoord;
};


float4 main(VSOut psin) : SV_Target
{
    return tex.Sample(splr, psin.tex);
}