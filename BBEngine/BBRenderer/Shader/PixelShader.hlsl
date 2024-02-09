Texture2D tex;
SamplerState splr;

struct Light
{
    float3 dir;
    float4 ambient;
    float4 diffuse;
};

cbuffer cbPerFrame {
    Light light;
};

struct VSOut
{
    float4 pos : SV_Position;
    float2 tex : TexCoord;
    float3 normal : Normal;
};

float4 main(VSOut psin) : SV_Target
{
    psin.normal = normalize(psin.normal);

    float4 diffuse = tex.Sample(splr, psin.tex);

    float3 finalColor;

    finalColor = diffuse * light.ambient;
    finalColor += saturate(dot(light.dir, input.normal) * light.diffuse * diffuse);

    return float4(finalColor, diffuse.a);
}