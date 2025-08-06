#include "Bindable/Sampler.h"

Sampler::Sampler(IGraphics& a_Gfx)
{
	D3D11_SAMPLER_DESC image_sampler_desc = {};
	image_sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	image_sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	image_sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	image_sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	image_sampler_desc.MaxAnisotropy = 1;
	image_sampler_desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	image_sampler_desc.BorderColor[0] = 1.0f;
	image_sampler_desc.BorderColor[1] = 1.0f;
	image_sampler_desc.BorderColor[2] = 1.0f;
	image_sampler_desc.BorderColor[3] = 1.0f;
	image_sampler_desc.MinLOD = -FLT_MAX;
	image_sampler_desc.MaxLOD = FLT_MAX;

	a_Gfx.GetDevice()->CreateSamplerState(&image_sampler_desc, &m_SamplerState);
}

void Sampler::Bind(IGraphics& a_Gfx) noexcept
{
	ID3D11SamplerState* samplerStates[] { m_SamplerState };
	a_Gfx.GetContext()->PSSetSamplers(0, 1, samplerStates);
}