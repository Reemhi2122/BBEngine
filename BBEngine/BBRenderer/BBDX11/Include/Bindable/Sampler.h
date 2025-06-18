#pragma once
#include "Bindable/Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler(Graphics& a_Gfx);
	void Bind(Graphics& a_Gfx) noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerState;
};