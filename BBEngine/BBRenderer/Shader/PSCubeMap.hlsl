struct VSOut
{
    float4 pos : SV_Position;
    //float4 vertexColor;
    //float2 tex : TexCoord;
};

float4 main(VSOut psin) : SV_Target
{
    return float4(0.5, 0.0, 0.0, 1.0);
}