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

VSOut main(float3 pos : Position)
{
    VSOut output;
    
    matrix MVP = mul(transform, WVP);
    output.pos = mul(float4(pos, 1.0f), MVP);

    output.worldPos = pos;
    // output.pos = mul(float4(pos, 0.0f), WVP);
    // output.pos.z = output.pos.w;
    return output;
}
