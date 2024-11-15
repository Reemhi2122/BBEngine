#include "Lights.h"

struct VSOut
{
    float4 pos : SV_Position;
};

float main(VSOut psin) : SV_Depth
{
    float lightDistance = length(psin.pos.xyz - pointlights[0].position);
    
    return lightDistance / 100;
};