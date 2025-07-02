#include "GameLib/TransformBuf.h"

TransformBuf::TransformBuf(IGraphics& a_Gfx, Vector3 a_LocalTranslation, Vector4 a_LocalRotation, Vector3 a_LocalScale)
{
	SetLocalTransform(a_LocalTranslation, a_LocalRotation, a_LocalScale);

	m_VCB = new RootConstantBuffer<perObjectBuffer>();
	m_VCB->Create(a_Gfx);
}

TransformBuf::~TransformBuf() {
	//TODO(Stan): Use custom allocation for this
	delete(m_VCB);
}

void TransformBuf::SetLocalTransform(Vector3 a_LocalTranslation, Vector4 a_LocalRotation, Vector3 a_LocalScale)
{
	SetTransform(m_LocalMatrix, a_LocalTranslation, a_LocalRotation, a_LocalScale);
}

void TransformBuf::SetObjectTransform(Vector3 a_ObjectTranslation, Vector4 a_ObjectRotation, Vector3 a_ObjectScale)
{
	SetTransform(m_ObjTransform, a_ObjectTranslation, a_ObjectRotation, a_ObjectScale);
}

void TransformBuf::SetObjectTransform(Vector3 a_ObjectTranslation, Vector3 a_ObjectRotation, Vector3 a_ObjectScale)
{
	m_ObjTransform =
		DirectX::XMMatrixScaling(a_ObjectScale.x, a_ObjectScale.y, a_ObjectScale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(a_ObjectRotation.x, a_ObjectRotation.y, a_ObjectRotation.z) *
		DirectX::XMMatrixTranslation(a_ObjectTranslation.x, a_ObjectTranslation.y, a_ObjectTranslation.z);
}

void TransformBuf::SetParentTransform(Vector3 a_ParentTranslation, Vector4 a_ParentRotation, Vector3 a_ParentScale)
{
	SetTransform(m_ParentTransform, a_ParentTranslation, a_ParentRotation, a_ParentScale);
}

void TransformBuf::SetTransform(DirectX::XMMATRIX& matrix, Vector3 a_Translation, Vector4 a_Rotation, Vector3 a_Scale)
{
	matrix =
		DirectX::XMMatrixScaling(a_Scale.x, a_Scale.y, a_Scale.z) *
		DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(a_Rotation.x, a_Rotation.y, a_Rotation.z, a_Rotation.w)) *
		DirectX::XMMatrixTranslation(a_Translation.x, a_Translation.y, a_Translation.z);
}

void TransformBuf::Bind(IGraphics& a_Gfx) noexcept
{
	DirectX::XMMATRIX finalMatrix = m_LocalMatrix * m_ObjTransform * m_ParentTransform; 

	perObjectBuffer buf = {
		DirectX::XMMatrixTranspose(DirectX::XMMatrixIdentity() * a_Gfx.GetCamera()->GetViewMatrix() * a_Gfx.GetCamera()->GetProjection()),
		DirectX::XMMatrixTranspose(finalMatrix)
	};

	m_VCB->Update(a_Gfx, buf);
	m_VCB->Bind(a_Gfx);
}

//void TransformBuf::Bind(IGraphics& a_Gfx, DirectX::XMMATRIX a_ObjTransform) noexcept
//{
//	DirectX::XMMATRIX finalMatrix = m_LocalMatrix * a_ObjTransform * m_ParentTransform;
//
//	perObjectBuffer buf = {
//		DirectX::XMMatrixTranspose(DirectX::XMMatrixIdentity() * a_Gfx.GetCamera()->GetViewMatrix() * a_Gfx.GetCamera()->GetProjection()),
//		DirectX::XMMatrixTranspose(finalMatrix)
//	};
//
//	m_VCB->Update(a_Gfx, buf);
//	m_VCB->Bind(a_Gfx);
//}
