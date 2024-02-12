
struct DirectionalLight
{
    float3 dir;
    float4 ambient;
    float4 diffuse;
};

struct SpotLight
{
    float3 position;
    float3 attenuation;
    float4 ambient;
    float4 diffuse;
    float range;
};

cbuffer cbPerFrame {
    DirectionalLight directionalLight;
    SpotLight spotLight;
};

Texture2D tex;
SamplerState splr;

struct VSOut
{
    float4 pos : SV_Position;
    float4 worldPos : POSITION;
    float2 tex : TexCoord;
    float3 normal : Normal;
};

float4 CalculateDirectionalLight(VSOut psin, float4 diffuse) {
    
    float3 finalColor;

    finalColor = diffuse * directionalLight.ambient;

    finalColor += saturate(dot(directionalLight.dir, psin.normal) * directionalLight.diffuse * diffuse);

    return float4(finalColor, diffuse.a);
}

float4 CalculateSpotLight(VSOut psin, float4 dirlightcolor) {
    float3 finalColor = float3(0.0f, 0.0f, 0.0f);

    float3 lightToPixelVec = spotLight.position - psin.worldPos;

    float d = length(lightToPixelVec);
  
    float3 finalAmbient = dirlightcolor * spotLight.ambient;

    if (d > spotLight.range)
        return float4(finalAmbient, dirlightcolor.a);

    lightToPixelVec /= d;

    float howMuchLight = dot(lightToPixelVec, psin.normal);

    if (howMuchLight > 0.0f)
    {
        finalColor += howMuchLight * dirlightcolor * spotLight.diffuse;
        finalColor /= spotLight.attenuation[0] + (spotLight.attenuation[1] * d) + (spotLight.attenuation[2] * (d * d));
    }

    finalColor = saturate(finalColor + finalAmbient);

    return float4(finalColor, dirlightcolor.a);
}

float4 main(VSOut psin) : SV_Target
{
    psin.normal = normalize(psin.normal);

    float4 diffuse = tex.Sample(splr, psin.tex);

    //float4 dirlightcolor = CalculateDirectionalLight(psin, diffuse);
    
    float4 spotlightcolor = CalculateSpotLight(psin, diffuse);

    return spotlightcolor;
};