#include "Drawable/Drawable.h"
#include "Drawable/Box.h"
#include "Utils/GraphicsThrowMacros.h"

Box::Box(Graphics& a_Gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist)
	:
	m_R(rdist(rng)),
	m_DRoll(ddist(rng)),
	m_DPitch(ddist(rng)),
	m_DYaw(ddist(rng)),
	m_DPhi(odist(rng)),
	m_DTheta(odist(rng)),
	m_DChi(odist(rng)),
	m_Chi(adist(rng)),
	m_Theta(adist(rng)),
	m_Phi(adist(rng))
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

		vShader = new VertexShader(a_Gfx, L"Assets/VertexShader.hlsl");
		AddStaticBind(vShader);

		pShader = new PixelShader(a_Gfx, L"Assets/PixelShader.hlsl");
		AddStaticBind(pShader);

		uint32_t indices[] = {
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4
		};

		IBuffer = new IndexBuffer(a_Gfx, indices, 36);
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

		m_InputLayout = new InputLayout(a_Gfx, ied, vShader->GetByteCode());
		AddStaticBind(m_InputLayout);

		m_Topology = new Topology(a_Gfx, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		AddStaticBind(m_Topology);

	}
	else {
		AddIndexFromStatic();
	}

	m_TransformBuf = new TransformBuf(a_Gfx, *this, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0));
	AddBind(m_TransformBuf);
}

void Box::Update(float a_DeltaTime) noexcept
{
	m_Roll += m_DRoll * a_DeltaTime;
	m_Pitch += m_DPitch * a_DeltaTime;
	m_Yaw += m_DYaw * a_DeltaTime;
	m_Theta += m_DTheta * a_DeltaTime;
	m_Phi += m_DPhi * a_DeltaTime;
	m_Chi += m_DChi * a_DeltaTime;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return	DirectX::XMMatrixRotationRollPitchYaw(m_Pitch, m_Yaw, m_Roll) *
			DirectX::XMMatrixTranslation(m_R, 0.0f, 0.0f) *
			DirectX::XMMatrixRotationRollPitchYaw(m_Theta, m_Phi, m_Chi) *
			DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}