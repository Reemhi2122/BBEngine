#include "Bindable/TransformBuf.h"

TransformBuf::TransformBuf(Graphics& a_Gfx, const Drawable& a_Parent) :
	m_Parent(a_Parent)
{
	if (!m_VCB) {
		m_VCB = new VertexConstantBuffer<perObjectBuffer>(a_Gfx);
	}
}

TransformBuf::~TransformBuf() {
	//TODO(Stan): Use custom allocation for this
	delete(m_VCB);
}

void TransformBuf::Bind(Graphics& a_Gfx) noexcept
{
	perObjectBuffer buf = {
		DirectX::XMMatrixTranspose(m_Parent.GetTransformXM() * a_Gfx.GetCamera()->GetViewMatrix() * a_Gfx.GetProjection()),
		DirectX::XMMatrixTranspose(m_Parent.GetTransformXM())
	};

	m_VCB->Update(a_Gfx, buf);
	m_VCB->Bind(a_Gfx);
}

VertexConstantBuffer<perObjectBuffer>* TransformBuf::m_VCB;