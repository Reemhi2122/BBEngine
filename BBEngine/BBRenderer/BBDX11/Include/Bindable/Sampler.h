#pragma once
#include "Bindable/Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler(IGraphics& a_Gfx);
	~Sampler() = default;
	void Bind(IGraphics& a_Gfx) noexcept override;

private:
	ID3D11SamplerState* m_SamplerState;
};