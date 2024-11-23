#include "Lights.h"

struct VSOut
{
    float4 pos : SV_Position;
    float3 worldPos : WorldPosition;
};

float main(VSOut psin) : SV_Depth
{
    float3 lightToPixelVec = psin.worldPos - pointlights[index].position;
    float lightDistance = length(lightToPixelVec);

    return lightDistance / 100;
};