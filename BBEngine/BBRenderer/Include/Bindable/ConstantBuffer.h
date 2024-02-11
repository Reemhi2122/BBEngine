#pragma once
#include "Bindable/Bindable.h"
#include "Utils/GraphicsThrowMacros.h"

template<typename T>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer() = default;

	ConstantBuffer(Graphics& a_Gfx) {
		INFOMAN(a_Gfx);

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(T);
		cbd.StructureByteStride = 0u;
		GFX_THROW_FAILED(a_Gfx.GetDevice()->CreateBuffer(&cbd, nullptr, &m_ConstantBuffer));
	}

	ConstantBuffer(Graphics& a_Gfx, const T& a_Consts) {
		INFOMAN(a_Gfx);

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(a_Consts);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &a_Consts;

		GFX_THROW_FAILED(a_Gfx.GetDevice()->CreateBuffer(&cbd, &csd, &m_ConstantBuffer));
	}

	void Update(Graphics& a_Gfx, const T& a_Consts) {
		INFOMAN(a_Gfx);

		D3D11_MAPPED_SUBRESOURCE msr;
		GFX_THROW_FAILED(		
			a_Gfx.GetContext()->Map(
			m_ConstantBuffer.Get(),
			0u,
			D3D11_MAP_WRITE_DISCARD,
			0u,
			&msr
			)
		);
		memcpy(msr.pData, &a_Consts, sizeof(a_Consts));
		a_Gfx.GetContext()->Unmap(m_ConstantBuffer.Get(), 0u);
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer;
};

template<typename T>
class VertexConstantBuffer : public ConstantBuffer<T>
{
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void Bind(Graphics& a_Gfx) noexcept override
	{
		a_Gfx.GetContext()->VSSetConstantBuffers(0u, 1u, m_ConstantBuffer.GetAddressOf());
	}
};

template<typename T>
class PixelConstantBuffer : public ConstantBuffer<T>
{
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void Bind(Graphics& a_Gfx) noexcept override
	{
		a_Gfx.GetContext()->PSSetConstantBuffers(0u, 1u, m_ConstantBuffer.GetAddressOf());
	}
};