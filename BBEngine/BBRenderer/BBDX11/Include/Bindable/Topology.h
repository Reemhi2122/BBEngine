#pragma once
#include "Bindable/Bindable.h"

class Topology : public Bindable
{
public:
	Topology(IGraphics& a_Gfx, D3D11_PRIMITIVE_TOPOLOGY a_Type);
	void Bind(IGraphics& a_Gfx) noexcept;

protected:
	D3D11_PRIMITIVE_TOPOLOGY m_Type;
};