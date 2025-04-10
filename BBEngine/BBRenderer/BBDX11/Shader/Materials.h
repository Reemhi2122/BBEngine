#pragma once
#include "ShaderDefines.h"

cbuffer MaterialConstant CBRegister2
{
    float4 baseColor;
    int hasTexture;
    float3 padding;
};