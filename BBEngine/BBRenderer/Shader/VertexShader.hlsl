cbuffer CBuf {
	matrix transform;
    matrix world;
};

struct VSOut
{
    float4 pos : SV_Position;
    float4 worldPos : POSITION;
    float2 tex : TexCoord;
    float3 normal : Normal;
};

VSOut main(float3 pos : Position, float2 tex : TexCoord, float3 normal : Normal)
{
    VSOut vso;
    vso.pos = mul(float4(pos, 1.0f), transform);
    vso.worldPos = mul(world, float4(pos, 1.0f));
    vso.tex = tex;
    vso.normal = mul(normal, world);
    return vso;
}