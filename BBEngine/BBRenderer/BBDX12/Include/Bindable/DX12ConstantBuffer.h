#pragma once
#include "Bindable/IConstantBuffer.h"
#include "Graphics.h"
#include "tesing.h"

struct ConstantUploadBufferReference
{
	uint8_t* cbvGPUAdress[FRAME_BUFFER_COUNT];
	ID3D12Resource* uploadHeaps[FRAME_BUFFER_COUNT];
	uint32_t uploadHeapOffset = 0;
};

template<typename T>
class DX12ConstantBuffer : public IConstantBuffer<T>
{
public:
	DX12ConstantBuffer() = default;
	~DX12ConstantBuffer() = default;

	bool Create(IGraphics& a_Gfx, uint32_t a_StartSlot = 0u, uint32_t a_NumBuffer = 1u) override
	{
		HRESULT hres;

		m_Slot = a_StartSlot;

		Graphics* gfx = static_cast<Graphics*>(&a_Gfx);
		gfx->GetRootConstantUploadBufferView(a_StartSlot, sizeof(T), m_ConstantBufferHeaps);

		for (uint32_t i = 0; i < FRAME_BUFFER_COUNT; i++)
		{
			ZeroMemory(m_ConstantBufferHeaps.cbvGPUAdress[i], sizeof(T));
			memcpy(m_ConstantBufferHeaps.cbvGPUAdress[i], &m_ConstantBuffer, sizeof(m_ConstantBuffer));
		}

		return true;
	}

	void Update(IGraphics& a_Gfx, T& a_ConstantBuffer) override
	{
		memcpy(m_ConstantBufferHeaps.cbvGPUAdress[a_Gfx.GetCurrentFrame()], &a_ConstantBuffer, sizeof(T));
	}

	void Update(IGraphics& a_Gfx) override
	{
		memcpy(m_ConstantBufferHeaps.cbvGPUAdress[a_Gfx.GetCurrentFrame()], &m_ConstantBuffer, sizeof(T));
	}

	ConstantUploadBufferReference* GetConstantUploadBufferReference() noexcept { return &m_ConstantBufferHeaps; };
	T* GetConstantBuffer() noexcept { return &m_ConstantBuffer; };

protected:
	ConstantUploadBufferReference m_ConstantBufferHeaps;
	T m_ConstantBuffer;

	uint32_t m_Slot = 0;
};

template<typename T>
class RootConstantBuffer : public DX12ConstantBuffer<T>
{
public:
	void Bind(IGraphics& a_Gfx) noexcept override
	{
		a_Gfx.GetCommandList()->SetGraphicsRootConstantBufferView(0, m_ConstantBufferHeaps.uploadHeaps[a_Gfx.GetCurrentFrame()]->GetGPUVirtualAddress() + m_ConstantBufferHeaps.uploadHeapOffset);
	}
};