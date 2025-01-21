#pragma once
#include "ShaderDefines.h"

cbuffer vcbPerFrame CBRegister1 {
	float4x4 lightMatrix[120];
};