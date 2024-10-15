cbuffer CBuf : register(b0)
{
    matrix WVP;
    matrix transform;
};

struct VSOut
{
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float2 tex : TexCoord, float3 normal : Normal)
{
    VSOut output;
    
    matrix MVP = mul(transform, WVP);
    
    output.pos = mul(float4(pos, 1.0f), MVP);
    return output;
}