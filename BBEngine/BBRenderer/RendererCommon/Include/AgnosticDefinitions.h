#pragma once
#include "Graphics.h"

#define BBDX12
#if defined(BBDX12)

typedef ID3D12Resource	BBShaderResourceView;
typedef ID3D12Resource	BBTexture2D;

#elif defined(BBDX11)

typedef ID3D11ShaderResourceView	BBShaderResourceView;
typedef ID3D11Texture2D				BBTexture2D;

#endif