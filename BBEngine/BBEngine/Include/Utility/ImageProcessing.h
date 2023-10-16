#pragma once
#include <cstdint>
#include "Thread/ThreadPool.h"

#define gaussian_blur5x5 BBE::Utility::Kernel{ GaussianBlur5x5BlurKernel, GaussianBlur5x5Multiplier, GaussianBlur5x5Size };
#define gaussian_blur BBE::Utility::Kernel{ GaussianBlurKernel, GaussianBlurMultiplier, GaussianBlurSize };
#define box_blur BBE::Utility::Kernel{ BoxBlurBlurKernel, BoxBlurMultiplier, BoxBlurSize };
#define sharpen BBE::Utility::Kernel{ SharpenKernel, SharpenMultiplier, SharpenSize };
#define ridge_detection BBE::Utility::Kernel{ RidgeDetectionKernel, RidgeDetectionMultiplier, RidgeDetectionSize };

constexpr float SharpenSize = 9;
constexpr float SharpenMultiplier = 1.f;
constexpr float SharpenKernel[3 * 3]{
	0, -1, 0,
	-1, 5, -1,
	0, -1, 0
};

constexpr float RidgeDetectionSize = 9;
constexpr float RidgeDetectionMultiplier = 1.f;
constexpr float RidgeDetectionKernel[3 * 3]{
	-1, -1, -1,
	-1, 8, -1,
	-1, -1, -1
};

constexpr float BoxBlurSize = 9;
constexpr float BoxBlurMultiplier = 1.f / 9.f;
constexpr float BoxBlurBlurKernel[3 * 3]{
	1, 1, 1,
	1, 1, 1,
	1, 1, 1
};

constexpr float GaussianBlurSize = 9;
constexpr float GaussianBlurMultiplier = 1.f / 16.f;
constexpr float GaussianBlurKernel[3 * 3]{
	1, 2, 1,
	2, 4, 2,
	1, 2, 1
};


constexpr float GaussianBlur5x5Size = 25;
constexpr float GaussianBlur5x5Multiplier = 1.f / 256.f;
constexpr float GaussianBlur5x5BlurKernel[5 * 5]{
	1, 4, 6, 4, 1,
	4, 16, 24, 16, 4,
	6, 24, 36, 24, 6,
	4, 16, 24, 16, 4,
	1, 4, 6, 4, 1,
};

namespace BBE {
	namespace Utility {

		struct Kernel {
			const float* kernel;
			float multiplier;
			float size;
		};

		struct ConvolutionDesc {
			uint8_t* buffer;
			int32_t width;
			int32_t height;
			uint32_t channelCount;

			Kernel kernel;
		};

		struct ThreadConvolutionDesc {
			ConvolutionDesc desc;

			uint8_t* OldData;
			uint32_t StartPosX;
			uint32_t StartPosY;
			uint32_t ThreadWidth;
			uint32_t ThreadHeight;
		};

		void ConvolutionSIMD(const ConvolutionDesc& a_Desc, Allocators::StackAllocator& a_Alloc);
		void Convolution(const ConvolutionDesc& a_Desc, Allocators::StackAllocator& a_Alloc);
		void ConvolutionMultiThreaded(const ConvolutionDesc& a_Desc, ThreadPool* a_ThreadPool, int a_Threads, BBE::Allocators::StackAllocator& a_Alloc);
	}
}