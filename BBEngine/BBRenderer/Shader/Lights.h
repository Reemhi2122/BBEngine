#pragma once
#include "ShaderDefines.h"

#define MAXLIGHTS 50

#ifndef ENGINE
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
#endif

cbuffer cbPerFrame{
    DirectionalLight directionalLight;
    PointLight pointlights[MAXLIGHTS];
    SpotLight spotlights[MAXLIGHTS];
};
