#include "Drawable/Model.h"
#include "Utils/GraphicsThrowMacros.h"


Model::Model(Graphics& a_Gfx, Vertex* ver, unsigned short* indices)
{
	vBuffer = new VertexBuffer(a_Gfx, ver, 36);
	AddBind(vBuffer);

	vShader = new VertexShader(a_Gfx, L"Assets/VertexShader.hlsl");
	AddBind(vShader);

	pShader = new PixelShader(a_Gfx, L"Assets/PixelShader.hlsl");
	AddBind(pShader);

	IBuffer = new IndexBuffer(a_Gfx, indices, 36);
	AddIndexBuffer(IBuffer);

	const ConstantBufferColorModel cbc = {
	{
		{1.0f, 0.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 1.0f, 1.0f}
	}
	};

	cCB = new PixelConstantBuffer<ConstantBufferColorModel>(a_Gfx, cbc);
	AddBind(cCB);

	const std::vector <D3D11_INPUT_ELEMENT_DESC> ied = {
	{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	m_InputLayout = new InputLayout(a_Gfx, ied, vShader->GetByteCode());
	AddBind(m_InputLayout);

	m_Topology = new Topology(a_Gfx, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(m_Topology);
}

void Model::Update(float a_DeltaTime) noexcept
{
	m_Angle += a_DeltaTime;
}

DirectX::XMMATRIX Model::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranspose(
		DirectX::XMMatrixRotationZ(m_Angle) *
		DirectX::XMMatrixRotationY(m_Angle) *
		DirectX::XMMatrixTranslation(0, 0, 5.0f) *
		DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
	);
}