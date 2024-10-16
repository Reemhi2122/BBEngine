cbuffer CBuf : register(b0)
{
    matrix WVP;
    matrix transform;
};

struct VSOut
{
    float4 pos : SV_Position;
    float3 worldPos : POSITION;
};

VSOut main(float3 pos : Position, float2 tex : TexCoord, float3 normal : Normal)
{
    VSOut output;
    
    matrix MVP = mul(transform, WVP);
    
    output.worldPos = pos;
    output.pos = mul(float4(pos, 1.0f), MVP);
    output.pos.z = output.pos.w;
    return output;
}