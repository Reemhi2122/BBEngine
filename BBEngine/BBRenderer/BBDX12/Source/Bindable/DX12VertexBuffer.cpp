#include "Bindable/VertexBuffer.h"

VertexBuffer::VertexBuffer(IGraphics& a_Gfx, void* a_Vertices, const uint32_t a_Count)
	: m_Count(a_Count)
{
	HRESULT hres;

	//hres = m_Device->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
	//	D3D12_HEAP_FLAG_NONE,
	//	&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
	//	D3D12_RESOURCE_STATE_COPY_DEST,
	//	nullptr,
	//	IID_PPV_ARGS(&m_VertexBuffer)
	//);
}

void VertexBuffer::Bind(IGraphics& a_Gfx) noexcept
{

}

UINT VertexBuffer::GetCount() const noexcept
{
	return 0;
}
