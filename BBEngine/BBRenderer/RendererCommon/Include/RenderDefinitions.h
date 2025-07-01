#pragma once

#define BBDX12
#if defined(BBDX12)

typedef class ID3D12Resource BBShaderResourceView;
typedef class ID3D12Resource BBTexture2D;

typedef class ID3D12Device   BBRenderDevice;


typedef class ID3D12GraphicsCommandList BBGraphicsCommandList;

#elif defined(BBDX11)

typedef ID3D11ShaderResourceView	BBShaderResourceView;
typedef ID3D11Texture2D				BBTexture2D;

typedef ID3D11Device				BBRenderDevice;

#endif