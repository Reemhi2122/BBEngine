#include "Lights.h"

Texture2D tex           : register(ps, t0);
Texture2D depthBuffer   : register(ps, t1);

SamplerState splr;

struct VSOut
{
    float4 pos : SV_Position;
    float4 worldPos : POSITION;
    float2 tex : TexCoord;
    float3 normal : Normal;
    float4 FragPosLightSpace : FragPos;
};

float3 CalculateDirectionalLight(VSOut psin, float4 diffuse, DirectionalLight directionalLight) {
    
    float3 finalColor;

    finalColor = diffuse * directionalLight.ambient;

    finalColor += saturate(dot(directionalLight.dir, psin.normal) * directionalLight.diffuse * diffuse);

    return finalColor;
}

float3 CalculatePointLight(VSOut psin, float4 dirlightcolor, PointLight pointlight) {

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

float3 CalculateSpotLight(VSOut psin, float4 dirlightcolor, SpotLight spotlight) {

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

float ShadowCalculation(float4 fragPosLigthSpace)
{
    float3 projCoords = fragPosLigthSpace.xyz / fragPosLigthSpace.w;
    
    projCoords = projCoords * 0.5 + 0.5;
    
    float closestDepth = depthBuffer.Sample(splr, projCoords.xy).r;
    
    float currentDepth = projCoords.z;
    
    if (currentDepth > closestDepth)
    {
        return 1.0;
    }
    
    return 0.0;
}

float4 main(VSOut psin) : SV_Target
{
    psin.normal = normalize(psin.normal);

    float4 diffuse = tex.Sample(splr, psin.tex);
    
    float4 finalColor = float4(0, 0, 0, 1);
    
    finalColor += float4(CalculateDirectionalLight(psin, diffuse, directionalLight), diffuse.a);       

    for(int i = 0; i < MAXLIGHTS; i++) {
        finalColor += float4(CalculatePointLight(psin, diffuse, pointlights[i]), diffuse.a);       
    }

    for(int i = 0; i < MAXLIGHTS; i++) {
        finalColor += float4(CalculateSpotLight(psin, diffuse, spotlights[i]), diffuse.a);       
    }

    float shadow = ShadowCalculation(psin.FragPosLightSpace);
    
    finalColor = finalColor * shadow;
    
    return finalColor;
};