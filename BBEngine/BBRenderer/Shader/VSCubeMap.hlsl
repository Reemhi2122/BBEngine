cbuffer CBuf : register(b0)
{
    matrix ViewProjectionMatrix;
    matrix WorldMatrix;
};

struct VSOut
{
    float4 pos : SV_Position;
    float3 worldPos : POSITION;
};

VSOut main(float3 pos : POSITION)
{
    VSOut output;
    
    matrix WVP = mul(WorldMatrix, ViewProjectionMatrix);
    
    //Note(Stan): Used to debug
    //output.worldPos = pos;
    //output.pos = mul(float4(pos, 1.0f), WVP);
    
    output.worldPos = pos;
    output.pos = mul(float4(pos, 0.0f), WVP);
    output.pos.z = output.pos.w;
    return output;
}
