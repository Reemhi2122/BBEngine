#include "Bindable/DX12VertexBuffer.h"

void DX12VertexBuffer::Create(IGraphics& a_Gfx, void* a_Vertices, const uint32_t vbSize, const uint32_t a_Count)
{
	HRESULT hres;

	m_Count = a_Count;
	uint32_t vbTotalSize = (vbSize * a_Count);

	hres = a_Gfx.GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(vbTotalSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_VertexBuffer)
	);

	m_VertexBuffer->SetName(L"Vertex Buffer Default Heap");
	if (FAILED(hres))
	{
		printf("[GFX]: Failed create Vertex Buffer Default Heap!");
	}

	ID3D12Resource* vertexBufferUploadHeap = {};
	hres = a_Gfx.GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(vbTotalSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBufferUploadHeap)
	);
	vertexBufferUploadHeap->SetName(L"Vertex Buffer Upload Heap");
	if (FAILED(hres))
	{
		printf("[GFX]: Failed create Vertex Buffer Upload Heap!");
	}

	D3D12_SUBRESOURCE_DATA vertexData = {};
	vertexData.pData = reinterpret_cast<BYTE*>(a_Vertices);
	vertexData.RowPitch = vbTotalSize;
	vertexData.SlicePitch = vbTotalSize;

	UpdateSubresources(a_Gfx.GetCommandList(), m_VertexBuffer, vertexBufferUploadHeap, 0, 0, 1, &vertexData);

	a_Gfx.GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_VertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
	m_VertexBufferView.StrideInBytes = vbSize;
	m_VertexBufferView.SizeInBytes = vbTotalSize;
}

void DX12VertexBuffer::Bind(IGraphics& a_Gfx) noexcept
{
	a_Gfx.GetCommandList()->IASetVertexBuffers(0, 1, &m_VertexBufferView);
}

UINT DX12VertexBuffer::GetCount() const noexcept
{
	return 0;
}
