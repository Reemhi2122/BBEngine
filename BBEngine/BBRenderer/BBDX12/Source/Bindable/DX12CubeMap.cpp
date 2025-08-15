#include "Bindable/DX12CubeMap.h"

#include "stb_image.h"

bool DX12CubeMap::Create(IGraphics& a_Gfx)
{
	HRESULT hres;
	constexpr char* names[CUBEMAP_SIZE] = { "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg" };

	int sizeX, sizeY, Channels;
	for (uint32_t i = 0; i < CUBEMAP_SIZE; i++)
	{
		char path[MAX_PATH] = "Assets/Textures/skybox/";
		strcat(path, names[i]);
		images[i] = stbi_load(path, &sizeX, &sizeY, &Channels, 4);
		assert(images[i] != nullptr);
	}

	D3D12_RESOURCE_DESC textureDescription = {};
	textureDescription.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDescription.Alignment = 0;
	textureDescription.Width = sizeX;
	textureDescription.Height = sizeY;
	textureDescription.DepthOrArraySize = 6;
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
	UINT64 rowSizeInBytes[6] = {};
	a_Gfx.GetDevice()->GetCopyableFootprints(&textureDescription, 0, 6, 0, nullptr, nullptr, rowSizeInBytes, &textureUploadBufferSize);

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

	D3D12_SUBRESOURCE_DATA textureData[CUBEMAP_SIZE] = {};
	for (uint32_t i = 0; i < CUBEMAP_SIZE; i++)
	{
		textureData[i].pData = images[i];
		textureData[i].RowPitch = rowSizeInBytes[i];
		textureData[i].SlicePitch = rowSizeInBytes[i] * sizeY;
	}

	UpdateSubresources(a_Gfx.GetCommandList(), m_TextureBuffer, m_TextureUploadBufferHeap, 0, 0, 6, textureData);

	a_Gfx.GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_TextureBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	m_DescriptorInfo = new DescriptorHandleInfo();
	Graphics* gfx = static_cast<Graphics*>(&a_Gfx);
	gfx->GetMainDescriptorHeap()->Alloc(&m_DescriptorInfo->cpuDescHandle, &m_DescriptorInfo->gpuDescHandle);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDescriptorDesc = {};
	srvDescriptorDesc.Format = textureDescription.Format;
	srvDescriptorDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
	srvDescriptorDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDescriptorDesc.TextureCube.MipLevels = textureDescription.MipLevels;

	gfx->GetDevice()->CreateShaderResourceView(m_TextureBuffer, &srvDescriptorDesc, m_DescriptorInfo->cpuDescHandle);

	return false;
}

bool DX12CubeMap::Create(IGraphics& a_Gfx, CubeMapType a_Type, uint32_t a_Resolution, char* a_TexturePaths)
{
	return false;
}

void DX12CubeMap::Bind(IGraphics& a_Gfx) noexcept
{
	a_Gfx.GetCommandList()->SetGraphicsRootDescriptorTable(1, m_DescriptorInfo->gpuDescHandle);
}
