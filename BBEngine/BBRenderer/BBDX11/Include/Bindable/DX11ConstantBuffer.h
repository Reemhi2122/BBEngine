#pragma once
#include "Bindable/IConstantBuffer.h"
#include "BBException.h"
#include "Utils/GraphicsThrowMacros.h"

template<typename T>
class DX11ConstantBuffer : public IConstantBuffer<T>
{
public:
	DX11ConstantBuffer() = default;

	//ConstantBuffer(Graphics& a_Gfx, uint32_t a_StartSlot = 0u, uint32_t a_NumBuffer = 1u)
	//	: m_StartSlot(a_StartSlot), m_NumBuffers(a_NumBuffer)
	//{
	//	INFOMAN;

	//	D3D11_BUFFER_DESC cbd;
	//	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//	cbd.Usage = D3D11_USAGE_DEFAULT;
	//	cbd.CPUAccessFlags = 0;
	//	cbd.MiscFlags = 0u;
	//	cbd.ByteWidth = std::max<int>(sizeof(T), 16);
	//	cbd.StructureByteStride = 0u;
	//	GFX_THROW_FAILED(a_Gfx.GetDevice()->CreateBuffer(&cbd, nullptr, &m_ConstantBuffer));
	//}

	bool Create(IGraphics& a_Gfx, uint32_t a_StartSlot = 0u, uint32_t a_NumBuffer = 1u) override
	{
		INFOMAN;

		m_StartSlot = a_StartSlot;
		m_NumBuffers = a_NumBuffer;

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = std::max<int>(sizeof(T), 16);
		cbd.StructureByteStride = 0u;
		GFX_THROW_FAILED(a_Gfx.GetDevice()->CreateBuffer(&cbd, nullptr, &m_ConstantBuffer));
	}

	//DX11ConstantBuffer(IGraphics& a_Gfx, const T& a_Consts, uint32_t a_StartSlot = 0u, uint32_t a_NumBuffer = 1u)
	//	: m_StartSlot(a_StartSlot), m_NumBuffers(a_NumBuffer)
	//{
	//	INFOMAN;

	//	D3D11_BUFFER_DESC cbd;
	//	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//	cbd.Usage = D3D11_USAGE_DYNAMIC;
	//	//cbd.Usage = D3D11_USAGE_DEFAULT;
	//	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//	//cbd.CPUAccessFlags = 0;
	//	cbd.MiscFlags = 0u;
	//	cbd.ByteWidth = sizeof(a_Consts);
	//	cbd.StructureByteStride = 0u;

	//	D3D11_SUBRESOURCE_DATA csd = {};
	//	csd.pSysMem = &a_Consts;

	//	GFX_THROW_FAILED(a_Gfx.GetDevice()->CreateBuffer(&cbd, &csd, &m_ConstantBuffer));
	//}

	void Update(IGraphics& a_Gfx) override
	{}

	virtual void Bind(IGraphics& a_Gfx) noexcept override
	{}

	void Update(IGraphics& a_Gfx, T& a_ConstantBuffer) override
	{
		INFOMAN;

		D3D11_MAPPED_SUBRESOURCE msr;
		GFX_THROW_FAILED(		
			a_Gfx.GetContext()->Map(
			m_ConstantBuffer,
			0u,
			D3D11_MAP_WRITE_DISCARD,
			0u,
			&msr
			)
		);
		memcpy(msr.pData, &a_Consts, sizeof(T));
		a_Gfx.GetContext()->Unmap(m_ConstantBuffer, 0u);
	}

	T* GetConstantBuffer() override { return nullptr; };

	//void Update(IGraphics& a_Gfx, const T& a_Consts)
	//{
	//	a_Gfx.GetContext()->UpdateSubresource(m_ConstantBuffer.Get(), 0, nullptr, &a_Consts, 0, 0);
	//}

protected:
	ID3D11Buffer* m_ConstantBuffer;

	uint32_t m_StartSlot;
	uint32_t m_NumBuffers;
};

template<typename T>
class VertexConstantBuffer : public DX11ConstantBuffer<T>
{
public:
	using DX11ConstantBuffer<T>::DX11ConstantBuffer;
	void Bind(IGraphics& a_Gfx) noexcept override
	{
		ID3D11Buffer* buffers[]{ m_ConstantBuffer };
		a_Gfx.GetContext()->VSSetConstantBuffers(m_StartSlot, m_NumBuffers, buffers);
	}

	void UnBind(IGraphics& a_Gfx) noexcept override
	{
		a_Gfx.GetContext()->VSSetConstantBuffers(m_StartSlot, m_NumBuffers, nullptr);
	}
};

template<typename T>
using RootConstantBuffer = VertexConstantBuffer<T>;

template<typename T>
class PixelConstantBuffer : public DX11ConstantBuffer<T>
{
public:
	using DX11ConstantBuffer<T>::DX11ConstantBuffer;
	void Bind(IGraphics& a_Gfx) noexcept override
	{
		ID3D11Buffer* buffers[]{ m_ConstantBuffer };
		a_Gfx.GetContext()->PSSetConstantBuffers(m_StartSlot, m_NumBuffers, buffers);
	}

	void UnBind(IGraphics& a_Gfx) noexcept override
	{
		ID3D11Buffer* nullBuffer[1]{ nullptr };
		a_Gfx.GetContext()->PSSetConstantBuffers(m_StartSlot, m_NumBuffers, nullBuffer);
	}
};