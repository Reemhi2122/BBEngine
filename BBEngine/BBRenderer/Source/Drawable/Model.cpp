#include "Drawable/Model.h"
#include "Utils/GraphicsThrowMacros.h"


Model::Model(Graphics& a_Gfx, Vertex* ver, uint32_t vertAmount, unsigned short* indices, uint32_t indicesAmount)
{
	vBuffer = new VertexBuffer(a_Gfx, ver, vertAmount);
	AddBind(vBuffer);

	vShader = new VertexShader(a_Gfx, L"Assets/VertexShader.hlsl");
	AddBind(vShader);

	pShader = new PixelShader(a_Gfx, L"Assets/PixelShader.hlsl");
	AddBind(pShader);

	IBuffer = new IndexBuffer(a_Gfx, indices, indicesAmount);
	AddIndexBuffer(IBuffer);

	const ConstantBufferColorModel cbc = {
	{
		{1.0f, 1.0f, 0.0f, 1.0f}
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
		DirectX::XMMatrixScaling(0.3, 0.3, 0.3) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 5.0f);

}