#pragma once
#include "Bindable/Bindable.h"

class DX11Sampler : public Bindable
{
public:
	DX11Sampler(IGraphics& a_Gfx);
	~DX11Sampler() = default;
	void Bind(IGraphics& a_Gfx) noexcept override;

private:
	ID3D11SamplerState* m_SamplerState;
};