#include "Bindable/DX11VertexBuffer.h"
#include "SharedRenderTypes.h"
#include "BBException.h"
#include "Utils/GraphicsThrowMacros.h"

DX11VertexBuffer::DX11VertexBuffer(IGraphics& a_Gfx, void* a_Vertices, const uint32_t vbSize, const uint32_t a_Count)
	: m_Count(a_Count)
{
	INFOMAN;

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.ByteWidth = m_Count * sizeof(BBE::Vertex);
	desc.StructureByteStride = sizeof(BBE::Vertex);
	D3D11_SUBRESOURCE_DATA source = {};
	source.pSysMem = a_Vertices;

	GFX_THROW_FAILED(a_Gfx.GetDevice()->CreateBuffer(&desc, &source, &vertex_buffer));
}

//void VertexBuffer::CreateInstanceBuffer(Graphics& a_Gfx, const void* a_Consts, const uint32_t a_DataSize, const uint32_t a_Count)
//{
//	INFOMAN;
//	m_HasInstanceBuffer = true;
//	m_InstanceDataSize = a_DataSize;
//
//	D3D11_BUFFER_DESC desc = {};
//	desc.Usage = D3D11_USAGE_DEFAULT;
//	desc.ByteWidth = m_InstanceDataSize * a_Count;
//	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	desc.CPUAccessFlags = 0;
//	desc.MiscFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA source = {};
//	source.pSysMem = a_Consts;
//	
//	GFX_THROW_FAILED(a_Gfx.GetDevice()->CreateBuffer(&desc, &source, &m_InstanceBuffer));
//}

void DX11VertexBuffer::Bind(IGraphics& a_Gfx) noexcept
{
	//if (m_HasInstanceBuffer) {
	//	const UINT stride[2] = { sizeof(BBE::Vertex), m_InstanceDataSize };
	//	const UINT offset[2] = { 0, 0 };
	//	ID3D11Buffer* vertInstBuffers[2] = { *vertex_buffer.GetAddressOf(), *m_InstanceBuffer.GetAddressOf()};
	//	a_Gfx.GetContext()->IASetVertexBuffers(0, 2, vertInstBuffers, stride, offset);
	//}
	//else {
		const UINT stride = sizeof(BBE::Vertex);
		const UINT offset = 0;

		ID3D11Buffer* buffers[]{ vertex_buffer };
		a_Gfx.GetContext()->IASetVertexBuffers(0, 1, buffers, &stride, &offset);
	//}
}

UINT DX11VertexBuffer::GetCount() const noexcept
{
	return m_Count;
}
