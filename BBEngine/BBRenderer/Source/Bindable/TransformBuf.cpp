#include "Bindable/TransformBuf.h"

TransformBuf::TransformBuf(Graphics& a_Gfx, const Drawable& a_Parent) :
	m_VCB(a_Gfx),
	m_Parent(a_Parent)
{}

void TransformBuf::Bind(Graphics& a_Gfx) noexcept
{
	m_VCB.Update(
		a_Gfx,
		DirectX::XMMatrixTranspose(
			m_Parent.GetTransformXM() * a_Gfx.GetProjection()
		)
	);
	m_VCB.Bind(a_Gfx);
}