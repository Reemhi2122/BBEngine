#pragma once
#include "Bindable/Bindable.h"

class Topology : public Bindable
{
public:
	Topology(Graphics& a_Gfx, D3D11_PRIMITIVE_TOPOLOGY a_Type);
	void Bind(Graphics& a_Gfx) noexcept;

protected:
	D3D11_PRIMITIVE_TOPOLOGY m_Type;
};