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

#define CBRegister0
#define CBRegister1
#define CBRegister2
#define CBRegister3
#else
#define CBRegister0 : register(b0)
#define CBRegister1 : register(b1)
#define CBRegister2 : register(b2)
#define CBRegister3 : register(b3)
#endif
