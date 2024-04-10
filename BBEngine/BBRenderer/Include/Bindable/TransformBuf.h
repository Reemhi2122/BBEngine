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
	TransformBuf(Graphics& a_Gfx, Vector3 a_LocalTranslation, Vector3 a_LocalRotation, Vector3 a_LocalScale);
	~TransformBuf();
	void Bind(Graphics& a_Gfx) noexcept;
	 
	void SetCurrentParentTransform(DirectX::XMMATRIX a_ParentTransform);

private:
	static VertexConstantBuffer<perObjectBuffer>* m_VCB;
	DirectX::XMMATRIX m_LocalMatrix;
	DirectX::XMMATRIX m_ParentTransform;
};