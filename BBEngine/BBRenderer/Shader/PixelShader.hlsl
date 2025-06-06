#include "Lights.h"
#include "Materials.h"

Texture2D tex                       : register(ps, t0);
Texture2D depthBuffer               : register(ps, t1);

TextureCubeArray depthCubeMap       : register(ps, t2);
Texture2DArray SLDepthArray         : register(ps, t3);
Texture2D depthMapDL                : register(ps, t4);

SamplerState splr                   : register(ps, s0);
SamplerState depthSampler           : register(ps, s1);

Texture2D khrThicknessTexture       : register(ps, t5);
Texture2D khrTransmissionTexture    : register(ps, t6);

struct VSOut
{
    float4 pos : SV_Position;
    float4 worldPos : POSITION;
    float2 tex : TexCoord;
    float3 normal : Normal;
};

float ShadowCalculationPointLight(float4 fragPos, PointLight pointLight, int lightIndex)
{
    float3 fragToLight = (fragPos.xyz - pointLight.position);
    
    float closestDepth = depthCubeMap.Sample(depthSampler, float4(fragToLight, lightIndex)).r;
    
    closestDepth *= 100;
    
    float currentDepth = length(fragToLight);
    
    float bias = 0.5f;
    
    return (currentDepth - bias) > closestDepth ? 1.0 : 0.0;
}

float3 CalculatePointLight(VSOut psin, float4 dirlightcolor, PointLight pointlight, int lightIndex) {

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

    float shadow = ShadowCalculationPointLight(psin.worldPos, pointlight, lightIndex);
    finalColor = finalColor * (1.0 - shadow);
    return finalColor;
}

float ShadowCalculationSpotLight(float4 fragPosLigthSpace, int index)
{
    float3 projCoords = fragPosLigthSpace.xyz / fragPosLigthSpace.w;
    
    float closestDepth = SLDepthArray.Sample(depthSampler, float3(projCoords.xy, index)).r; //We need to add index here
    
    float currentDepth = projCoords.z;
    
    float bias = 0.005f;
    
    return (currentDepth - bias) > closestDepth ? 1.0 : 0.0;
}

float3 CalculateSpotLight(VSOut psin, float4 dirlightcolor, SpotLight spotlight, float4 fragPosLigthSpace, int lightIndex)
{
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

    float shadow = ShadowCalculationSpotLight(fragPosLigthSpace, lightIndex);
    finalColor = finalColor * (1.0 - shadow);
    return finalColor;
}

float ShadowCalculationDirectionalLight(float4 fragPosLigthSpace)
{
    float3 projCoords = fragPosLigthSpace.xyz / fragPosLigthSpace.w;
    
    float closestDepth = depthMapDL.Sample(depthSampler, float2(projCoords.xy)).r;
    
    float currentDepth = projCoords.z;
    
    float bias = 0.00001f;
    
    return (currentDepth - bias) > closestDepth ? 1.0 : 0.0;
}

float3 CalculateDirectionalLight(VSOut psin, float4 diffuse, DirectionalLight dirlight, float4 fragPosLigthSpace)
{
    float3 finalColor = float3(0.0f, 0.0f, 0.0f);

    finalColor += saturate(dot(directionalLight.dir, psin.normal) * directionalLight.diffuse * diffuse);
    
    float shadow = ShadowCalculationDirectionalLight(fragPosLigthSpace);
    finalColor = finalColor * (1.0 - shadow);

    finalColor = saturate(finalColor + (diffuse * directionalLight.ambient));

    return finalColor;
}

float4 main(VSOut psin) : SV_Target
{
    psin.normal = normalize(psin.normal);

    float4 diffuse = baseColor;
    if (hasBaseColorTexture)
    {
        diffuse = tex.Sample(splr, psin.tex);
        diffuse *= baseColor;
    }
    
    //if (hasKhrVolumeTexture)
    //{
    //    diffuse *= khrThicknessTexture.Sample(splr, psin.tex);
    //}
    
    if (diffuse.a < 0.01f)
        discard;
    
    float4 finalColor = float4(0, 0, 0, 1);
    
    float4 curLightView;
    float4 curFragLightPos;
    
    curLightView = mul(psin.worldPos, directionalLight.lightView);
    curFragLightPos = curLightView * float4(0.5f, -0.5f, 1.0f, 1.0f) + (float4(0.5f, 0.5f, 0.0f, 0.0f) * curLightView.w);
    finalColor += float4(CalculateDirectionalLight(psin, diffuse, directionalLight, curFragLightPos), diffuse.a);

    for (int i = 0; i < MAXLIGHTS; i++)
    {
        finalColor += float4(CalculatePointLight(psin, diffuse, pointlights[i], i), diffuse.a);
    }

    for (int i = 0; i < MAXLIGHTS; i++)
    {
        curLightView = mul(psin.worldPos, spotlights[i].lightView);
        curFragLightPos = curLightView * float4(0.5f, -0.5f, 1.0f, 1.0f) + (float4(0.5f, 0.5f, 0.0f, 0.0f) * curLightView.w);
        finalColor += float4(CalculateSpotLight(psin, diffuse, spotlights[i], curFragLightPos, i), diffuse.a);
    }
    
    return finalColor;
};