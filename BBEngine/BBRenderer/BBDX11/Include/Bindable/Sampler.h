#pragma once
#include "Bindable/Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler(IGraphics& a_Gfx);
	void Bind(IGraphics& a_Gfx) noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerState;
};