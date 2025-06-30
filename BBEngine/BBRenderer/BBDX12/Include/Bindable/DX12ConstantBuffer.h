#include "Bindable/IConstantBuffer.h"

struct ConstantUploadBufferReference
{
	uint8_t* m_CBVGPUAdress[FRAME_BUFFER_COUNT];
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

		Graphics* gfx = static_cast<Graphics*>(&a_Gfx);
		gfx->GetRootConstantUploadBufferView(a_StartSlot, sizeof(T), m_ConstantBufferHeaps);

		for (uint32_t i = 0; i < FRAME_BUFFER_COUNT; i++)
		{
			ZeroMemory(m_ConstantBufferHeaps.m_CBVGPUAdress[i], sizeof(T));
			memcpy(m_ConstantBufferHeaps.m_CBVGPUAdress[i], &m_ConstantBuffer, sizeof(m_ConstantBuffer));
		}

		return true;
	}

	ConstantUploadBufferReference* GetConstantUploadBufferReference() noexcept { return &m_ConstantBufferHeaps; };
	T* GetConstantBuffer() noexcept { return &m_ConstantBuffer; };

	void Bind(IGraphics& a_Gfx) noexcept override
	{
		return; 
	};

private:
	ConstantUploadBufferReference m_ConstantBufferHeaps;
	T m_ConstantBuffer;
};
