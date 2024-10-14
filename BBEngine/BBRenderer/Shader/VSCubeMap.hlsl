struct VSOut
{
    float4 pos : SV_Position;
    //float2 tex : TexCoord;
};

VSOut main(float3 inPos : position)
{
    VSOut output = (VSOut) 0;
    
    output.pos = float4(inPos, 1.0);

    return output;
}