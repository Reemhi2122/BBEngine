#include "Utility/ImageProcessing.h"
#include <cstdint>
#include <algorithm>

namespace BBE {
	namespace Utility {



		bool Convolution(uint8_t* a_Buffer, const int a_Width, const int a_Height, const int a_ChannelCount, const float* a_Kernel, const int& a_KernelSize, const int& multiplier)
		{
			for (int y = 0; y < a_Height; y++) {
				for (int x = 0; x < a_Width; x++) {

					int matrixIndex = 0;
					uint8_t RAccumulator = 0;
					uint8_t GAccumulator = 0;
					uint8_t BAccumulator = 0;

					uint32_t accum = 0;

					for (int r = -1; r < 2; r++)
					{
						for (int c = -1; c < 2; c++) {
							int y0 = std::clamp((y + r), 0, (int)a_Height);
							int x0 = std::clamp((x + c), 0, (int)a_Width);

							uint32_t PixelIndex = a_ChannelCount * (y0 * a_Width + x0);

							RAccumulator += a_Buffer[PixelIndex + 0] * a_Kernel[matrixIndex] * multiplier;
							GAccumulator += a_Buffer[PixelIndex + 1] * a_Kernel[matrixIndex] * multiplier;
							BAccumulator += a_Buffer[PixelIndex + 2] * a_Kernel[matrixIndex] * multiplier;
							matrixIndex++;
						}
					}

					uint32_t PixelIndex = a_ChannelCount * (y * a_Width + x);
					a_Buffer[PixelIndex] = accum;
					a_Buffer[PixelIndex + 0] = RAccumulator;
					a_Buffer[PixelIndex + 1] = GAccumulator;
					a_Buffer[PixelIndex + 2] = BAccumulator;
				}
			}

			return false;
		}
	}
}