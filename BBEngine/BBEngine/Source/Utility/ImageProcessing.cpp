#include "Utility/ImageProcessing.h"
#include <algorithm>
#include "Utility/BBMemory.h"

namespace BBE {
	namespace Utility {

		void Convolution(const ConvolutionDesc& a_Desc, Allocators::StackAllocator& a_Alloc)
		{
			BBStackScope(a_Alloc) {
				uint32_t size = a_Desc.height * a_Desc.width * a_Desc.channelCount;
				uint8_t* oldData = BBNewArr(a_Alloc, size, uint8_t);
				memcpy(oldData, a_Desc.buffer, size);

				uint32_t range = sqrt(a_Desc.kernel.size);
				uint32_t filterHeight = range;
				uint32_t filterWidth = range;

				for (int y = 0; y < a_Desc.height; y++) {
					for (int x = 0; x < a_Desc.width; x++) {
						int kernelIndex = 0;
						float RAccumulator = 0.f;
						float GAccumulator = 0.f;
						float BAccumulator = 0.f;

						for (uint32_t kernalY = 0; kernalY < filterHeight; kernalY++) {
							for (uint32_t kernelX = 0; kernelX < filterWidth; kernelX++)
							{
								const uint32_t imageX = (x - filterWidth / 2 + kernelX + a_Desc.width) % a_Desc.width;
								const uint32_t imageY = (y - filterHeight / 2 + kernalY + a_Desc.height) % a_Desc.height;

								uint32_t PixelIndex = a_Desc.channelCount * (imageY * a_Desc.width + imageX);
								RAccumulator += oldData[PixelIndex + 0] * a_Desc.kernel.kernel[kernelIndex] * a_Desc.kernel.multiplier;
								GAccumulator += oldData[PixelIndex + 1] * a_Desc.kernel.kernel[kernelIndex] * a_Desc.kernel.multiplier;
								BAccumulator += oldData[PixelIndex + 2] * a_Desc.kernel.kernel[kernelIndex] * a_Desc.kernel.multiplier;
								kernelIndex++;
							}
						}

						uint32_t PixelIndex = a_Desc.channelCount * (y * a_Desc.width + x);
						a_Desc.buffer[PixelIndex + 0] = std::clamp(RAccumulator, 0.f, 255.f);
						a_Desc.buffer[PixelIndex + 1] = std::clamp(GAccumulator, 0.f, 255.f);
						a_Desc.buffer[PixelIndex + 2] = std::clamp(BAccumulator, 0.f, 255.f);
						PixelIndex = 0;
					}
				}
			}
		}

		void ConvolutionTask(void* ptr) {

			ThreadConvolutionDesc* desc = reinterpret_cast<ThreadConvolutionDesc*>(ptr);
			ConvolutionDesc convoDes = desc->desc;

			uint32_t range = sqrt(desc->desc.kernel.size);
			uint32_t kernelHeight = range;
			uint32_t kernelWidth = range;

			for (int y = desc->StartPosX; y < desc->ThreadHeight; y++) {
				for (int x = desc->StartPosY; x < desc->ThreadWidth; x++) {

					int matrixIndex = 0;
					float RAccumulator = 0;
					float GAccumulator = 0;
					float BAccumulator = 0;

					for (uint32_t kernalY = 0; kernalY < kernelHeight; kernalY++) {
						for (uint32_t kernelX = 0; kernelX < kernelWidth; kernelX++)
						{
							const uint32_t imageX = (x - kernelWidth / 2 + kernelX + desc->desc.width) % desc->desc.width;
							const uint32_t imageY = (y - kernelHeight / 2 + kernalY + desc->desc.height) % desc->desc.height;

							uint32_t PixelIndex = convoDes.channelCount * (imageY * convoDes.width + imageX);
							RAccumulator += desc->OldData[PixelIndex + 0] * convoDes.kernel.kernel[matrixIndex] * convoDes.kernel.multiplier;
							GAccumulator += desc->OldData[PixelIndex + 1] * convoDes.kernel.kernel[matrixIndex] * convoDes.kernel.multiplier;
							BAccumulator += desc->OldData[PixelIndex + 2] * convoDes.kernel.kernel[matrixIndex] * convoDes.kernel.multiplier;
							matrixIndex++;
						}
					}

					uint32_t PixelIndex = convoDes.channelCount * (y * convoDes.width + x);
					convoDes.buffer[PixelIndex + 0] = std::clamp(RAccumulator, 0.f, 255.f);
					convoDes.buffer[PixelIndex + 1] = std::clamp(GAccumulator, 0.f, 255.f);
					convoDes.buffer[PixelIndex + 2] = std::clamp(BAccumulator, 0.f, 255.f);
				}
			}
			printf("done \n");
		}

		void ConvolutionMultiThreaded(const ConvolutionDesc& a_Desc, ThreadPool* a_ThreadPool, int a_Threads, BBE::Allocators::StackAllocator& a_Alloc) {

			BBStackScope(a_Alloc) {
				uint32_t size = a_Desc.height * a_Desc.width * a_Desc.channelCount;
				uint8_t* oldData = BBNewArr(a_Alloc, size, uint8_t);
				memcpy(oldData, a_Desc.buffer, size);
				
				uint32_t threadHeight = a_Desc.height / a_Threads;
				uint32_t threadWidth = a_Desc.width;

				ThreadConvolutionDesc* desc = BBNewArr(a_Alloc, a_Threads, ThreadConvolutionDesc);
				BBTaskHandle* handle = BBNewArr(a_Alloc, a_Threads, BBTaskHandle);

				for (int i = 0; i < a_Threads; i++) {
					desc[i].desc = a_Desc;
					desc[i].OldData = oldData;
					desc[i].StartPosX = threadHeight * i;
					desc[i].StartPosY = 0;
					desc[i].ThreadHeight = desc[i].StartPosX + threadHeight;
					desc[i].ThreadWidth = threadWidth;
					a_ThreadPool->AddTask(ConvolutionTask, &desc[i], &handle[i]);
				}

				for (int i = 0; i < a_Threads; i++) {
					a_ThreadPool->WaitTillTaskIsDone(handle[i]);
				}
			}
		}
	}
}