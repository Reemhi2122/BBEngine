#pragma once

#define BBDX12
#if defined(BBDX12)

#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>

#include "D3DX12/d3dx12.h"

typedef ID3D12Resource	BBShaderResourceView;
typedef ID3D12Resource	BBTexture2D;

typedef ID3D12Device	BBRenderDevice;

#elif defined(BBDX11)

typedef ID3D11ShaderResourceView	BBShaderResourceView;
typedef ID3D11Texture2D				BBTexture2D;

#endif