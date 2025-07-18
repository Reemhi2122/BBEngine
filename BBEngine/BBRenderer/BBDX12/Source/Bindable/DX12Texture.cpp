#include "Bindable/DX12Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

bool DX12Texture::Create(IGraphics& a_Gfx, const char* a_Path, uint32_t a_StartSlot, bool CreateRSV)
{
	HRESULT hres;

	int xSize = 0;
	int ySize = 0;
	int channelcount = 0;
	unsigned char* imgData = stbi_load(a_Path, &xSize, &ySize, &channelcount, 4);

	if ((xSize + ySize) <= 0)
	{
		printf("[DX12:TEXTURE]: Failed to create Texture from file!");
		return false;
	}
	D3D12_RESOURCE_DESC textureDescription = {};
	textureDescription.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDescription.Alignment = 0;
	textureDescription.Width = xSize;
	textureDescription.Height = ySize;
	textureDescription.DepthOrArraySize = 1;
	textureDescription.MipLevels = 1;
	textureDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDescription.SampleDesc.Count = 1;
	textureDescription.SampleDesc.Quality = 0;
	textureDescription.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	textureDescription.Flags = D3D12_RESOURCE_FLAG_NONE;

	hres = a_Gfx.GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&textureDescription,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_TextureBuffer)
	);
	if (FAILED(hres))
	{
		printf("[GFX::DX12:TEXTURE]: Failed to create Texture Default Heap!");
		return false;
	}
	m_TextureBuffer->SetName(L"Texture Default Heap");

	UINT64 textureUploadBufferSize = 0;
	UINT64 rowSizeInBytes = 0;
	a_Gfx.GetDevice()->GetCopyableFootprints(&textureDescription, 0, 1, 0, nullptr, nullptr, &rowSizeInBytes, &textureUploadBufferSize);

	hres = a_Gfx.GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(textureUploadBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_TextureUploadBufferHeap)
	);
	if (FAILED(hres))
	{
		printf("[GFX::DX12:TEXTURE]: Failed to create Texture Upload Heap!");
		return false;
	}
	m_TextureUploadBufferHeap->SetName(L"Texture Upload Heap");

	D3D12_SUBRESOURCE_DATA textureData = {};
	textureData.pData = &imgData[0];
	textureData.RowPitch = rowSizeInBytes;
	textureData.SlicePitch = rowSizeInBytes * ySize;

	UpdateSubresources(a_Gfx.GetCommandList(), m_TextureBuffer, m_TextureUploadBufferHeap, 0, 0, 1, &textureData);

	a_Gfx.GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_TextureBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	m_DescriptorInfo = new SRVDescriptorInfo();

	if (CreateRSV)
	{
		Graphics* gfx = static_cast<Graphics*>(&a_Gfx);
		gfx->GetMainDescriptorHeap()->Alloc(&m_DescriptorInfo->cpuDescHandle, &m_DescriptorInfo->gpuDescHandle);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDescriptorDesc = {};
		srvDescriptorDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDescriptorDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDescriptorDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDescriptorDesc.Texture2D.MipLevels = 1;

		gfx->GetDevice()->CreateShaderResourceView(m_TextureBuffer, &srvDescriptorDesc, m_DescriptorInfo->cpuDescHandle);
	}

	delete imgData;
	return true;
}

void DX12Texture::Bind(IGraphics& a_Gfx) noexcept
{
	a_Gfx.GetCommandList()->SetGraphicsRootDescriptorTable(1, m_DescriptorInfo->gpuDescHandle);
}

void DX12Texture::UnBind(IGraphics& a_Gfx) noexcept
{
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DX12Texture::GetSRVGPUHandle()
{
	return m_DescriptorInfo->gpuDescHandle;
}