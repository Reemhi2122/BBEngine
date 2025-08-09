#include "GameLib/Drawable/SkyBox.h"
#include "Utils/GraphicsThrowMacros.h"
#include "SharedRenderTypes.h"

Skybox::Skybox(IGraphics& a_Gfx)
{
	BBE::Vertex vertices[] = {
		{ {-1.0f, -1.0f, -1.0f}, {0, 0}, {0, 0, 0} },
		{ {1.0f, -1.0f, -1.0f}, {0, 0}, {0, 0, 0} },
		{ {-1.0f, 1.0f, -1.0f}, {0, 0}, {0, 0, 0} },
		{ {1.0f, 1.0f, -1.0f}, {0, 0}, {0, 0, 0} },
		{ {-1.0f, -1.0f, 1.0f}, {0, 0}, {0, 0, 0} },
		{ {1.0f, -1.0f, 1.0f}, {0, 0}, {0, 0, 0} },
		{ {-1.0f, 1.0f, 1.0f}, {0, 0}, {0, 0, 0} },
		{ {1.0f, 1.0f, 1.0f}, {0, 0}, {0, 0, 0} }
	};

	vBuffer = GFXCreateVertexBuffer();
	vBuffer->Create(a_Gfx, vertices, sizeof(BBE::Vertex), 8);

	//vShader = a_Gfx.CreateShader(ShaderType::VertexShader, "Assets/VSCubeMap.hlsl");
	//pShader = a_Gfx.CreateShader(ShaderType::PixelShader, "Assets/PSCubeMap.hlsl");

	uint8_t indices[] = {
		1,2,0, 1,3,2,
		5,3,1, 5,7,3,
		3,6,2, 7,6,3,
		7,5,4, 6,7,4,
		2,4,0, 6,4,2,
		4,1,0, 4,5,1
	};

	IBuffer = GFXCreateIndexBuffer();
	IBuffer->Create(a_Gfx, indices, 36, 1);
	SetIndexBuffer(IBuffer);
	
	const std::vector <D3D11_INPUT_ELEMENT_DESC> ied = {
		{ "Position",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TexCoord",	0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_CubeMap = GFXCreateCubeMap();
	m_CubeMap->Create(a_Gfx);
	AddBind(m_CubeMap);

	//m_Sampler = new Sampler(a_Gfx);

	//m_InputLayout = new InputLayout(a_Gfx, ied, a_Gfx.GetVertexShaderByteCode(vShader).Get());

	//m_Topology = new Topology(a_Gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_TransformBuf = new TransformBuf(a_Gfx, Vector3(0, 0, 0), Vector4(0, 0, 0, 1), Vector3(1, 1, 1));
	AddBind(m_TransformBuf);
}

void Skybox::Update(float a_DeltaTime) noexcept {};

void Skybox::Draw(IGraphics& a_Gfx) noexcept
{
	//a_Gfx.BindShader(ShaderType::VertexShader, vShader);
	//a_Gfx.BindShader(ShaderType::PixelShader, pShader);
	Drawable::Draw(a_Gfx);
}