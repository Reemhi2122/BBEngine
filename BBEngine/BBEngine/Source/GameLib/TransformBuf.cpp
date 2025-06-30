#include "GameLib/TransformBuf.h"

TransformBuf::TransformBuf(IGraphics& a_Gfx, Vector3 a_LocalTranslation, Vector4 a_LocalRotation, Vector3 a_LocalScale)
{
	UpdateTransform(a_LocalTranslation, a_LocalRotation, a_LocalScale);

	if (!m_VCB) {
		m_VCB = new RootConstantBuffer<perObjectBuffer>();
		m_VCB->Create(a_Gfx);
	}
}

TransformBuf::~TransformBuf() {
	//TODO(Stan): Use custom allocation for this
	delete(m_VCB);
}

void TransformBuf::UpdateTransform(Vector3 a_LocalTranslation, Vector4 a_LocalRotation, Vector3 a_LocalScale)
{
	m_LocalMatrix =
		DirectX::XMMatrixScaling(a_LocalScale.x, a_LocalScale.y, a_LocalScale.z) *
		DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(a_LocalRotation.x, a_LocalRotation.y, a_LocalRotation.z, a_LocalRotation.w)) *
		DirectX::XMMatrixTranslation(a_LocalTranslation.x, a_LocalTranslation.y, a_LocalTranslation.z);
}

void TransformBuf::SetParentTransform(Vector3 a_ParentTranslation, Vector4 a_ParentRotation, Vector3 a_ParentScale)
{
	m_ParentTransform =
		DirectX::XMMatrixScaling(a_ParentScale.x, a_ParentScale.y, a_ParentScale.z) *
		DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(a_ParentRotation.x, a_ParentRotation.y, a_ParentRotation.z, a_ParentRotation.w)) *
		DirectX::XMMatrixTranslation(a_ParentTranslation.x, a_ParentTranslation.y, a_ParentTranslation.z);
}

void TransformBuf::Bind(IGraphics& a_Gfx) noexcept
{
	DirectX::XMMATRIX finalMatrix = m_LocalMatrix * m_ParentTransform; 

	perObjectBuffer buf = {
		DirectX::XMMatrixTranspose(DirectX::XMMatrixIdentity() * a_Gfx.GetCamera()->GetViewMatrix() * a_Gfx.GetCamera()->GetProjection()),
		DirectX::XMMatrixTranspose(finalMatrix)
	};

	m_VCB->Update(a_Gfx, buf);
	m_VCB->Bind(a_Gfx);
}

void TransformBuf::Bind(IGraphics& a_Gfx, DirectX::XMMATRIX a_ObjTransform) noexcept
{
	DirectX::XMMATRIX finalMatrix = m_LocalMatrix * a_ObjTransform * m_ParentTransform;

	perObjectBuffer buf = {
		DirectX::XMMatrixTranspose(DirectX::XMMatrixIdentity() * a_Gfx.GetCamera()->GetViewMatrix() * a_Gfx.GetCamera()->GetProjection()),
		DirectX::XMMatrixTranspose(finalMatrix)
	};

	m_VCB->Update(a_Gfx, buf);
	m_VCB->Bind(a_Gfx);
}

RootConstantBuffer<perObjectBuffer>* TransformBuf::m_VCB;