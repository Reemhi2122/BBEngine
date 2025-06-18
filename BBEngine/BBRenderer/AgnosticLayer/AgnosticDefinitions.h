#pragma once

#define BBDX11
#if defined(BBDX12)

#elif defined(BBDX11)
#include "Graphics.h"
typedef ID3D11ShaderResourceView BBShaderResourceView;
#endif