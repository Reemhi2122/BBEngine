#pragma once
#include <cstdint>
#define gaussian_blur BBE::Utility::Kernel{ gaussianBlurKernel, gaussianBlurMultiplier, gaussianBlurSize };

#include "Thread/ThreadPool.h"

constexpr float gaussianBlurSize = 9;
constexpr float gaussianBlurMultiplier = 1.f / 16.f;
constexpr float gaussianBlurKernel[3 * 3]{
	1, 2, 1,
	2, 4, 2,
	1, 2, 1
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

			uint32_t StartPosX;
			uint32_t StartPosY;
			uint32_t ThreadWidth;
			uint32_t ThreadHeight;
		};

		bool Convolution(const ConvolutionDesc& a_Desc);
		void ConvolutionMultiThreaded(const ConvolutionDesc& a_Desc, ThreadPool* a_ThreadPool, int a_Threads);
	}
}