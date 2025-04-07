#include "Drawable/Drawable.h"
#include "Drawable/Quad.h"
#include "Utils/GraphicsThrowMacros.h"

Quad::Quad(Graphics& a_Gfx)
{
	BBE::Vertex vertices[] = {
		{ {-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}, {0, 1, 0} },
		{ {-1.0f, +1.0f, 0.0f}, {1.0f, 0.0f}, {0, 1, 0} },
		{ {+1.0f, +1.0f, 0.0f}, {0.0f, 0.0f}, {0, 1, 0} },
		{ {+1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}, {0, 1, 0} },
	};

	vBuffer = new VertexBuffer(a_Gfx, vertices, 4);
	AddBind(vBuffer);

	uint8_t indices[] = {
		1,2,0, 1,3,2,
	};

	vShader = a_Gfx.CreateShader(ShaderType::VertexShader, "Assets/VertexShader.hlsl");
	pShader = a_Gfx.CreateShader(ShaderType::PixelShader, "Assets/PixelShader.hlsl");

	IBuffer = new IndexBuffer(a_Gfx, indices, 6, 1);
		
	const std::vector <D3D11_INPUT_ELEMENT_DESC> ied = {
		{ "Position",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TexCoord",	0, DXGI_FORMAT_R32G32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	m_Texture = new Texture(a_Gfx, "Assets/Textures/grass.png");
	AddBind(m_Texture);

	m_Sampler = new Sampler(a_Gfx);
	AddBind(m_Sampler);

	m_InputLayout = new InputLayout(a_Gfx, ied, a_Gfx.GetVertexShaderByteCode(vShader).Get());
	AddBind(m_InputLayout);

	m_Topology = new Topology(a_Gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(m_Topology);

	m_TransformBuf = new TransformBuf(a_Gfx, Vector3(-4, 1, 1.3f), Vector4(0, 0, 0, 1), Vector3(1, 1, 1));
	AddBind(m_TransformBuf);
}

void Quad::Update(float a_DeltaTime) noexcept {};

void Quad::Draw(Graphics& a_Gfx) noexcept
{
	a_Gfx.BindShader(ShaderType::VertexShader, vShader);
	a_Gfx.BindShader(ShaderType::PixelShader, pShader);

	SetIndexBuffer(IBuffer);
	Drawable::Draw(a_Gfx);
}