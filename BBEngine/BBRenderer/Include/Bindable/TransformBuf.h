#pragma once
#include "Bindable/ConstantBuffer.h"
#include "Drawable/Drawable.h"
#include <DirectXMath.h>

class TransformBuf : public Bindable {
public:
	TransformBuf(Graphics& a_Gfx, const Drawable& a_Parent);
	void Bind(Graphics& a_Gfx) noexcept;
	
private:
	VertexConstantBuffer<DirectX::XMMATRIX> m_VCB;
	const Drawable& m_Parent;
};