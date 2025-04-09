#pragma once
#include "ShaderDefines.h"

cbuffer MaterialConstant CBRegister3
{
    float4 baseColor;
    bool hasTexture;
    float3 pad;
};