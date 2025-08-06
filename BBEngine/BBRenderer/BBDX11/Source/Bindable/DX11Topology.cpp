#include "Bindable/DX11Topology.h"

DX11Topology::DX11Topology(IGraphics& a_Gfx, D3D11_PRIMITIVE_TOPOLOGY a_Type)
{
	m_Type = a_Type; //D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST
}

void DX11Topology::Bind(IGraphics& a_Gfx) noexcept
{
	a_Gfx.GetContext()->IASetPrimitiveTopology(m_Type);
}
