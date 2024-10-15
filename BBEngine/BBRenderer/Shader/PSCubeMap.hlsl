struct VSOut
{
    float4 pos : SV_Position;
};

float4 main(VSOut psin) : SV_Target
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}