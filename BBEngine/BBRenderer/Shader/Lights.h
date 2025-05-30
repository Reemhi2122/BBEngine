#pragma once
#include "ShaderDefines.h"

#define MAXLIGHTS 50

#ifndef ENGINE
struct DirectionalLight
{
    float3 dir;
    float4 ambient;
    float4 diffuse;
    float4x4 lightView;
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
    float4x4 lightView;
};
#endif

cbuffer cbPerFrame CBRegister0
{
    DirectionalLight directionalLight;
    PointLight pointlights[MAXLIGHTS];
    SpotLight spotlights[MAXLIGHTS];
};

cbuffer ShadowMapCreation CBRegister1
{
    int index;
};