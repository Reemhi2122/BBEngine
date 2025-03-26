#pragma once

#ifdef ENGINE
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3x3.h"
#include "DirectXMath.h"

#define float3 Vector3
#define float4 Vector4
#define cbuffer struct
#define float4x4 DirectX::XMMATRIX

#define CBRegister1
#else
#define CBRegister1 : register(b1)
#endif
