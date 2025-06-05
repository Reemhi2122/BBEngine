struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

float4 main(VS_OUT psin) : SV_TARGET
{
    return psin.color;
}