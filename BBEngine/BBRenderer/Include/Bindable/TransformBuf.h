#pragma once
#include "Bindable/Bindable.h"
#include "Drawable/Drawable.h"
#include <DirectXMath.h>

class TransformBuf : Bindable {
public:
	TransformBuf(Graphics& a_Gfx, const Drawable& a_Parent);
	void Bind(Graphics& a_Gfx) noexcept;
	

private:
	VertexConstantBuffer<DirectX::XMMATRIX> vcbuf;
	const Drawable& m_Parent;
};