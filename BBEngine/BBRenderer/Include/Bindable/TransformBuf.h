#pragma once
#include "Bindable/ConstantBuffer.h"
#include "Drawable/Drawable.h"
#include <DirectXMath.h>

struct perObjectBuffer {
	DirectX::XMMATRIX mvp;
	DirectX::XMMATRIX world;
};

class TransformBuf : public Bindable {
public:
	TransformBuf(Graphics& a_Gfx, Vector3 a_LocalTranslation, Vector4 a_LocalRotation, Vector3 a_LocalScale);
	~TransformBuf();
	void Bind(Graphics& a_Gfx) noexcept;
	 
	void SetCurrentObjTransform(DirectX::XMMATRIX a_ObjTransform);
	void SetParentTransform(Vector3 a_LocalTranslation, Vector4 a_LocalRotation, Vector3 a_LocalScale);

private:
	static VertexConstantBuffer<perObjectBuffer>* m_VCB;
	DirectX::XMMATRIX m_LocalMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_ObjTransform = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_ParentTransform = DirectX::XMMatrixIdentity();
};