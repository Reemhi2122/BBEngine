#include "Bindable/DX12IndexBuffer.h"

bool DX12IndexBuffer::Create(IGraphics& a_Gfx, uint8_t* a_Indices, const uint32_t a_Count, uint8_t a_IndexDataSize)
{
	HRESULT hres;

	m_Count = a_Count;
	uint32_t indexBufferSize = a_Count * a_IndexDataSize;

	hres = a_Gfx.GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_IndexBuffer)
	);
	m_IndexBuffer->SetName(L"Index Buffer Default Heap");
	if (FAILED(hres))
	{
		printf("[GFX::DX12:INDEXBUFFER]: Failed create Index Buffer Default Heap!");
		return false;
	}

	ID3D12Resource* indexBufferUploadHeap;
	hres = a_Gfx.GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBufferUploadHeap)
	);
	indexBufferUploadHeap->SetName(L"Index Buffer Upload Heap");
	if (FAILED(hres))
	{
		printf("[GFX::DX12:INDEXBUFFER]: Failed create Index Buffer Upload Heap!");
		return false;
	}

	D3D12_SUBRESOURCE_DATA indexData = {};
	indexData.pData = reinterpret_cast<BYTE*>(a_Indices);
	indexData.RowPitch = indexBufferSize;
	indexData.SlicePitch = indexBufferSize;

	UpdateSubresources(a_Gfx.GetCommandList(), m_IndexBuffer, indexBufferUploadHeap, 0, 0, 1, &indexData);

	a_Gfx.GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_IndexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER));

	DXGI_FORMAT format;
	switch (a_IndexDataSize)
	{
	case 2:
		format = DXGI_FORMAT_R16_UINT;
		break;
	case 4:
		format = DXGI_FORMAT_R32_UINT;
		break;
	default:
		format = DXGI_FORMAT_R32_UINT;
		break;
	}

	m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
	m_IndexBufferView.Format = format;
	m_IndexBufferView.SizeInBytes = indexBufferSize;

	return true;
}

void DX12IndexBuffer::Bind(IGraphics& a_Gfx) noexcept
{
	a_Gfx.GetCommandList()->IASetIndexBuffer(&m_IndexBufferView);
}

UINT DX12IndexBuffer::GetCount() const noexcept
{
	return m_Count;
}
