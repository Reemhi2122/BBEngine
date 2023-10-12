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
			
					for (int r = -1; r < 2; r++)
					{
						for (int c = -1; c < 2; c++) {
							uint32_t y0 = std::clamp((y + r), 0, a_Desc.height - 1);
							uint32_t x0 = std::clamp((x + c), 0, a_Desc.width - 1);
			
							uint32_t PixelIndex = a_Desc.channelCount * (y0 * a_Desc.width + x0);
							RAccumulator += a_Desc.buffer[PixelIndex + 0] * a_Desc.kernel.kernel[matrixIndex] * a_Desc.kernel.multiplier;
							GAccumulator += a_Desc.buffer[PixelIndex + 1] * a_Desc.kernel.kernel[matrixIndex] * a_Desc.kernel.multiplier;
							BAccumulator += a_Desc.buffer[PixelIndex + 2] * a_Desc.kernel.kernel[matrixIndex] * a_Desc.kernel.multiplier;
							matrixIndex++;
						}
					}

					
					uint32_t PixelIndex = a_Desc.channelCount * (y * a_Desc.width + x);
					a_Desc.buffer[PixelIndex + 0] = RAccumulator;
					a_Desc.buffer[PixelIndex + 1] = GAccumulator;
					a_Desc.buffer[PixelIndex + 2] = BAccumulator;
				}
			}
			
			return false;
		}

		void ConvolutionTask(void* ptr) {

			ThreadConvolutionDesc* desc = reinterpret_cast<ThreadConvolutionDesc*>(ptr);
			ConvolutionDesc convoDes = desc->desc;

			for (int y = desc->StartPosX; y < desc->ThreadHeight; y++) {
				for (int x = desc->StartPosY; x < desc->ThreadWidth; x++) {

					int matrixIndex = 0;
					uint8_t RAccumulator = 0;
					uint8_t GAccumulator = 0;
					uint8_t BAccumulator = 0;

					for (int r = -1; r < 2; r++)
					{
						for (int c = -1; c < 2; c++) {
							int y0 = std::clamp((y + r), 0, convoDes.height - 1);
							int x0 = std::clamp((x + c), 0, convoDes.width - 1);

							uint32_t PixelIndex = convoDes.channelCount * (y0 * convoDes.width + x0);
							RAccumulator += convoDes.buffer[PixelIndex + 0] * convoDes.kernel.kernel[matrixIndex] * convoDes.kernel.multiplier;
							GAccumulator += convoDes.buffer[PixelIndex + 1] * convoDes.kernel.kernel[matrixIndex] * convoDes.kernel.multiplier;
							BAccumulator += convoDes.buffer[PixelIndex + 2] * convoDes.kernel.kernel[matrixIndex] * convoDes.kernel.multiplier;
							matrixIndex++;
						}
					}

					uint32_t PixelIndex = convoDes.channelCount * (y * convoDes.width + x);
					convoDes.buffer[PixelIndex + 0] = RAccumulator;
					convoDes.buffer[PixelIndex + 1] = GAccumulator;
					convoDes.buffer[PixelIndex + 2] = BAccumulator;
				}
			}
			printf("done \n");
		}

		void ConvolutionMultiThreaded(const ConvolutionDesc& a_Desc, ThreadPool* a_ThreadPool, int a_Threads) {
			uint32_t threadHeight = a_Desc.height / a_Threads;
			uint32_t threadWidth = a_Desc.width;

			ThreadConvolutionDesc desc[16] = {};
			BBTaskHandle handle[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

			for (int i = 0; i < a_Threads; i++) {
				desc[i].desc = a_Desc;
				desc[i].StartPosX = threadHeight * i;
				desc[i].StartPosY = 0;
				desc[i].ThreadHeight = desc[i].StartPosX + threadHeight;
				desc[i].ThreadWidth = threadWidth;
				a_ThreadPool->AddTask(ConvolutionTask, &desc[i], &handle[i]);
			}

			for (int i = 0; i < 8; i++) {
				a_ThreadPool->WaitTillTaskIsDone(handle[i]);
			}
		}
	}
}