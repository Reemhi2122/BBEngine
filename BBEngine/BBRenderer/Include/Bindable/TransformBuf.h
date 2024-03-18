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
	TransformBuf(Graphics& a_Gfx, const Drawable& a_Parent, Vector3 a_LocalTranslation, Vector3 a_LocalRotation, Vector3 a_LocalScale);
	~TransformBuf();
	void Bind(Graphics& a_Gfx) noexcept;
	
private:
	static VertexConstantBuffer<perObjectBuffer>* m_VCB;
	const Drawable& m_Parent;
	DirectX::XMMATRIX m_LocalMatrix;
};