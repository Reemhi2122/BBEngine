cbuffer CBuf {
	matrix transform;
};

struct VSOut
{
    float4 pos : SV_Position;
    float2 tex : TexCoord;
};

VSOut main(float3 pos : Position, float2 tex : TexCoord)
{
    VSOut vso;
    vso.pos = mul(float4(pos, 1.0f), transform);
    vso.tex = tex;
    return vso;
}