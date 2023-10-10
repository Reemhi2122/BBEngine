#include "Utility/ImageProcessing.h"
#include <algorithm>

namespace BBE {
	namespace Utility {

		bool Convolution(const ConvolutionDesc& a_Desc)
		{
			for (int y = 0; y < a_Desc.height; y++) {
				for (int x = 0; x < a_Desc.width; x++) {

					int matrixIndex = 0;
					uint8_t RAccumulator = 0;
					uint8_t GAccumulator = 0;
					uint8_t BAccumulator = 0;

					uint32_t accum = 0;

					for (int r = -1; r < 2; r++)
					{
						for (int c = -1; c < 2; c++) {
							int y0 = std::clamp((y + r), 0, a_Desc.height);
							int x0 = std::clamp((x + c), 0, a_Desc.width);

							uint32_t PixelIndex = a_Desc.channelCount * (y0 * a_Desc.width + x0);

							RAccumulator += a_Desc.buffer[PixelIndex + 0] * a_Desc.kernel.kernel[matrixIndex] * a_Desc.kernel.multiplier;
							GAccumulator += a_Desc.buffer[PixelIndex + 1] * a_Desc.kernel.kernel[matrixIndex] * a_Desc.kernel.multiplier;
							BAccumulator += a_Desc.buffer[PixelIndex + 2] * a_Desc.kernel.kernel[matrixIndex] * a_Desc.kernel.multiplier;
							matrixIndex++;
						}
					}

					uint32_t PixelIndex = a_Desc.channelCount * (y * a_Desc.width + x);
					a_Desc.buffer[PixelIndex] = accum;
					a_Desc.buffer[PixelIndex + 0] = RAccumulator;
					a_Desc.buffer[PixelIndex + 1] = GAccumulator;
					a_Desc.buffer[PixelIndex + 2] = BAccumulator;
				}
			}

			return false;
		}
	}
}