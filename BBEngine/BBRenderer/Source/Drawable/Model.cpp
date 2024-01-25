#include "Drawable/Model.h"
#include "Utils/GraphicsThrowMacros.h"

//Note(Stan):	Should probably change this later to
//				to use the full GLTF file.
Model::Model(Graphics& a_Gfx, BBE::Node a_ModelFile)
{
	BBE::Vertex* vertices = reinterpret_cast<BBE::Vertex*>(malloc(a_ModelFile.mesh.vertAmount * sizeof(BBE::Vertex)));

	for (size_t i = 0; i < a_ModelFile.mesh.vertAmount; i++)
	{
		vertices[i].pos	=		a_ModelFile.mesh.vertices[i];
		vertices[i].texCoords = {0, 0};
		vertices[i].normals =	{0, 0, 0};
	}

	vBuffer = new VertexBuffer(a_Gfx, vertices, a_ModelFile.mesh.vertAmount);
	AddBind(vBuffer);

	vShader = new VertexShader(a_Gfx, L"Assets/VertexShader.hlsl");
	AddBind(vShader);

	pShader = new PixelShader(a_Gfx, L"Assets/PixelShader.hlsl");
	AddBind(pShader);

	IBuffer = new IndexBuffer(a_Gfx, a_ModelFile.mesh.indices, a_ModelFile.mesh.indicesAmount);
	AddIndexBuffer(IBuffer);

	const std::vector <D3D11_INPUT_ELEMENT_DESC> ied = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	m_InputLayout = new InputLayout(a_Gfx, ied, vShader->GetByteCode());
	AddBind(m_InputLayout);

	m_Topology = new Topology(a_Gfx, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(m_Topology);

	m_TransformBuf = new TransformBuf(a_Gfx, *this);
	AddBind(m_TransformBuf);

	//Testing
	m_Translation = a_ModelFile.translation;
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
		DirectX::XMMatrixScaling(1, 1, 1) *
		DirectX::XMMatrixTranslation(m_Translation.x, m_Translation.y -15, 30);
}


//const ConstantBufferColorModel cbc = {
//{
//	{1.0f, 1.0f, 0.0f, 1.0f}
//}
//};

//cCB = new PixelConstantBuffer<ConstantBufferColorModel>(a_Gfx, cbc);
//AddBind(cCB);
