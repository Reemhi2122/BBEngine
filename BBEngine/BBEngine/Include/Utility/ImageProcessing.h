#pragma once

namespace BBE {
	namespace Utility {

		bool Convolution(void* a_Buffer, const int a_Width, const int a_Height, const float& a_Kernel, const int& a_KernelSize, const int& multiplier = 0);
	}
}