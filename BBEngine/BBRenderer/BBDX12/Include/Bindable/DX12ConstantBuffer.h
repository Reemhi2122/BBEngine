#include "Bindable/IConstantBuffer.h"

template<typename T>
class DX12ConstantBuffer : public IConstantBuffer
{
public:
	DX12ConstantBuffer() = default;
	~DX12ConstantBuffer() = default;

	bool Create(IGraphics& a_Gfx, const T& a_Consts, uint32_t a_StartSlot = 0u, uint32_t a_NumBuffer = 1u)
	{
		HRESULT hres;

		for (uint32_t i = 0; i < FRAME_BUFFER_COUNT; i++)
		{
			hres = a_Gfx.GetDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(1024 * 64),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&m_ConstantBufferUploadHeaps[i])
			);
			m_ConstantBufferUploadHeaps[i]->SetName(L"CB Upload Resource Heap");

			ZeroMemory(&a_Consts, sizeof(a_Consts));

			CD3DX12_RANGE readRange(0, 0);

			hres = m_ConstantBufferUploadHeaps[i]->Map(0, &readRange, reinterpret_cast<void**>(&m_CBVGPUAdress[i]));

			memcpy(m_CBVGPUAdress[i], &a_Consts, sizeof(a_Consts));
			memcpy(m_CBVGPUAdress[i] + GET_CONSTANT_BUFFER_OFFSET(ConstantBufferPerObject), &a_Consts, sizeof(a_Consts));
		}

		return true;
	}


	void Bind() override;
	void UnBind() override;

private:
	ID3D12Resource* m_ConstantBufferUploadHeaps[FRAME_BUFFER_COUNT];
	uint8_t* m_CBVGPUAdress[FRAME_BUFFER_COUNT];
};