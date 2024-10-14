#include "Drawable/Drawable.h"
#include "Drawable/Box.h"
#include "Utils/GraphicsThrowMacros.h"

Box::Box(Graphics& a_Gfx)
{
	if (!IsStaticInitialized()) {
		BBE::Vertex vertices[] = {
			{ {-1.0f, -1.0f, -1.0f}, {0, 0} },
			{ {1.0f, -1.0f, -1.0f}, {0, 0} },
			{ {-1.0f, 1.0f, -1.0f}, {0, 0} },
			{ {1.0f, 1.0f, -1.0f}, {0, 0} },
			{ {-1.0f, -1.0f, 1.0f}, {0, 0} },
			{ {1.0f, -1.0f, 1.0f}, {0, 0} },
			{ {-1.0f, 1.0f, 1.0f}, {0, 0} },
			{ {1.0f, 1.0f, 1.0f}, {0, 0} }
		};

		vBuffer = new VertexBuffer(a_Gfx, vertices, 8);
		AddStaticBind(vBuffer);

		vShader = a_Gfx.CreateShader(ShaderType::VertexShader, L"Assets/VSCubeMap.hlsl");
		pShader = a_Gfx.CreateShader(ShaderType::PixelShader, L"Assets/PSCubeMap.hlsl");

		uint8_t indices[] = {
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4
		};

		IBuffer = new IndexBuffer(a_Gfx, indices, 36, 4);
		AddStaticBindIndexBuffer(IBuffer);

		const ConstantBufferColor cbc = {
			{
				{1.0f, 0.0f, 1.0f, 1.0f},
				{1.0f, 0.0f, 0.0f, 1.0f},
				{0.0f, 1.0f, 0.0f, 1.0f},
				{0.0f, 0.0f, 1.0f, 1.0f},
				{1.0f, 1.0f, 0.0f, 1.0f},
				{0.0f, 1.0f, 1.0f, 1.0f}
			}
		};

		cCB = new PixelConstantBuffer<ConstantBufferColor>(a_Gfx, cbc);
		AddStaticBind(cCB);

		const std::vector <D3D11_INPUT_ELEMENT_DESC> ied = {
			{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		m_InputLayout = new InputLayout(a_Gfx, ied, a_Gfx.GetVertexShaderByteCode(vShader).Get());
		AddStaticBind(m_InputLayout);

		m_Topology = new Topology(a_Gfx, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		AddStaticBind(m_Topology);

	}
	else {
		AddIndexFromStatic();
	}

	m_TransformBuf = new TransformBuf(a_Gfx, Vector3(0, 0, 0), Vector4(0, 0, 0, 0), Vector3(0, 0, 0));
	AddBind(m_TransformBuf);
}

void Box::Update(float a_DeltaTime) noexcept {};

void Box::Draw(Graphics& a_Gfx) noexcept
{
	a_Gfx.BindShader(ShaderType::VertexShader, vShader);
	a_Gfx.BindShader(ShaderType::PixelShader, pShader);

	Drawable::Draw(a_Gfx);
}

//DirectX::XMMATRIX Box::GetTransformXM() const noexcept
//{
//	return	DirectX::XMMatrixRotationRollPitchYaw(m_Pitch, m_Yaw, m_Roll) *
//			DirectX::XMMatrixTranslation(m_R, 0.0f, 0.0f) *
//			DirectX::XMMatrixRotationRollPitchYaw(m_Theta, m_Phi, m_Chi) *
//			DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
//}