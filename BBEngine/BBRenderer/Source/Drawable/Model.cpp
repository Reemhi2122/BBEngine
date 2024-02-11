#include "Drawable/Model.h"
#include "Utils/GraphicsThrowMacros.h"

Model::Model(Graphics& a_Gfx, BBE::Mesh::Primative a_ModelFile, BBE::GLTFFile* a_File, VertexShader* a_VertexShader, PixelShader* a_PixelShader)
{
	BBE::Vertex* vertices = reinterpret_cast<BBE::Vertex*>(malloc(a_ModelFile.vertexCount * sizeof(BBE::Vertex)));

	if (vertices == nullptr)
		return;

	for (size_t i = 0; i < a_ModelFile.vertexCount; i++)
	{
		vertices[i].pos	=		a_ModelFile.vertices[i];
		vertices[i].texCoords = a_ModelFile.texCoords[i];
		vertices[i].normals =	a_ModelFile.normals[i];
	}

	char texturePath[64] = "";
	strcat(texturePath, a_File->gltfPath);
	strcat(texturePath, a_ModelFile.Material.pbrMetallicRoughness.baseColorTexture.image.m_Path);
	
	m_Texture = new Texture(a_Gfx, texturePath);
	AddBind(m_Texture);

	vBuffer = new VertexBuffer(a_Gfx, vertices, a_ModelFile.vertexCount);
	AddBind(vBuffer);

	m_Sampler = new Sampler(a_Gfx);
	AddBind(m_Sampler);

	AddBind(a_VertexShader);

	AddBind(a_PixelShader);

	IBuffer = new IndexBuffer(a_Gfx, a_ModelFile.indices, a_ModelFile.indicesAmount);
	AddIndexBuffer(IBuffer);

	const std::vector <D3D11_INPUT_ELEMENT_DESC> ied = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	m_InputLayout = new InputLayout(a_Gfx, ied, a_VertexShader->GetByteCode());
	AddBind(m_InputLayout);

	m_Topology = new Topology(a_Gfx, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(m_Topology);

	m_TransformBuf = new TransformBuf(a_Gfx, *this);
	AddBind(m_TransformBuf);
}

void Model::Update(float a_DeltaTime) noexcept
{
	m_Angle += a_DeltaTime;
}

DirectX::XMMATRIX Model::GetTransformXM() const noexcept
{
	return
		//DirectX::XMMatrixRotationY(m_Angle) *
		//DirectX::XMMatrixRotationZ(m_Angle) *
		DirectX::XMMatrixScaling(0.00800000037997961f, 0.00800000037997961f, 0.00800000037997961f) *
		DirectX::XMMatrixTranslation(m_Translation.x, m_Translation.y -15, 30);
}