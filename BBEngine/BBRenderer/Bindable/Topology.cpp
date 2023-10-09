#include "Bindable/Topology.h"

Topology::Topology(Graphics& a_Gfx, D3D11_PRIMITIVE_TOPOLOGY a_Type)
{
	m_Type = a_Type; //D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST
}

void Topology::Bind(Graphics& a_Gfx) noexcept
{
	GetContext(a_Gfx)->IASetPrimitiveTopology(m_Type);
}
