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

VSOut main(float3 pos : Position, float2 tex : TexCoord, float3 normal : Normal, matrix instanceTransform : InstanceTransform)
{
    VSOut vso;
    vso.pos = mul(float4(pos, 1.0f), transform);
    vso.worldPos = mul(mul(float4(pos, 1.0f), world), instanceTransform);
    vso.tex = tex;
    vso.normal = mul(normal, world);
    return vso;
}