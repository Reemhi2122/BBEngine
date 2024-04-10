#include "Bindable/TransformBuf.h"

TransformBuf::TransformBuf(Graphics& a_Gfx, Vector3 a_LocalTranslation, Vector3 a_LocalRotation, Vector3 a_LocalScale)
{
	m_LocalMatrix = 
		DirectX::XMMatrixScaling(a_LocalScale.x, a_LocalScale.y, a_LocalScale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(a_LocalRotation.x, a_LocalRotation.y, a_LocalRotation.z) *
		DirectX::XMMatrixTranslation(a_LocalTranslation.x, a_LocalTranslation.y, a_LocalTranslation.z);

	if (!m_VCB) {
		m_VCB = new VertexConstantBuffer<perObjectBuffer>(a_Gfx);
	}
}

TransformBuf::~TransformBuf() {
	//TODO(Stan): Use custom allocation for this
	delete(m_VCB);
}

void TransformBuf::SetCurrentParentTransform(DirectX::XMMATRIX a_ParentTransform) 
{
	m_ParentTransform = a_ParentTransform;
}

void TransformBuf::Bind(Graphics& a_Gfx) noexcept
{
	perObjectBuffer buf = {
		DirectX::XMMatrixTranspose((m_LocalMatrix * m_ParentTransform) * a_Gfx.GetCamera()->GetViewMatrix() * a_Gfx.GetProjection()),
		DirectX::XMMatrixTranspose((m_LocalMatrix * m_ParentTransform))
	};

	m_VCB->Update(a_Gfx, buf);
	m_VCB->Bind(a_Gfx);
}

VertexConstantBuffer<perObjectBuffer>* TransformBuf::m_VCB;