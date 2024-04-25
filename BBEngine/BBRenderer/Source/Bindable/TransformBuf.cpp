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

void TransformBuf::SetCurrentObjTransform(DirectX::XMMATRIX a_ObjTransform)
{
	m_ObjTransform = a_ObjTransform;
}

void TransformBuf::SetParentTransform(Vector3 a_ParentTranslation, Vector3 a_ParentRotation, Vector3 a_ParentScale)
{
	m_ParentTransform =
		DirectX::XMMatrixScaling(a_ParentScale.x, a_ParentScale.y, a_ParentScale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(a_ParentRotation.x, a_ParentRotation.y, a_ParentRotation.z) *
		DirectX::XMMatrixTranslation(a_ParentTranslation.x, a_ParentTranslation.y, a_ParentTranslation.z);
}

void TransformBuf::Bind(Graphics& a_Gfx) noexcept
{
	DirectX::XMMATRIX finalMatrix = m_LocalMatrix * m_ParentTransform * m_ObjTransform;

	perObjectBuffer buf = {
		DirectX::XMMatrixTranspose(finalMatrix * a_Gfx.GetCamera()->GetViewMatrix() * a_Gfx.GetProjection()),
		DirectX::XMMatrixTranspose(finalMatrix)
	};

	m_VCB->Update(a_Gfx, buf);
	m_VCB->Bind(a_Gfx);
}

VertexConstantBuffer<perObjectBuffer>* TransformBuf::m_VCB;