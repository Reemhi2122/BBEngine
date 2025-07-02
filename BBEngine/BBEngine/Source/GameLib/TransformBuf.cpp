#include "GameLib/TransformBuf.h"

TransformBuf::TransformBuf(IGraphics& a_Gfx, Vector3 a_LocalTranslation, Vector4 a_LocalRotation, Vector3 a_LocalScale)
{
	SetTransform(TransformType::LOCAL, a_LocalTranslation, a_LocalRotation, a_LocalScale);

	m_VCB = new RootConstantBuffer<perObjectBuffer>();
	m_VCB->Create(a_Gfx);
}

TransformBuf::~TransformBuf() {
	//TODO(Stan): Use custom allocation for this
	delete(m_VCB);
}

DirectX::XMMATRIX& TransformBuf::GetMatrix(TransformType type)
{
	switch (type)
	{
	case TransformType::LOCAL:
		return m_LocalMatrix;
	case TransformType::OBJECT:
		return m_ObjTransform;
	case TransformType::PARENT:
		return m_ParentTransform;
		break;
	}
}

void TransformBuf::SetTransformRPY(TransformType type, Vector3 a_ObjectTranslation, Vector3 a_ObjectRotation, Vector3 a_ObjectScale)
{
	DirectX::XMMATRIX& matrix = GetMatrix(type);
	matrix =
		DirectX::XMMatrixScaling(a_ObjectScale.x, a_ObjectScale.y, a_ObjectScale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(a_ObjectRotation.x, a_ObjectRotation.y, a_ObjectRotation.z) *
		DirectX::XMMatrixTranslation(a_ObjectTranslation.x, a_ObjectTranslation.y, a_ObjectTranslation.z);
}

void TransformBuf::SetTransform(TransformType type, Vector3 a_Translation, Vector4 a_Rotation, Vector3 a_Scale)
{
	DirectX::XMMATRIX& matrix = GetMatrix(type);
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