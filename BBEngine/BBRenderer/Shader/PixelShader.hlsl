Texture2D tex;
SamplerState splr;

struct DirectionalLight
{
    float3 dir;
    float4 ambient;
    float4 diffuse;
};

struct PointLight
{
    float3  position;
    float3  attenuation;
    float   range;
    float4  ambient;
    float4  diffuse;
};

struct SpotLight
{
    float3  position;
    float3  direction;
    float   cone;
    float3  attenuation;
    float   range;
    float4  ambient;
    float4  diffuse;
};

cbuffer cbPerFrame {
    DirectionalLight directionalLight[50];
    PointLight pointlight[50];
    SpotLight spotlight[50];
};

struct VSOut
{
    float4 pos : SV_Position;
    float4 worldPos : POSITION;
    float2 tex : TexCoord;
    float3 normal : Normal;
};

float3 CalculateDirectionalLight(VSOut psin, float4 diffuse) {
    
    float3 finalColor;

    finalColor = diffuse * directionalLight.ambient;

    finalColor += saturate(dot(directionalLight.dir, psin.normal) * directionalLight.diffuse * diffuse);

    return finalColor;
}

float3 CalculatePointLight(VSOut psin, float4 dirlightcolor) {

    float3 finalColor = float3(0.0f, 0.0f, 0.0f);

    float3 lightToPixelVec = pointlight.position - psin.worldPos;

    float d = length(lightToPixelVec);
  
    float3 finalAmbient = dirlightcolor * pointlight.ambient;

    if (d > pointlight.range)
        return finalAmbient;

    lightToPixelVec /= d;

    float howMuchLight = dot(lightToPixelVec, psin.normal);

    if (howMuchLight > 0.0f)
    {
        finalColor += howMuchLight * dirlightcolor * pointlight.diffuse;
        finalColor /= pointlight.attenuation[0] + (pointlight.attenuation[1] * d) + (pointlight.attenuation[2] * (d * d));
    }

    finalColor = saturate(finalColor + finalAmbient);

    return finalColor;
}

float3 CalculateSpotLight(VSOut psin, float4 dirlightcolor) {

    float3 finalColor = float3(0.0f, 0.0f, 0.0f);

    float3 lightToPixelVec = spotlight.position - psin.worldPos;

    float d = length(lightToPixelVec);
  
    float3 finalAmbient = dirlightcolor * spotlight.ambient;

    if (d > spotlight.range)
        return finalAmbient;

    lightToPixelVec /= d;

    float howMuchLight = dot(lightToPixelVec, psin.normal);

    if (howMuchLight > 0.0f)
    {
        finalColor += dirlightcolor * spotlight.diffuse;
        finalColor /= (spotlight.attenuation[0] + (spotlight.attenuation[1] * d)) + (spotlight.attenuation[2] * (d * d));
        finalColor *= pow(max(dot(-lightToPixelVec, spotlight.direction), 0.0f), spotlight.cone);
    }

    finalColor = saturate(finalColor + finalAmbient);

    return finalColor;
}

float4 main(VSOut psin) : SV_Target
{
    psin.normal = normalize(psin.normal);

    float4 diffuse = tex.Sample(splr, psin.tex);
    
    float4 finalColor = float4(0, 0, 0, 1);
        
    finalColor += float4(CalculateDirectionalLight(psin, diffuse), diffuse.a);
    finalColor += float4(CalculatePointLight(psin, diffuse), diffuse.a);
    finalColor += float4(CalculateSpotLight(psin, diffuse), diffuse.a);

    return finalColor;
};